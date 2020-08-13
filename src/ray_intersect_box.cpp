#include "ray_intersect_box.h"

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t)
{
  ////////////////////////////////////////////////////////////////////////////
  double t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;

  const double x_min = box.min_corner[0];
  const double y_min = box.min_corner[1];
  const double z_min = box.min_corner[2];
  const double x_max = box.max_corner[0];
  const double y_max = box.max_corner[1];
  const double z_max = box.max_corner[2];

  const double x_direction = ray.direction[0];
  const double y_direction = ray.direction[1];
  const double z_direction = ray.direction[2];

  const double x_e = ray.origin[0];
  const double y_e = ray.origin[1];
  const double z_e = ray.origin[2];

  const double a_x = 1.0/x_direction;
  if (a_x >= 0) {
    t_x_min = (x_min - x_e) * a_x;
    t_x_max = (x_max - x_e) * a_x;
  } else {
    t_x_max = (x_min - x_e) * a_x;
    t_x_min = (x_max - x_e) * a_x;
  }

  const double a_y = 1.0/y_direction;
  if (a_y >= 0) {
    t_y_min = (y_min - y_e) * a_y;
    t_y_max = (y_max - y_e) * a_y;
  } else {
    t_y_max = (y_min - y_e) * a_y;
    t_y_min = (y_max - y_e) * a_y;
  }

  const double a_z = 1.0/z_direction;
  if (a_z >= 0) {
    t_z_min = (z_min - z_e) * a_z;
    t_z_max = (z_max - z_e) * a_z;
  } else {
    t_z_max = (z_min - z_e) * a_z;
    t_z_min = (z_max - z_e) * a_z;
  }

  return !((t_x_max < t_y_min) || (t_y_max < t_x_min) || (t_x_max < t_z_min) || (t_z_max < t_x_min) || (t_y_max < t_z_min) || (t_z_max < t_y_min));
  ////////////////////////////////////////////////////////////////////////////
}
