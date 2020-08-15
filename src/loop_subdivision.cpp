#include "loop_subdivision.h"

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T> 
std::vector<T> add(const std::vector<T>& V1, const std::vector<T>& V2) {
  assert(V1.size() == V2.size());
  std::vector<T> V3(V1.size());
  for (int i = 0; i < V1.size(); i++) {
    V3[i] = V1[i] + V2[i];
  }
  return V3;
}

template <typename T> 
std::vector<T> times(const std::vector<T>& V1, const double num) {
  std::vector<T> V(V1.size());
  for (int i = 0; i < V1.size(); i++) {
    V[i] = V1[i] * num;
  }
  return V;
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

void loop_subdivision(
    const std::vector<std::vector<double>>& V,
    const std::vector<std::vector<int>>& F,
    const int num_iters,
    std::vector<std::vector<double>>& SV,
    std::vector<std::vector<int>>& SF) {
  ////////////////////////////////////////////////////////////////////////////
  if (num_iters <= 0 || V.size() == 0 || F.size() == 0 || V[0].size() == 0 || F[0].size() == 0) {
    SV = V;
    SF = F;
    return;
  }

  std::cout << "\n\n\nV\n";
  std::cout << "size:" << V.size() << "\n";
  for (auto obj : V) {
    for (auto oo: obj) {
      std::cout << oo << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  std::cout << "\n\n\nF\n";
  std::cout << "size:" << F.size() << "\n";
  for (auto obj : F) {
    for (auto oo: obj) {
      std::cout << oo << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  const int V_rows = V.size();
  const int F_rows = F.size();

  // build edge to (one or two) faces
  std::unordered_map<std::pair<int, int>, std::vector<int>, hash_pair> edge_to_faces;
  int current_vertex_index, next_vertex_index, previous_vertex_index;
  std::pair<int, int> pair;

  for (int i = 0; i < F_rows; i++) {
    for (int j = 0; j < 3; j++) {
      current_vertex_index = F[i][j];
      next_vertex_index = F[i][(j + 1) % 3];
      pair = (current_vertex_index <= next_vertex_index) ? std::make_pair(current_vertex_index, next_vertex_index) : std::make_pair(next_vertex_index, current_vertex_index);
      if (edge_to_faces.find(pair) == edge_to_faces.end()) {
        std::vector<int> entry;
        edge_to_faces[pair] = entry;
      }
      edge_to_faces[pair].push_back(i);
    }
  }

  SV.resize(V_rows + edge_to_faces.size());
  SF.resize(4 * F_rows);

  // build vertex to neightboring vertices
  std::unordered_map<int, std::vector<int>> vertex_to_vertices;
  int a, b;

  for (auto entry : edge_to_faces) {
    a = entry.first.first;
    b = entry.first.second;

    // if vertex
    if (vertex_to_vertices.find(a) == vertex_to_vertices.end()) {
      std::vector<int> entry_vec;
      vertex_to_vertices[a] = entry_vec;
    }
    vertex_to_vertices[a].push_back(b);

    if (vertex_to_vertices.find(b) == vertex_to_vertices.end()) {
      std::vector<int> entry_vec;
      vertex_to_vertices[b] = entry_vec;
    }
    vertex_to_vertices[b].push_back(a);
  }

  std::vector<int> faces;
  std::vector<double> position(3), A(3), B(3), C(3), D(3);
  std::vector<int> neighbour_vertices;
  int size;
  float BETA;
  // Add (modified) old vertices into SV
  int SV_index = 0;
  for (int i = 0; i < V_rows; i++) {
    neighbour_vertices = vertex_to_vertices[i];
    size = neighbour_vertices.size();
    if (size == 2) {
      // Boundary vertex
      A = V[neighbour_vertices[0]];
      B = V[neighbour_vertices[1]];
      position = add(times(add(A, B), 1.0 / 8.0), times(V[i], 3.0 / 4.0));
    } else {
      // Interior vertex
      BETA = 1.0 / size * (5.0 / 8.0 - (3.0 / 8.0 + 1.0 / 4.0 * cos(2.0 * M_PI / size)) * (3.0 / 8.0 + 1.0 / 4.0 * cos(2.0 * M_PI / size)));
      position = times(V[i], 1 - size * BETA);
      for (int w = 0; w < size; w++) {
        position = add(position, times(V[neighbour_vertices[w]], BETA));
      }
    }
    SV[SV_index++] = position;
  }

  // Add new (edge) points to SV and populate edge_to_SV
  std::unordered_map<std::pair<int, int>, int, hash_pair> edge_to_SV;  // Key represents an edge. Value Represents the row number of the edge point in SV.
  for (auto entry : edge_to_faces) {
    pair = entry.first;
    faces = entry.second;
    if (faces.size() == 2) {
      A = V[pair.first];
      B = V[pair.second];
      for (int j = 0; j < 3; j++) {
        if (F[faces[0]][j] != pair.first && F[faces[0]][j] != pair.second) {
          C = V[F[faces[0]][j]];
          break;
        }
      }
      for (int j = 0; j < 3; j++) {
        if (F[faces[1]][j] != pair.first && F[faces[1]][j] != pair.second) {
          D = V[F[faces[0]][j]];
          break;
        }
      }
      position = add(times(add(A, B), 3.0 / 8.0), times(add(C, D), 1.0 / 8.0));
    } else if (faces.size() == 1) {
      A = V[pair.first];
      B = V[pair.second];
      position = times(add(A, B), 1.0 / 2.0);
    } else {
      std::cerr << "Edge connects to " << faces.size() << " faces\n";
    }
    SV[SV_index] = position;
    edge_to_SV[pair] = SV_index;
    SV_index++;
  }
  // Populate SF
  for (int i = 0; i < F_rows; i++) {
    for (int j = 0; j < 3; j++) {
      SF[i * 4 + j].resize(3);
      SF[i * 4 + 3].resize(3);
      SF[i * 4 + j][0] = F[i][j];
      pair = (F[i][j] <= F[i][(j + 1) % 3]) ? std::make_pair(F[i][j], F[i][(j + 1) % 3]) : std::make_pair(F[i][(j + 1) % 3], F[i][j]);
      SF[i * 4 + j][1] = edge_to_SV[pair];
      SF[i * 4 + 3][j] = SF[i * 4 + j][1];
      pair = (F[i][j] <= F[i][(j + 2) % 3]) ? std::make_pair(F[i][j], F[i][(j + 2) % 3]) : std::make_pair(F[i][(j + 2) % 3], F[i][j]);
      SF[i * 4 + j][2] = edge_to_SV[pair];
    }
  }
  std::vector<std::vector<double>> SV_copy = SV;
  std::vector<std::vector<int>> SF_copy = SF;

  std::cout << "\n\n\nSV\n";
  std::cout << "size:" << SV.size() << "\n";
  for (auto obj : SV) {
    for (auto oo: obj) {
      std::cout << oo << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";


  std::cout << "\n\n\nSF\n";
  std::cout << "size:" << SF.size() << "\n";
  for (auto obj : SF) {
    for (auto oo: obj) {
      std::cout << oo << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  loop_subdivision(SV_copy, SF_copy, num_iters - 1, SV, SF);
  ////////////////////////////////////////////////////////////////////////////
}
