#include "Sphere.h"
#include "Ray.h"

#include <limits>

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // 1 if t is updated, 0 otherwise
  bool t_updated = 0;
  
  const double a = ray.direction.dot(ray.direction);
  const double b = 2 * ray.direction.dot(ray.origin - center);
  const double c = (ray.origin - center).dot(ray.origin - center) - radius * radius;
  
  const double discriminant = b * b - 4 * a * c;
  
  // there is no solution
  if (discriminant < 0) return false;
  
  const double numerator1 = (-b) + sqrt(discriminant);
  const double numerator2 = (-b) - sqrt(discriminant);
  const double denominator = 2 * a;
  
  // denominator cannot be zero
  if (denominator == 0) return false;

  const double T1 = numerator1 / denominator;
  const double T2 = numerator2 / denominator;
  
  // Check if T1 is a qualified value
  if (T1 >= min_t) {
    t = T1;
    t_updated = 1;
  }
  
  // Check if T2 is a qualified value
  if (T2 >= min_t) {
    if ((t_updated && T2 < t) || (!t_updated)) {
        t = T2;
        t_updated = 1;
    }
  }
  
  // if t has been updated to T1 or T2
  if (t_updated) {
    Eigen::Vector3d intersect_point = ray.origin + t * (ray.direction);
    n = (intersect_point - center).normalized();
    return true;
  }
  
  return false;
  
  ////////////////////////////////////////////////////////////////////////////
}

