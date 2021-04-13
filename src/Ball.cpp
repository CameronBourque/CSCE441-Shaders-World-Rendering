#include "Ball.h"

Ball::Ball(std::shared_ptr<Shape> shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
           glm::vec3 ks) :
           Object(shape, translation, angles, scale, kd, ks)
{
}

Ball::~Ball()
{
}

void Ball::transform(std::shared_ptr<MatrixStack> &MV)
{
    // Need to adjust for the object's min y value if it's grounded
    float minY = shape->getMinYPos();

    // Fix min y to scale of object
    minY = minY * scale.y;

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y - minY, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);

    // Constants
    float t = glfwGetTime();
    float yFactor = 1.3f;
    float sFactor = 0.5f;
    float p = 1.7f;
    float to = 0.9f;
    float half = 0.5f;

    // Calculate translation and scale
    float y = yFactor * (half * sin(2 * M_PI / p * (t + to) + half)) + yFactor * half;
    float s = -sFactor * (half * cos(4 * M_PI / p * (t + to)) + half) + 1;

    // More transforms based on time
    MV->translate(0.0f, y, 0.0f);
    MV->scale(s, 1.0f, s);
}
