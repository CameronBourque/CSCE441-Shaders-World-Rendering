#include "Surface.h"

Surface::Surface(std::shared_ptr<Shape> shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
                 glm::vec3 ks) :
                 Object(shape, translation, angles, scale, kd, ks)
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
