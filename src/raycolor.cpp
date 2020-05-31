#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  int hit_id;
  double t;
  Eigen::Vector3d n;
  if (first_hit(ray, min_t, objects, hit_id, t, n)) {
    rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    if (num_recursive_calls <= 10) {
      Eigen::Vector3d rgb_temp;
      Ray reflected_ray;
      reflected_ray.origin = ray.origin + ray.direction * t;
      reflected_ray.direction = reflect(ray.direction, n);
      if (raycolor(reflected_ray, 1e-6, objects, lights, num_recursive_calls + 1, rgb_temp))
      {
        rgb += (objects[hit_id]->material->km.array() * rgb_temp.array()).matrix();
      }
    }
    return true;
  }
  rgb = Eigen::Vector3d(0,0,0);
  return false;
  ////////////////////////////////////////////////////////////////////////////
}
