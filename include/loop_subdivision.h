#ifndef LOOP_SUBDIVISION_H
#define LOOP_SUBDIVISION_H
#include <iostream>

// Conduct num_iters iterations of Charles-Loop subdivision on a **pure trig**
// mesh (V,F).
//
// Inputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of trig mesh indices into V
//   num_iters  number of iterations
// Outputs:
//   SV  #SV by 3 list of vertex positions
//   SF  #SF by 3 list of trig mesh indices into SV
//
void loop_subdivision(
    const std::vector<std::vector<double>>& V,
    const std::vector<std::vector<int>>& F,
    const int num_iters,
    std::vector<std::vector<double>>& SV,
    std::vector<std::vector<int>>& SF);
#endif
