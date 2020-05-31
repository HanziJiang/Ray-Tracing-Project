#include "Plane.h"
#include "Ray.h"

#include <iostream>

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // ray will never intersect with this plane if the former's direction vector is orthogonal to the latter's normal vector
  if (normal.dot(ray.direction) == 0.0) return false;
  
  const double T = (- 1 * normal.dot(ray.origin - point)) / (normal.dot(ray.direction));
  if (T >= min_t) {
    t = T;
    n = normal.normalized();
    return true;
  }
  return false;
  ////////////////////////////////////////////////////////////////////////////
}

