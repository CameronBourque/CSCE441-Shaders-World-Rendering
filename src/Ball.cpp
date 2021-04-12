#include "Ball.h"

Ball::Ball(glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd, glm::vec3 ks) :
           Object(translation, angles, scale, kd, ks)
{

}

Ball::~Ball()
{
}