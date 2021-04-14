#ifndef _BUNNY_H_
#define _BUNNY_H_

#include "Object.h"

class Bunny : public Object
{
public:
    Bunny(std::shared_ptr<Shape>& shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
          glm::vec3 ks, float offset, float offsetScale);
    ~Bunny();

    void transform(std::shared_ptr<MatrixStack>& MV);
};

#endif
