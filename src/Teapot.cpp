#include "Teapot.h"

Teapot::Teapot(std::shared_ptr<Shape> &shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
               glm::vec3 ks, float offset, float offsetScale) :
               Object(shape, translation, angles, scale, kd, ks, offset, offsetScale)
{
}

Teapot::~Teapot()
{
}

void Teapot::transform(std::shared_ptr<MatrixStack> &MV)
{
    // Need to adjust for the object's min y value if it's grounded
    float minY = shape->getMinYPos();

    // Fix min y to scale of object
    minY = minY * scale.y;

    // Get time
    double t = glfwGetTime();

    // Set shear
    glm::mat4 S(1.0f);
    S[1][2] = (float)cos((t * offsetScale) + offset) * 0.5f;

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y - minY, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
    MV->multMatrix(S);
}