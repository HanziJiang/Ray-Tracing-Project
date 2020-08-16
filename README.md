# Computer Graphics Final Project
## New Tasks
### Refraction
Refraction exits in everyday life, that's why I added it to my ray tracing project. It is implemented using [this method](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel). Here is my implementation:
First I add `kt` (transparency index) and `ior` (index of refraction) attributes to the Material class. In `raycolor.cpp`, I determine if the hit object is transmissive based on `kt`. If no, the code is the same as the original, we have Blinn-Phong and reflection. If yes, we also have refraction. How much light is refracted vs reflected is determined by `fresnel`. The direction of refrated light is determined by `refract`. I assume the `ior` of air is 1.0. 
In `refract`, [Snell's Law](https://en.wikipedia.org/wiki/Snell%27s_law) seems simple, but there are a few things we need to pay attention to. First, before taking the ratio of the two iors, we need to know if the ray is hiting the object from outside or inside. This can be done by checking the sign of the dot product of the ray direction and normal vector. If outside, we need to invert the sign of the normal vector. We also need to check when total internal reflection occurs. That is, when the angle is greater than the critical angle, the ray is 100% reflected.
![Alt Text](https://github.com/HanziJiang/ray-trace/blob/master/images/refract.png)

### Fresnel Effect
The precentage of light reflected and refracted is determined by the angle of incidence. It can be calculated using [Fresnel's Equations](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel). We must also deal with total internal reflection when calculating.

### Charles-Loop Subdivision
Since the Catmull-Clark subdivision in A5 only works for quad meshes, I implemented the Charles-Loop subdivision for triangular meshes. This subdivision scheme can be found in [this paper](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/thesis-10.pdf). The output looks quite smooth.
The subdivision rules are based on properties of B-spline and many other multivariate spline theories. Subdivision surfaces are defined recursively, and each triangle is divided into four smaller ones. The procedure can be summarized into four phases:
1. Build Adjacency Data Structure

   I mapped each edge (represented by a `std::pair`) to its adjacent faces. Then I mapped each vertex to its neighbouring vertices. 
2. Compute Edge Vertices

   A triangle has three origional vertices and three edge vertices. If an edge is not at crease or boundary, the edge point is a linear combination of the four vertices of the two faces adjacent to this edge. Otherwise, the edge point is only a linear combination of the two vertices connected by this edge. The weights are fixed.
3. Recompute Original Vertices

   Each original vertex is recomputed as a linear combination of the vertex itself, and all its neighbouring vertices. The weights are determined by a value \beta, which is determined by the number of neighbouring vertices.
4. Rebuild Mesh

   First recomputed original vertices are added in their original order. Then edge vertices are added in arbitrary order. At the same time, a map is built. The key is an edge, value the row number of the new vertices matrix that the edge vertex corresponds to. Finally, I populated the new faces matrix.


In `read_json.h`, I subdivide the meshes before writing them into the TriangleSoup for a smoother sppearance.

![Alt Text](https://github.com/HanziJiang/ray-trace/blob/master/images/comparison.gif)
![Alt Text](https://github.com/HanziJiang/ray-trace/blob/master/images/comparison_video.gif)
Charles-Loop subdivision and Catmull-Clark subdivision, 5 iterations. 


Some other outputs:
![Alt Text](https://github.com/HanziJiang/ray-trace/blob/master/images/icosahedron.gif)
Icosahedron, 5 iterations.


![Alt Text](https://github.com/HanziJiang/ray-trace/blob/master/images/cow.gif)
Cow, 2 iterations.
To reproduce the above images, checkout my [loop_subdivision project](https://github.com/HanziJiang/loop_subdivision).

### Read both .obj and .stl
Since some of my favourite meshes are in .obj, some in .stl, I make my project take both file formats as inputs. This is done by checking the file extension in `read_json.h`. `readOBJ.cpp` and relating files are imported from other projects.


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
