#include "Light.h"

Light::Light(glm::vec3 _color, glm::vec3 _lightPos) :
    color(_color), lightPos(_lightPos), increment(0.1)
{}

Light::~Light()
{}

void Light::moveXPos(bool decrement)
{
    if(decrement)
    {
        lightPos.x -= increment;
    }
    else
    {
        lightPos.x += increment;
    }
}

void Light::moveYPos(bool decrement)
{
    if(decrement)
    {
        lightPos.y -= increment;
    }
    else
    {
        lightPos.y += increment;
    }
}
