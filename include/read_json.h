#ifndef READ_JSON_H
#define READ_JSON_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
// Forward declaration
struct Object;
struct Light;

// Get extention of file file_name
std::string extension(std::string file_name) {
  int position = file_name.find_last_of(".");
  return file_name.substr(position + 1);
}

// Read a scene description from a .json file
//
// Input:
//   filename  path to .json file
// Output:
//   camera  camera looking at the scene
//   objects  list of shared pointers to objects
//   lights  list of shared pointers to lights
inline bool read_json(
    const std::string& filename,
    Camera& camera,
    std::vector<std::shared_ptr<Object> >& objects,
    std::vector<std::shared_ptr<Light> >& lights);

// Implementation

#include <Eigen/Geometry>
#include <cassert>
#include <fstream>
#include <iostream>
#include <json.hpp>

#include "DirectionalLight.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Plane.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleSoup.h"
#include "dirname.h"
#include "loop_subdivision.h"
#include "readOBJ.h"
#include "readSTL.h"

inline bool read_json(
    const std::string& filename,
    Camera& camera,
    std::vector<std::shared_ptr<Object> >& objects,
    std::vector<std::shared_ptr<Light> >& lights) {
  // Heavily borrowing from
  // https://github.com/yig/graphics101-raycasting/blob/master/parser.cpp
  using json = nlohmann::json;

  std::ifstream infile(filename);
  if (!infile) return false;
  json j;
  infile >> j;

  // parse a vector
  auto parse_Vector3d = [](const json& j) -> Eigen::Vector3d {
    return Eigen::Vector3d(j[0], j[1], j[2]);
  };
  // parse camera
  auto parse_camera =
      [&parse_Vector3d](const json& j, Camera& camera) {
        assert(j["type"] == "perspective" && "Only handling perspective cameras");
        camera.d = j["focal_length"].get<double>();
        camera.e = parse_Vector3d(j["eye"]);
        camera.v = parse_Vector3d(j["up"]).normalized();
        camera.w = -parse_Vector3d(j["look"]).normalized();
        camera.u = camera.v.cross(camera.w);
        camera.height = j["height"].get<double>();
        camera.width = j["width"].get<double>();
      };
  parse_camera(j["camera"], camera);

  // Parse materials
  std::unordered_map<std::string, std::shared_ptr<Material> > materials;
  auto parse_materials = [&parse_Vector3d](
                             const json& j,
                             std::unordered_map<std::string, std::shared_ptr<Material> >& materials) {
    materials.clear();
    for (const json& jmat : j) {
      std::string name = jmat["name"];
      std::shared_ptr<Material> material(new Material());
      material->ka = parse_Vector3d(jmat["ka"]);
      material->kd = parse_Vector3d(jmat["kd"]);
      material->ks = parse_Vector3d(jmat["ks"]);
      material->km = parse_Vector3d(jmat["km"]);
      material->phong_exponent = jmat["phong_exponent"];
      material->kt = jmat["kt"];
      materials[name] = material;
    }
  };
  parse_materials(j["materials"], materials);

  auto parse_lights = [&parse_Vector3d](
                          const json& j,
                          std::vector<std::shared_ptr<Light> >& lights) {
    lights.clear();
    for (const json& jlight : j) {
      if (jlight["type"] == "directional") {
        std::shared_ptr<DirectionalLight> light(new DirectionalLight());
        light->d = parse_Vector3d(jlight["direction"]).normalized();
        light->I = parse_Vector3d(jlight["color"]);
        lights.push_back(light);
      } else if (jlight["type"] == "point") {
        std::shared_ptr<PointLight> light(new PointLight());
        light->p = parse_Vector3d(jlight["position"]);
        light->I = parse_Vector3d(jlight["color"]);
        lights.push_back(light);
      }
    }
  };
  parse_lights(j["lights"], lights);

  auto parse_objects = [&parse_Vector3d, &filename, &materials](
                           const json& j,
                           std::vector<std::shared_ptr<Object> >& objects) {
    objects.clear();
    for (const json& jobj : j) {
      if (jobj["type"] == "sphere") {
        std::shared_ptr<Sphere> sphere(new Sphere());
        sphere->center = parse_Vector3d(jobj["center"]);
        sphere->radius = jobj["radius"].get<double>();
        objects.push_back(sphere);
      } else if (jobj["type"] == "plane") {
        std::shared_ptr<Plane> plane(new Plane());
        plane->point = parse_Vector3d(jobj["point"]);
        plane->normal = parse_Vector3d(jobj["normal"]).normalized();
        objects.push_back(plane);
      } else if (jobj["type"] == "triangle") {
        std::shared_ptr<Triangle> tri(new Triangle());
        tri->corners = std::make_tuple(
            parse_Vector3d(jobj["corners"][0]),
            parse_Vector3d(jobj["corners"][1]),
            parse_Vector3d(jobj["corners"][2]));
        objects.push_back(tri);
      } else if (jobj["type"] == "soup") {
#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR std::string("\\")
#else
#define PATH_SEPARATOR std::string("/")
#endif
        const std::string mesh_path = jobj["mesh"];

        if (extension(mesh_path) == "stl") {
          std::vector<std::vector<double> > V;
          std::vector<std::vector<int> > F;
          std::vector<std::vector<int> > N;
          {
            igl::readSTL(
                igl::dirname(filename) +
                    PATH_SEPARATOR +
                    mesh_path,
                V, F, N);
          }
          std::shared_ptr<TriangleSoup> soup(new TriangleSoup());
          for (int f = 0; f < F.size(); f++) {
            std::shared_ptr<Triangle> tri(new Triangle());
            tri->corners = std::make_tuple(
                Eigen::Vector3d(V[F[f][0]][0], V[F[f][0]][1], V[F[f][0]][2]),
                Eigen::Vector3d(V[F[f][1]][0], V[F[f][1]][1], V[F[f][1]][2]),
                Eigen::Vector3d(V[F[f][2]][0], V[F[f][2]][1], V[F[f][2]][2]));
            soup->triangles.push_back(tri);
          }
          objects.push_back(soup);
        } else {
          Eigen::MatrixXd V_obj, SV_obj;
          Eigen::MatrixXi F_obj, SF_obj;
          {
            igl::readOBJ(igl::dirname(filename) +
                             PATH_SEPARATOR +
                             mesh_path,
                         V_obj, F_obj);
          }
          loop_subdivision(V_obj, F_obj, 2, SV_obj, SF_obj);
          std::shared_ptr<TriangleSoup> soup(new TriangleSoup());
          for (int f = 0; f < F_obj.rows(); f++) {
            std::shared_ptr<Triangle> tri(new Triangle());
            tri->corners = std::make_tuple(
                Eigen::Vector3d(SV_obj(SF_obj(f, 0), 0), SV_obj(SF_obj(f, 0), 1), SV_obj(SF_obj(f, 0), 2)),
                Eigen::Vector3d(SV_obj(SF_obj(f, 1), 0), SV_obj(SF_obj(f, 1), 1), SV_obj(SF_obj(f, 1), 2)),
                Eigen::Vector3d(SV_obj(SF_obj(f, 2), 0), SV_obj(SF_obj(f, 2), 1), SV_obj(SF_obj(f, 2), 2)));
            soup->triangles.push_back(tri);
          }
          objects.push_back(soup);
        }
      }
      //objects.back()->material = default_material;
      if (jobj.count("material")) {
        if (materials.count(jobj["material"])) {
          objects.back()->material = materials[jobj["material"]];
        }
      }
    }
  };
  parse_objects(j["objects"], objects);
  
  return true;
}

#endif
