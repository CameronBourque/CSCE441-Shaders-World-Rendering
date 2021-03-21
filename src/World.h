#ifndef _WORLD_H_
#define _WORLD_H_

#include <memory>
#include <vector>
#include <string>
#include <random>

#include "Object.h"
#include "ShaderManager.h"
#include "Light.h"
#include "Camera.h"

class World
{
public:
    World(std::string resDir);

    ~World();

    void draw(std::shared_ptr<Camera> camera, double t);

private:
    inline float getRandom() { return randDist(randEngine); }
    std::mt19937 randEngine;
    std::uniform_real_distribution<float> randDist;

    std::vector<std::shared_ptr<Object>> objs;
    std::shared_ptr<Object> sun;
    std::shared_ptr<Object> ground;
    std::shared_ptr<ShaderManager> shaderManager;
    std::shared_ptr<Light> sunLight;
};

#endif