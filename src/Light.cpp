#include "Light.h"

Light::Light(std::shared_ptr<Shape>& shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 ke,
             float offset, float offsetScale) :
             Object(shape, translation, angles, scale, ke, offset, offsetScale),
             t(0.0f)
{
}

Light::~Light()
{
}

void Light::transform(std::shared_ptr<MatrixStack> &MV)
{
    // Need to adjust for the object's min y value if it's grounded
    float minY = shape->getMinYPos();

    // Fix min y to scale of object
    minY = minY * scale.y;

    // Get time
    t = (float)glfwGetTime() * offsetScale + offset;

    // Transform the shape
    MV->rotate(angles.y + t, 0.0, 1.0, 0.0);
    MV->translate(glm::vec3(translation.x, translation.y - minY, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);

    // Constants
    float yFactor = 3.3f;
    float p = 1.7f;
    float to = 0.9f;
    float half = 0.5f;

    // Calculate translation and scale
    float y = yFactor * (half * sin(2 * M_PI / p * (t + to) + half)) + yFactor * half;

    // More transforms based on time
    MV->translate(0.0f, y, 0.0f);
}

glm::vec3 Light::getPosition(std::shared_ptr<MatrixStack> MV)
{
    MV->pushMatrix();
    transform(MV);
    glm::vec4 position = MV->topMatrix() * glm::vec4(0.0, 0.0, 0.0, 1.0);
    MV->popMatrix();
    return glm::vec3(position);
}
