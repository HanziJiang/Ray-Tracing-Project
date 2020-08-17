#include <Eigen/Core>
#include "refract.h"
// Reference: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

Eigen::Vector3d refract(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const double &ior) {
  double cosi = in.dot(n);
  if (cosi < -1)
    cosi = -1;
  else if (cosi > 1)
    cosi = 1;

  double ior_1 = 1.0, ior_2 = ior;

  Eigen::Vector3d n_copy = n;


  // From inside of object to outside
  if (cosi >= 0) {
    std::swap(ior_1, ior_2);
    n_copy = -n;
  } else {
    cosi = std::abs(cosi);
  }
  
  double ratio = ior_1 / ior_2;
  double k = 1 - ratio * ratio * (1 - cosi * cosi);
  return ((k < 0) ? Eigen::Vector3d(0.0, 0.0, 0.0) : (ratio * in + (ratio * cosi - sqrt(k)) * n_copy)).normalized();
}
