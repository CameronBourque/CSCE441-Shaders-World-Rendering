#ifndef _WORLD_H_
#define _WORLD_H_

#include <memory>
#include <vector>
#include <string>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "Program.h"

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

    std::vector<std::shared_ptr<Object>> objs;
};

#endif