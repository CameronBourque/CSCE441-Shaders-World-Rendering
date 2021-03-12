#ifndef _LIGHT_H_
#define _LIGHT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
    Light(glm::vec3 _color, glm::vec3 _lightPos);
    ~Light();

    glm::vec3 getColor() { return color; }
    glm::vec3 getPosition() { return lightPos; }

    void moveXPos(bool decrement);
    void moveYPos(bool decrement);

private:
    glm::vec3 color;
    glm::vec3 lightPos;

    float increment;
};

#endif
