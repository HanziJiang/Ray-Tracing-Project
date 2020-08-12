#include "Triangle.h"
#include "Ray.h"

#include <Eigen/Geometry>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // formulas from textbook
  
  Eigen::Vector3d point1 = std::get<0>(corners);
  Eigen::Vector3d point2 = std::get<1>(corners);
  Eigen::Vector3d point3 = std::get<2>(corners);
  
  const double a = point1[0] - point2[0];
  const double b = point1[1] - point2[1];
  const double c = point1[2] - point2[2];
  const double d = point1[0] - point3[0];
  const double e = point1[1] - point3[1];
  const double f = point1[2] - point3[2];
  const double g = ray.direction[0];
  const double h = ray.direction[1];
  const double i = ray.direction[2];
  const double j = point1[0] - ray.origin[0];
  const double k = point1[1] - ray.origin[1];
  const double l = point1[2] - ray.origin[2];
  
  const double M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
  if (M == 0) return false;

  const double T = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / M;
  if (T < min_t) return false;
  
  const double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / M;
  if (gamma < 0 || gamma > 1) return false;
  
  const double beta = (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / M;
  if (beta < 0 || beta > 1 - gamma) return false;
  
  t = T;
  n = (point3 - point2).cross(point1 - point2).normalized();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}


