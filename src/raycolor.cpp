#include "raycolor.h"

#include <iostream>

#include "blinn_phong_shading.h"
#include "first_hit.h"
#include "reflect.h"
#include "refract.h"
#include "fresnel.h"
//#include "AABBTree.h"
#define PRINT_VEC(d) (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")

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

  Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
  rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);

  if (num_recursive_calls > 5) {
    return true;
  }

  // reflection
  Eigen::Vector3d rgb_temp_reflect(0, 0, 0);
  Ray reflected_ray;
  reflected_ray.origin = ray.origin + ray.direction * t;
  reflected_ray.direction = reflect(ray.direction, n);
  if (raycolor(reflected_ray, 1e-6, objects, lights, num_recursive_calls + 1,
               rgb_temp_reflect)) {
    rgb_temp_reflect =
        (objects[hit_id]->material->km.array() * rgb_temp_reflect.array())
            .matrix();
  }

  if (objects[hit_id]->material->kt < 0.97) {
    rgb += rgb_temp_reflect;
    return true;
  }

  // refraction
  double kr;
  double ior = objects[hit_id]->material->ior;
  
  fresnel(ray.direction, n, ior, kr);
  

  // Total internal reflection
  if (kr >= 1) {
    rgb += rgb_temp_reflect;
    return true;
  }

  bool outside = ray.direction.dot(n) < 0;
  Ray refrated_ray;
  refrated_ray.origin = outside ? hit_point - 1e-6 *n : refrated_ray.origin = hit_point + 1e-6 * n;
  refrated_ray.direction = refract(ray.direction, n, ior);

  Eigen::Vector3d rgb_temp_refract(0, 0, 0);
  if (raycolor(refrated_ray, 1e-6, objects, lights, num_recursive_calls + 1,
               rgb_temp_refract)) {
    rgb += rgb_temp_reflect * kr + rgb_temp_refract * (1 - kr);
  } else {
    rgb += rgb_temp_reflect;
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
//   Eigen::Vector3d rgb_temp_reflect(0, 0, 0);
//   Ray reflected_ray;
//   reflected_ray.origin = ray.origin + ray.direction * t;
//   reflected_ray.direction = reflect(ray.direction, n);
//   if (raycolor(reflected_ray, 1e-6, tree, lights, num_recursive_calls + 1,
//                rgb_temp_reflect)) {
//     rgb_temp_reflect =
//         (descendant->material->km.array() * rgb_temp_reflect.array())
//             .matrix();
//   }

//   if (descendant->material->kt < 0.97) {
//     rgb += rgb_temp_reflect;
//     return true;
//   }

//   // refraction
//   double kr;
//   fresnel(ray.direction, n, 1.5, kr);

//   // Total internal reflection
//   if (kr >= 1) {
//     rgb += rgb_temp_reflect;
//     return true;
//   }

//   bool outside = ray.direction.dot(n) < 0;
//   Ray refrated_ray;
//   refrated_ray.origin = outside ? hit_point - 1e-6 *n : refrated_ray.origin = hit_point + 1e-6 * n;
//   refrated_ray.direction = refract(ray.direction, n, 1.5);

//   Eigen::Vector3d rgb_temp_refract(0, 0, 0);
//   if (raycolor(refrated_ray, 1e-6, tree, lights, num_recursive_calls + 1,
//                rgb_temp_refract)) {
//     rgb += rgb_temp_reflect * kr + rgb_temp_refract * (1 - kr);
//   } else {
//     rgb += rgb_temp_reflect;
//   }

//   return true;
//   ////////////////////////////////////////////////////////////////////////////
// }
