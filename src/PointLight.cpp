#include "PointLight.h"

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
  ////////////////////////////////////////////////////////////////////////////
  const Eigen::Vector3d direction = p - q;
  d = direction.normalized();
  max_t = direction.norm();
  ////////////////////////////////////////////////////////////////////////////
}
