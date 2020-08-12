#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include "refract.h"
#include <iostream>

#define PRINT_VEC(d) (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")
#define PRINT_NUM(d) (std::cout << #d << ": " << d << "\n")

bool raycolor(
    const Ray &ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights,
    const int num_recursive_calls,
    Eigen::Vector3d &rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  int hit_id;
  double t;
  Eigen::Vector3d n;

  if (first_hit(ray, min_t, objects, hit_id, t, n))
  {
    Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
    rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    if (num_recursive_calls <= 10)
    {
      // reflection
      Eigen::Vector3d rgb_temp_reflect(0, 0, 0);
      Ray reflected_ray;
      reflected_ray.origin = ray.origin + ray.direction * t;
      reflected_ray.direction = reflect(ray.direction, n);
      if (raycolor(reflected_ray, 1e-6, objects, lights, num_recursive_calls + 1, rgb_temp_reflect))
      {
        rgb_temp_reflect = (objects[hit_id]->material->km.array() * rgb_temp_reflect.array()).matrix();
      }

      if (objects[hit_id]->material->kt >= 0.95)
      {
        //refraction
        Eigen::Vector3d rgb_temp_refract(0, 0, 0);
        float kr;
        fresnel(ray.direction, n, 1.5, kr);

        // compute refraction if it is not a case of total internal reflection
        if (kr < 1)
        {
          bool outside = ray.direction.dot(n) < 0;
          Ray refrated_ray;
          if (outside)
          {
            refrated_ray.origin = hit_point - 1e-6 * n;
          }
          else
          {
            refrated_ray.origin = hit_point + 1e-6 * n;
          }

          refrated_ray.direction = refract(ray.direction, n, 1.5);
          if (raycolor(refrated_ray, 1e-6, objects, lights, num_recursive_calls + 1, rgb_temp_refract))
          {
            rgb += rgb_temp_reflect * kr + rgb_temp_refract * (1 - kr);
            return true;
          }
        }
      }
      rgb += rgb_temp_reflect;
    }
    return true;
  }

  // if the ray does not hit any object
  rgb = Eigen::Vector3d(0, 0, 0);
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
