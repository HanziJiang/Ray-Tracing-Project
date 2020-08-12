#ifndef REFRACT_H
#define REFRACT_H
#include <Eigen/Core>

// Calculate fresnel
//
// Inputs:
//   in  incoming _unit_ ray direction
//   n   surface _unit_ normal about which to reflect
//   ior refraction index of object

// Output:
//   kr  frenel
void fresnel(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const float &ior, float &kr);


// Refract an incoming ray into an out going ray
//
// Inputs:
//   in  incoming _unit_ ray direction
//   n   surface _unit_ normal about which to reflect

// Returns refracted _unit_ ray direction
Eigen::Vector3d refract(const Eigen::Vector3d &I, const Eigen::Vector3d &N, const float &ior);
#endif 
