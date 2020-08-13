#include "first_hit.h"

#include <limits>

//#include "AABBTree.h"
//#include "ray_intersect_box.h"

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

// bool first_hit(
//     const Ray& ray,
//     const double min_t,
//     const std::shared_ptr<AABBTree>& tree,
//     double& t,
//     Eigen::Vector3d& n,
//     std::shared_ptr<Object> & descendant) {
//   ////////////////////////////////////////////////////////////////////////////
//   // TODO: Set n
//   if (tree == NULL || tree == 0) return false;
//   if (!ray_intersect_box(ray, tree->box, min_t)) return false;

//   t = std::numeric_limits<double>::infinity();
//   double left_t, right_t;
//   Eigen::Vector3d left_n, right_n;
//   std::shared_ptr<Object> left_descendant, right_descendant;

//   bool i1 = (tree->left == NULL || tree->left == 0) ? false : first_hit(ray, min_t, tree->left, left_t, left_n, left_descendant);
//   bool i2 = (tree->right == NULL || tree->right == 0) ? false : first_hit(ray, min_t, tree->right, right_t, right_n, right_descendant);

//   std::shared_ptr<AABBTree> AABB_attempt;

//   if (i1) {
//     AABB_attempt = std::dynamic_pointer_cast<AABBTree>(tree->left);

//     // if it is a leaf
//     if (!AABB_attempt) {
//       left_descendant = tree->left;
//     }
//   }

//   if (i2) {
//     AABB_attempt = std::dynamic_pointer_cast<AABBTree>(tree->right);

//     // if it is a leaf
//     if (!AABB_attempt) {
//       right_descendant = tree->right;
//     }
//   }

//   if (i1 && i2) {
//     if (left_t <= right_t) {
//       t = left_t;
//       n = left_n;
//       descendant = left_descendant;
//     } else {
//       t = right_t;
//       n = right_n;
//       descendant = right_descendant;
//     }
//   } else if (i1) {
//     t = left_t;
//     n = left_n;
//     descendant = left_descendant;
//   } else if (i2) {
//     t = right_t;
//     n = right_n;
//     descendant = right_descendant;
//   }
  
//   return (!isinf(t) && descendant && descendant != 0);

////////////////////////////////////////////////////////////////////////////
//}
