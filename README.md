# Computer Graphics Final Project
## New Tasks
#### Refraction
Refraction is implemented adopting [this](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel) method.
#### Fresnel effect
Use Fresnel's Equations defines how much light is reflected and transmitted for transparent materials. The method can be found [here](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel).

## Task in Progress
It would be great to use BVH to accelerate the ray tracing progress. However, having to prepare for other course projects and exams, I was unable to finish the task. I have commented out BVH related code. My attempt was make `blinn_phong_shading` and `first_hit` take an AABBTree and descendant as arguments instead of a list of objects and hit_id. Each object had an box attribute. Related functions such as `insert_triangle_into_box` were implemented but commented out. Each object in the scene file was an AABB tree, together they formed a large AABB tree.

## Running the Project

> ```
> mkdir build-release
> cd build-release
> cmake ../ -DCMAKE_BUILD_TYPE=Release
> make
> ./raytracing <json_scene_file_name>
> ```
For example:
> ```
> ./raytracing the_catch
> ```
There are three scenes: x, x and x.
**Output**: The output .ppm files will be in the build-release folder and has the same name as the input json file.
