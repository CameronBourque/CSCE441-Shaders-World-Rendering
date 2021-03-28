#ifndef _WORLD_H_
#define _WORLD_H_

#include <memory>
#include <vector>
#include <string>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Object.h"
#include "ShaderManager.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureObject.h"
#include "Program.h"

class World
{
public:
    World(std::string resDir);

    ~World();

    void draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t);
    void drawHUD(std::shared_ptr<MatrixStack>& P, double t);
    void drawTopDown(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, glm::mat4 camMatrix, double t,
                     float a, float fov);

private:
    inline float getRandom() { return randDist(randEngine); }
    std::mt19937 randEngine;
    std::uniform_real_distribution<float> randDist;

    std::shared_ptr<Program> textureProgram;
    std::shared_ptr<TextureObject> texture;
    std::vector<std::shared_ptr<Object>> objs;
    std::vector<std::shared_ptr<Object>> hud;
    std::shared_ptr<Object> sun;
    std::shared_ptr<Object> ground;
    std::shared_ptr<Object> frustum;
    std::shared_ptr<ShaderManager> shaderManager;
    std::shared_ptr<Light> sunLight;
};

#endif