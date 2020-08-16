#include <Eigen/Core>
#include "fresnel.h"
// Reference: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

void fresnel(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const double &ior, double &kr) {
  double cosi = in.dot(n);
  if (cosi < -1)
    cosi = -1;
  else if (cosi > 1)
    cosi = 1;

  double ior_1 = 1.0, ior_2 = ior;

  // If the ray is from inside object, ior_1 should be the larger of the two
  if (cosi > 0) {
    std::swap(ior_1, ior_2);
  }

  // Snell's law
  double sint = ior_1 / ior_2 * sqrt(std::max(0.0, 1 - cosi * cosi));

  if (sint >= 1) {
    // Total internal reflection
    kr = 1;
  } else {
    double cost = sqrt(std::max(0.0, 1 - sint * sint));
    cosi = std::abs(cosi);
    double Rs = ((ior_2 * cosi) - (ior_1 * cost)) / ((ior_2 * cosi) + (ior_1 * cost));
    double Rp = ((ior_1 * cosi) - (ior_2 * cost)) / ((ior_1 * cosi) + (ior_2 * cost));

    kr = (Rs * Rs + Rp * Rp) / 2.0;
  }
}