#include <Eigen/Core>

void fresnel(const Eigen::Vector3d &in, const Eigen::Vector3d &n, const float &ior, float &kr) {
  float cosi = in.dot(n);
  if (cosi < -1)
    cosi = -1;
  else if (cosi > 1)
    cosi = 1;
  float etai = 1, etat = ior;
  if (cosi > 0) {
    std::swap(etai, etat);
  }
  // Compute sini using Snell's law
  float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
  // Total internal reflection
  if (sint >= 1) {
    kr = 1;
  } else {
    float cost = sqrtf(std::max(0.f, 1 - sint * sint));
    cosi = fabsf(cosi);
    float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
    kr = (Rs * Rs + Rp * Rp) / 2;
  }
}

Eigen::Vector3d refract(const Eigen::Vector3d &I, const Eigen::Vector3d &N, const float &ior) {
  float cosi = I.dot(N);
  if (cosi < -1)
    cosi = -1;
  else if (cosi > 1)
    cosi = 1;
  float etai = 1, etat = ior;
  Eigen::Vector3d n = N;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n = -N;
  }
  float eta = etai / etat;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  return ((k < 0) ? Eigen::Vector3d(0, 0, 0) : (eta * I + (eta * cosi - sqrtf(k)) * n)).normalized();
}
