# Dynamic Geometry and Deferred Rendering
Presents a world of attenuated lights, dynamic 3D transforms, static surface of revolution on the CPU, dynamic surface of revolution on the GPU, and deferred rendering.

# Build
mkdir build
cd build
cmake ../src
make -j

# Run
./A5 ../resources

# Controls
Mouse click and drag - move the camera around the center

# How to generate different textures for deferred rendering
**In pass2_frag.glsl:** \
Line 56 is how the world is intended to look  
Uncomment line 57 for camera-space position of fragments  
Uncomment line 58 for camera-space normal of fragments  
Uncomment line 59 for emissive color of all the fragments  
Uncomment line 60 for diffuse color of all the fragments  
