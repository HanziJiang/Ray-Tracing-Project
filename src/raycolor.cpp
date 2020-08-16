#include "raycolor.h"

#include <iostream>

#include "blinn_phong_shading.h"
#include "first_hit.h"
#include "reflect.h"
#include "refract.h"
//#include "AABBTree.h"

#define PRINT_VEC(d) \
  (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")
#define PRINT_NUM(d) (std::cout << #d << ": " << d << "\n")

bool raycolor(const Ray &ray, const double min_t,
              const std::vector<std::shared_ptr<Object>> &objects,
              const std::vector<std::shared_ptr<Light>> &lights,
              const int num_recursive_calls, Eigen::Vector3d &rgb) {
  ////////////////////////////////////////////////////////////////////////////
  int hit_id;
  double t;
  Eigen::Vector3d n;

  if (!first_hit(ray, min_t, objects, hit_id, t, n)) {
    rgb = Eigen::Vector3d(0, 0, 0);
    return false;
  }

  const bool is_transparent = objects[hit_id]->material->kt > 0.97;

  Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
  Eigen::Vector3d bf_color = blinn_phong_shading(ray, hit_id, t, n, objects, lights);

  if (num_recursive_calls > 5) {
    if (!is_transparent) rgb = bf_color;
    return true;
  }

  // reflection
  Eigen::Vector3d reflect_color(0, 0, 0);
  Ray reflected_ray;
  reflected_ray.origin = ray.origin + ray.direction * t;
  reflected_ray.direction = reflect(ray.direction, n);
  if (raycolor(reflected_ray, 1e-6, objects, lights, num_recursive_calls + 1,
               reflect_color)) {
    reflect_color =
        (objects[hit_id]->material->km.array() * reflect_color.array())
            .matrix();
  }

  if (!is_transparent) {
    rgb = reflect_color + bf_color;
    return true;
  }

  // refraction
  float kr;
  fresnel(ray.direction, n, 1.5, kr);

  // Total internal reflection
  if (kr >= 1) {
    rgb = reflect_color;
    return true;
  }

  bool outside = ray.direction.dot(n) < 0;
  Ray refrated_ray;
  refrated_ray.origin = outside ? hit_point - 1e-6 *n : refrated_ray.origin = hit_point + 1e-6 * n;
  refrated_ray.direction = refract(ray.direction, n, 1.5);

  Eigen::Vector3d refract_color(0, 0, 0);
  if (raycolor(refrated_ray, 1e-6, objects, lights, num_recursive_calls + 1,
               refract_color)) {
    rgb = reflect_color * kr + refract_color * (1 - kr);
    // on the sphere
    //PRINT_VEC(rgb);
  } else {
    rgb = reflect_color + bf_color;
  }

  return true;
  ////////////////////////////////////////////////////////////////////////////
}


// bool raycolor(const Ray &ray, const double min_t,
//               const std::shared_ptr<AABBTree>& tree,
//               const std::vector<std::shared_ptr<Light>> &lights,
//               const int num_recursive_calls, Eigen::Vector3d &rgb) {
//   ////////////////////////////////////////////////////////////////////////////
//   int hit_id;
//   double t;
//   Eigen::Vector3d n;
//   std::shared_ptr<Object> descendant;

//   if (!first_hit(ray, min_t, tree, t, n, descendant)) {
//     rgb = Eigen::Vector3d(0, 0, 0);
//     return false;
//   }

//   Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
//   rgb = blinn_phong_shading(ray, tree, t, n, lights, descendant);

//   if (num_recursive_calls > 5) {
//     return true;
//   }

//   // reflection
//   Eigen::Vector3d reflect_color(0, 0, 0);
//   Ray reflected_ray;
//   reflected_ray.origin = ray.origin + ray.direction * t;
//   reflected_ray.direction = reflect(ray.direction, n);
//   if (raycolor(reflected_ray, 1e-6, tree, lights, num_recursive_calls + 1,
//                reflect_color)) {
//     reflect_color =
//         (descendant->material->km.array() * reflect_color.array())
//             .matrix();
//   }

//   if (descendant->material->kt < 0.97) {
//     rgb += reflect_color;
//     return true;
//   }

//   // refraction
//   float kr;
//   fresnel(ray.direction, n, 1.5, kr);

//   // Total internal reflection
//   if (kr >= 1) {
//     rgb += reflect_color;
//     return true;
//   }

//   bool outside = ray.direction.dot(n) < 0;
//   Ray refrated_ray;
//   refrated_ray.origin = outside ? hit_point - 1e-6 *n : refrated_ray.origin = hit_point + 1e-6 * n;
//   refrated_ray.direction = refract(ray.direction, n, 1.5);

//   Eigen::Vector3d refract_color(0, 0, 0);
//   if (raycolor(refrated_ray, 1e-6, tree, lights, num_recursive_calls + 1,
//                refract_color)) {
//     rgb += reflect_color * kr + refract_color * (1 - kr);
//   } else {
//     rgb += reflect_color;
//   }

//   return true;
//   ////////////////////////////////////////////////////////////////////////////
// }
