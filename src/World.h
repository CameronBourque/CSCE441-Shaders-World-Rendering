#ifndef _WORLD_H_
#define _WORLD_H_

#include <memory>
#include <vector>
#include <string>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "Camera.h"
#include "Program.h"
#include "Light.h"
#include "Bunny.h"
#include "Teapot.h"
#include "Ball.h"
#include "BallShape.h"
#include "Surface.h"
#include "SurfaceShape.h"

class World
{
public:
    World(std::string resDir);

    ~World();

    void draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t);

private:
    inline float getRandom() { return randDist(randEngine); }
    std::mt19937 randEngine;
    std::uniform_real_distribution<float> randDist;

    void bindLights(std::shared_ptr<MatrixStack>& MV);

    std::shared_ptr<Object> ground;
    std::vector<std::shared_ptr<Object>> objs;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Program> prog;
    std::shared_ptr<Program> surfaceProg;
};

#endif