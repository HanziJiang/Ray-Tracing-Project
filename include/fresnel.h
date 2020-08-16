#ifndef FRESNEL_h
#define FRESNEL_h
#include <Eigen/Core>

// Calculate fresnel factor, which is the percenrage of light that is reflected
//
// Inputs:
//   in  incoming _unit_ ray direction
//   n   surface _unit_ normal about which to reflect
//   ior refraction index of object

// Output:
//   kr  frenel factor
void fresnel(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const double &ior, double &kr);

#endif
