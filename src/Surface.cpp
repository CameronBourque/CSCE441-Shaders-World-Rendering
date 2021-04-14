#include "Surface.h"

Surface::Surface(std::shared_ptr<Shape> shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
                 glm::vec3 ks, float offset, float offsetScale) :
                 Object(shape, translation, angles, scale, kd, ks, offset, offsetScale)
{
}

Surface::~Surface()
{
}

void Surface::transform(std::shared_ptr<MatrixStack> &MV)
{
    // Need to adjust for the object's min y value if it's grounded
    float maxX = shape->getMaxXPos();

    // Fix min y to scale of object
    maxX = maxX * scale.y;

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y + maxX, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
}

#include <iostream>
void Surface::bind(std::shared_ptr<Program> &prog, float t) const
{
    // Bind just like a normal object
    Object::bind(prog);

    // Need to also bind the animation
    float time = (glfwGetTime() * offsetScale) - offset;
//    std::cout << time << std::endl;
    glUniform1f(prog->getUniform("t"), time);
}
