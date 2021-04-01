#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.h"
#include "MatrixStack.h"

class Object
{
public:
    Object(std::shared_ptr<Shape> _shape, glm::vec3 _translation,
           glm::vec3 _angles, glm::vec3 _scale, glm::vec3 _ka,
           glm::vec3 _kd, glm::vec3 _ks, float _s);
    ~Object();

    void transform(std::shared_ptr<MatrixStack>& MV, bool grounded);

    std::shared_ptr<Shape> getShape() { return shape; }
    glm::vec3 getTranslation() { return translation; }

protected:
    std::shared_ptr<Shape> shape;

    glm::vec3 translation;
    glm::vec3 angles;
    glm::vec3 scale;

    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float s;

};

#endif