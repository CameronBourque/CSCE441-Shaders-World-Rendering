#include "Object.h"

Object::Object(std::shared_ptr<Shape> _shape, std::shared_ptr<Material> _material, glm::vec3 _translation,
               glm::vec3 _angles, glm::vec3 _scale, glm::mat4 _shear, bool _doShear) :
               shape(_shape),
               material(_material),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               shear(_shear),
               doShear(_doShear),
               growth(0.0f),
               growthLimiter(1.0f)
{}

Object::~Object()
{}

void Object::transform(std::shared_ptr<MatrixStack> &MV, bool grounded)
{
    // Need to adjust for the object's min y value if it's grounded
    float minY = 0;
    if(grounded)
    {
        minY = shape->getPosBuf()[1];
        for (size_t i = 1; i < shape->getPosBuf().size(); i += 3) {
            if (shape->getPosBuf()[i] < minY) {
                minY = shape->getPosBuf()[i];
            }
        }
    }

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y - (minY * (scale.y + (growth * growthLimiter))), translation.z));
    if(doShear)
    {
        MV->multMatrix(shear);
    }
    MV->scale(scale + (growth * growthLimiter));
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
}
