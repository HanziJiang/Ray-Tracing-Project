#ifndef REFRACT_H
#define REFRACT_H
#include <Eigen/Core>

// Refract an incoming ray
//
// Inputs:
//   in  incoming _unit_ ray direction
//   n   surface _unit_ normal about which to refract
//   ior refraction index of object

// Returns refracted _unit_ ray direction
Eigen::Vector3d refract(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const double &ior);
#endif 
