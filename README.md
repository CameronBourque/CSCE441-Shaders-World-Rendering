# Free-Look World
Presents the user in a 3D world that is navigable. Camera has freelook. A HUD appears. Minimap is toggleable. Uses Blinn-Phong lighting.

# Build
mkdir build
cd build
cmake ../src
make -j

# Run
./A4 ../resources

# Controls
Mouse - controls the camera angle \
W/A/S/D - controls movement of the user relative to the direction they are facing (ignoring the pitch of the camera) \
z/Z - changes the zoom (FOV) \
t - toggles the minimap
