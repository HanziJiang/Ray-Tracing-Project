#include "viewing_ray.h"

void viewing_ray(
    const Camera& camera,
    const int i,
    const int j,
    const int width,
    const int height,
    Ray& ray) {
  ////////////////////////////////////////////////////////////////////////////
  ray.origin = camera.e;

  double u = camera.width / width * (j + 0.5) - camera.width / 2.0;
  double v = -camera.height / height * (i + 0.5) + camera.height / 2.0;

  Eigen::Vector3d pixel_position(u, v, camera.d);

  ray.direction = pixel_position[0] * camera.u + pixel_position[1] * camera.v - pixel_position[2] * camera.w;
  ////////////////////////////////////////////////////////////////////////////
}
