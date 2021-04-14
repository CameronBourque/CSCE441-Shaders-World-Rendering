#ifndef _TEAPOT_H_
#define _TEAPOT_H_

#include "Object.h"

class Teapot : public Object
{
public:
    Teapot(std::shared_ptr<Shape>& shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
           glm::vec3 ks, float offset, float offsetScale);
    ~Teapot();

    void transform(std::shared_ptr<MatrixStack>& MV);
};

#endif
