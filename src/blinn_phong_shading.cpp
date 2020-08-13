#include "blinn_phong_shading.h"

#include <iostream>

#include "first_hit.h"
//#include "AABBTree.h"

Eigen::Vector3d blinn_phong_shading(
    const Ray& ray,
    const int& hit_id,
    const double& t,
    const Eigen::Vector3d& n,
    const std::vector<std::shared_ptr<Object> >& objects,
    const std::vector<std::shared_ptr<Light> >& lights) {
  ////////////////////////////////////////////////////////////////////////////
  Eigen::Vector3d color = Eigen::Vector3d(0, 0, 0);
  Eigen::Vector3d ambient = Eigen::Vector3d(0, 0, 0);
  Eigen::Vector3d diffuse = Eigen::Vector3d(0, 0, 0);
  Eigen::Vector3d specular = Eigen::Vector3d(0, 0, 0);

  const Eigen::Vector3d ka = objects[hit_id]->material->ka;
  const Eigen::Vector3d kd = objects[hit_id]->material->kd;
  const Eigen::Vector3d ks = objects[hit_id]->material->ks;
  const double p = objects[hit_id]->material->phong_exponent;
  Eigen::Vector3d h;

  double max_t;
  Ray light_ray;
  light_ray.origin = ray.origin + t * ray.direction;
  int hit_id_light;
  double t_light;
  Eigen::Vector3d n_light;

  for (std::shared_ptr<Light> light : lights) {
    light->direction(light_ray.origin, light_ray.direction, max_t);

    // if the object is not in shadow
    if (!first_hit(light_ray, 1e-6, objects, hit_id_light, t_light, n_light) || t_light > max_t) {
      diffuse += (kd.array() * light->I.array() * std::max(0.0, n.dot(light_ray.direction))).matrix();
      h = (-ray.direction.normalized() + light_ray.direction).normalized();
      specular += (ks.array() * light->I.array() * pow(std::max(0.0, n.dot(h)), p)).matrix();
    }
  }

  ambient += (ka.array() * Eigen::Vector3d(0.1, 0.1, 0.1).array()).matrix();
  color = ambient + specular + diffuse;
  return color;
  ////////////////////////////////////////////////////////////////////////////
}

// Eigen::Vector3d blinn_phong_shading(
//     const Ray& ray,
//     const std::shared_ptr<AABBTree>& tree,
//     const double& t,
//     const Eigen::Vector3d& n,
//     const std::vector<std::shared_ptr<Light> >& lights,
//     const std::shared_ptr<Object>& descendant) {
//   ////////////////////////////////////////////////////////////////////////////
//   Eigen::Vector3d color = Eigen::Vector3d(0, 0, 0);
//   Eigen::Vector3d ambient = Eigen::Vector3d(0, 0, 0);
//   Eigen::Vector3d diffuse = Eigen::Vector3d(0, 0, 0);
//   Eigen::Vector3d specular = Eigen::Vector3d(0, 0, 0);

//   const Eigen::Vector3d ka = descendant->material->ka;
//   const Eigen::Vector3d kd = descendant->material->kd;
//   const Eigen::Vector3d ks = descendant->material->ks;
//   const double p = descendant->material->phong_exponent;
//   Eigen::Vector3d h;

//   double max_t;
//   Ray light_ray;
//   light_ray.origin = ray.origin + t * ray.direction;
//   double t_light;
//   Eigen::Vector3d n_light;
//   std::shared_ptr<Object>& descendant_light;

//   for (std::shared_ptr<Light> light : lights) {
//     light->direction(light_ray.origin, light_ray.direction, max_t);

//     // if the object is not in shadow
//     if (!first_hit(light_ray, 1e-6, tree, t_light, n_light, descendant_light) || t_light > max_t) {
//       diffuse += (kd.array() * light->I.array() * std::max(0.0, n.dot(light_ray.direction))).matrix();
//       h = (-ray.direction.normalized() + light_ray.direction).normalized();
//       specular += (ks.array() * light->I.array() * pow(std::max(0.0, n.dot(h)), p)).matrix();
//     }
//   }

//   ambient += (ka.array() * Eigen::Vector3d(0.1, 0.1, 0.1).array()).matrix();
//   color = ambient + specular + diffuse;
//   return color;
  ////////////////////////////////////////////////////////////////////////////
//}


