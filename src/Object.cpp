#include "Object.h"

Object::Object(std::shared_ptr<Shape> &_shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale,
               glm::vec3 _ke) :
               shape(_shape),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               ke(_ke),
               kd(0.0f),
               ks(0.0f),
               s(1.0f)
{
}

Object::Object(std::shared_ptr<Shape> &_shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale,
               glm::vec3 _kd, glm::vec3 _ks) :
               shape(_shape),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               ke(0.0f),
               kd(_kd),
               ks(_ks),
               s(10.0f)
{
}

Object::~Object()
{}

void Object::transform(std::shared_ptr<MatrixStack>& MV, bool grounded)
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
    // Fix min y to scale of object
    minY = minY * scale.y;

    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y - minY, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
}

void Object::bind(std::shared_ptr<Program>& prog) const
{
    // This will set the color for the object
    glUniform3f(prog->getUniform("ke"), ke.r, ke.g, ke.b);
    glUniform3f(prog->getUniform("kd"), kd.r, kd.g, kd.b);
    glUniform3f(prog->getUniform("ks"), ks.r, ks.g, ks.b);
    glUniform1f(prog->getUniform("s"), s);
}

void Object::bindTransform(std::shared_ptr<Program> &prog, std::shared_ptr<MatrixStack> &MV)
{
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("ITMV"), 1, GL_FALSE,
                       glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));
}
