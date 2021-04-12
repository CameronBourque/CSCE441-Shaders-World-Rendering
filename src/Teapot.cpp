#include "Teapot.h"

Teapot::Teapot(std::shared_ptr<Shape> &shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
               glm::vec3 ks) :
               Object(shape, translation, angles, scale, kd, ks)
{
}

Teapot::~Teapot()
{
}

void Teapot::transform(std::shared_ptr<MatrixStack> &MV)
{
    // Need to adjust for the object's min y value if it's grounded
    float minY = shape->getPosBuf()[1];
    for (size_t i = 1; i < shape->getPosBuf().size(); i += 3) {
        if (shape->getPosBuf()[i] < minY) {
            minY = shape->getPosBuf()[i];
        }
    }

    // Fix min y to scale of object
    minY = minY * scale.y;

    // Get time
    double t = glfwGetTime();

    // Set shear
    glm::mat4 S(1.0f);
    S[1][2] = (float)cos(t) * 0.5f;

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y - minY, translation.z));
    MV->multMatrix(S);
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
}