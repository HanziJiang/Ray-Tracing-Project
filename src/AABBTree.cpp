// #include "AABBTree.h"
// #include "insert_box_into_box.h"

// AABBTree::AABBTree(
//   const std::vector<std::shared_ptr<Object> > & objects,
//   int a_depth): 
//   depth(std::move(a_depth)), 
//   num_leaves(objects.size())
// {
//   ////////////////////////////////////////////////////////////////////////////
//   const int num_object = objects.size();

//   if (num_object == 0) {
//     left = NULL;
//     right = NULL;
//   } else if (num_object == 1) {
//     left = objects[0];
//     right = NULL;
//     insert_box_into_box(objects[0]->box, box);
//   } else if (num_object == 2) {
//     left = objects[0];
//     right = objects[1];
//     insert_box_into_box(objects[0]->box, box);
//     insert_box_into_box(objects[1]->box, box);
//   } else {
//     for (int i = 0; i < num_object; i++) {
//       insert_box_into_box(objects[i]->box, box);
//     }

//     const double x_length = abs(box.max_corner[0] - box.min_corner[0]);
//     const double y_length = abs(box.max_corner[1] - box.min_corner[1]);
//     const double z_length = abs(box.max_corner[2] - box.min_corner[2]);

//     int axis;
//     axis = (x_length > y_length) ? 0 : 1;
//     if (z_length > x_length && z_length > y_length) axis = 2;

//     // the midpoint of the loggest side
//     const double m = (box.max_corner[axis] + box.min_corner[axis]) / 2;

//     // divide objects into two lists, according to m
//     std::vector<std::shared_ptr<Object>> left_objects;
//     std::vector<std::shared_ptr<Object>> right_objects;
//     for (int i = 0; i < num_object; i++) {
//       if ((objects[i]->box).center()[axis] < m) {
//         left_objects.push_back(objects[i]);
//       } else {
//         right_objects.push_back(objects[i]);
//       }
//     }

//     // if either left list or right list is empty, move half of the elements from the non-empty list to the empty list
//     const int left_size = left_objects.size();
//     const int right_size = right_objects.size();
//     if (left_size == 0) {
//       for (int i = right_size - 1; i >= right_size / 2; i--) {
//         left_objects.push_back(right_objects[i]);
//         right_objects.pop_back();
//       }
//     }
//     else if (right_size == 0) {
//       for (int i = left_size - 1; i >= left_size / 2; i--) {
//         right_objects.push_back(left_objects[i]);
//         left_objects.pop_back();
//       }
//     }

//     left = (left_objects.size() == 1) ? left_objects[0] : std::make_shared<AABBTree>(left_objects, a_depth + 1);
//     right = (right_objects.size() == 1) ? right_objects[0] : std::make_shared<AABBTree>(right_objects, a_depth + 1);
//   }
//   ////////////////////////////////////////////////////////////////////////////
// }
