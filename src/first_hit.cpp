#include "first_hit.h"

#include <limits>

bool first_hit(
    const Ray& ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object> >& objects,
    int& hit_id,
    double& t,
    Eigen::Vector3d& n) {
  ////////////////////////////////////////////////////////////////////////////
  t = std::numeric_limits<double>::max();
  hit_id = -1;
  int index = 0;
  double t_curr;
  Eigen::Vector3d n_curr;

  for (std::shared_ptr<Object> obj_ptr : objects) {
    if (obj_ptr->intersect(ray, min_t, t_curr, n_curr)) {
      if (t_curr < t) {
        t = t_curr;
        n = n_curr;
        hit_id = index;
      }
    }
    index++;
  }

  return (hit_id != -1);
  ////////////////////////////////////////////////////////////////////////////
}
