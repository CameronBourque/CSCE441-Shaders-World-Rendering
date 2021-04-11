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
    enum ObjectShape {
        BUNNY = 0,
        TEAPOT,
        BOUNCE_SPHERE,
        SURFACE_OF_REVOLUTION,
        OTHER
    };

    Object(std::shared_ptr<Shape>& _shape, glm::vec3 _translation,
           glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _ke);
    Object(std::shared_ptr<Shape>& _shape, glm::vec3 _translation,
           glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _kd, glm::vec3 _ks,
           ObjectShape _objectShape);
    ~Object();

    virtual void bind(std::shared_ptr<Program>& prog) const;
    virtual void transform(std::shared_ptr<MatrixStack>& MV);
    static void bindTransform(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& MV);

    std::shared_ptr<Shape> getShape() { return shape; }
    glm::vec3 getTranslation() { return translation; }

protected:
    std::shared_ptr<Shape> shape;
    ObjectShape objectShape;

    glm::vec3 translation;
    glm::vec3 angles;
    glm::vec3 scale;

    glm::vec3 ke;
    glm::vec3 kd;
    glm::vec3 ks;
    float s;

};

#endif