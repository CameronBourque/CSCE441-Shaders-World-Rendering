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

class ShaderManager
{
public:
    ShaderManager(std::string resDir);
    ~ShaderManager();

    void changeProgram(bool decrement = false);
    void changeMaterial(bool decrement = false);
    void bind(std::shared_ptr<MatrixStack>& P);
    void unbind();
    void draw(std::shared_ptr<Shape>& bunny, std::shared_ptr<MatrixStack>& MV);

private:
    std::vector<std::shared_ptr<Program>> programs;
    unsigned int programSelection;

    std::vector<std::shared_ptr<Material>> materials;
    unsigned int materialSelection;

    glm::vec3 lightPos; // TODO: replace with lights class
};

#endif