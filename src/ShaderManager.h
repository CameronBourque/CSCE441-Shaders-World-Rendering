#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <vector>
#include <string>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Program.h"
#include "Material.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Light.h"
#include "Object.h"

class ShaderManager
{
public:
    ShaderManager(std::string resDir);
    ~ShaderManager();

    void changeProgram(bool decrement = false);
    void changeLight(bool decrement = false);

    void addLight(std::shared_ptr<Light> light);

    void bind(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV);
    void unbind();
    void draw(std::shared_ptr<Object>& shape, std::shared_ptr<MatrixStack>& MV, double t = 0, bool grounded = false);

private:
    std::vector<std::shared_ptr<Program>> programs;
    unsigned int programSelection;

    std::vector<std::shared_ptr<Light>> lights;
    unsigned int lightSelection;
};

#endif