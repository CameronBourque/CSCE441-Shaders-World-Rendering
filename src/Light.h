#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Object.h"

class Light : public Object
{
public:
    Light(std::shared_ptr<Shape>& shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 ke,
          float offset, float offsetScale);
    ~Light();

    void transform(std::shared_ptr<MatrixStack>& MV);

    glm::vec3 getKE() { return ke; }
    glm::vec3 getPosition(std::shared_ptr<MatrixStack> MV);

private:
    float t;
};

#endif