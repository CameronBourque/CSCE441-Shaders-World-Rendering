#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.h"
#include "MatrixStack.h"
#include "Program.h"

class Object
{
public:
    Object(std::shared_ptr<Shape>& _shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _ke,
           float _offset, float _offsetScale);
    Object(std::shared_ptr<Shape>& _shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _kd,
           glm::vec3 _ks);
    Object(std::shared_ptr<Shape>& _shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _kd,
           glm::vec3 _ks, float _offset, float _offsetScale);
    ~Object();

    virtual void transform(std::shared_ptr<MatrixStack>& MV);
    virtual void draw(std::shared_ptr<Program>& prog);
    virtual void bind(std::shared_ptr<Program>& prog, float t = 0.0f) const;
    static void bindTransform(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& MV);

    std::shared_ptr<Shape> getShape() { return shape; }
    glm::vec3 getTranslation() { return translation; }

    virtual bool needsNewProgram() { return false; }

protected:
    std::shared_ptr<Shape> shape;

    glm::vec3 translation;
    glm::vec3 angles;
    glm::vec3 scale;

    glm::vec3 ke;
    glm::vec3 kd;
    glm::vec3 ks;
    float s;

    float offset;
    float offsetScale;

};

#endif