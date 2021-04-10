#include "Light.h"

Light::Light(std::shared_ptr<Shape>& shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 ke,
             float s) : Object(shape, translation, angles, scale, ke, s)
{
}

Light::~Light()
{
}
