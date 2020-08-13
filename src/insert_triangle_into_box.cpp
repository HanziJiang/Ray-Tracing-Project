// #include "insert_triangle_into_box.h"

// double min(const double num1, const double num2, const double num3, const double num4) {
//   return (std::min(std::min(num1, num2), std::min(num3, num4)));
// }

// double max(const double num1, const double num2, const double num3, const double num4) {
//   return (std::max(std::max(num1, num2), std::max(num3, num4)));
// }

// void insert_triangle_into_box(
//   const Eigen::RowVector3d & a,
//   const Eigen::RowVector3d & b,
//   const Eigen::RowVector3d & c,
//   BoundingBox & B)
// {
//   ////////////////////////////////////////////////////////////////////////////
//   for (int i = 0; i < 3; i++) {
//     B.min_corner[i] = min(a[i], b[i], c[i], B.min_corner[i]);
//     B.max_corner[i] = max(a[i], b[i], c[i], B.max_corner[i]);
//   }

//   ////////////////////////////////////////////////////////////////////////////
// }

