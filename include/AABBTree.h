// #ifndef AABBTREE_H
// #define AABBTREE_H

// #include "BoundingBox.h"
// #include "Object.h"
// #include <Eigen/Core>
// #include <memory>
// #include <vector>

// struct AABBTree : public Object, public std::enable_shared_from_this<AABBTree>
// {
//   // Pointers to left and right subtree branches. These could be another
//   // AABBTree (internal node) or a leaf (primitive Object like MeshTriangle, or
//   // CloudPoint)
//   std::shared_ptr<Object> left;
//   std::shared_ptr<Object> right;
//   // For debugging, keep track of the depth (root has depth == 0)
//   int depth;
//   // For debugging, keep track of the number leaf, descendants 
//   int num_leaves;
//   // Construct a axis-aligned bounding box tree given a list of objects. Use the
//   // midpoint along the longest axis of the box containing the given objects to
//   // determine the left-right split.
//   //
//   // Inputs:
//   //   objects  list of objects to store in this AABBTree
//   //   Optional inputs:
//   //     depth  depth of this tree (usually set by constructor of parent as
//   //       their depth+1)
//   // Side effects: num_leaves is set to objects.size() and left/right pointers
//   // set to subtrees or leaf Objects accordingly.
//   AABBTree(
//     const std::vector<std::shared_ptr<Object> > & objects, 
//     int depth=0);
// };

// #endif
