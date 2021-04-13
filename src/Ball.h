#ifndef _BALL_H_
#define _BALL_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Object.h"

class Ball : public Object
{
public:
    Ball(std::shared_ptr<Shape> shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
         glm::vec3 ks);
    ~Ball();

    void transform(std::shared_ptr<MatrixStack>& MV);
};

#endif