#include "Object.h"

Object::Object(std::shared_ptr<Shape> &_shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale,
               glm::vec3 _ke, float _offset, float _offsetScale) :
               shape(_shape),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               ke(_ke),
               kd(0.0f),
               ks(0.0f),
               s(1.0f),
               offset(_offset),
               offsetScale(_offsetScale)
{
}

Object::Object(std::shared_ptr<Shape> &_shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale,
               glm::vec3 _ke) :
               shape(_shape),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               ke(_ke),
               kd(0.0f),
               ks(0.0f),
               s(1.0f),
               offset(0.0f),
               offsetScale(1.0f)
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
               s(10.0f),
               offset(0.0f),
               offsetScale(1.0f)
{
}

Object::Object(std::shared_ptr<Shape> &_shape, glm::vec3 _translation, glm::vec3 _angles, glm::vec3 _scale,
               glm::vec3 _kd, glm::vec3 _ks, float _offset, float _offsetScale) :
               shape(_shape),
               translation(_translation),
               angles(_angles),
               scale(_scale),
               ke(0.0f),
               kd(_kd),
               ks(_ks),
               s(10.0f),
               offset(_offset),
               offsetScale(_offsetScale)
{

}

Object::~Object()
{}

void Object::transform(std::shared_ptr<MatrixStack>& MV)
{
    // Transform the shape
    MV->translate(glm::vec3(translation.x, translation.y, translation.z));
    MV->scale(scale);
    MV->rotate(angles.x, 1, 0, 0);
    MV->rotate(angles.y, 0, 1, 0);
    MV->rotate(angles.z, 0, 0, 1);
}

void Object::draw(std::shared_ptr<Program> &prog)
{
    shape->draw(prog);
}

void Object::bind(std::shared_ptr<Program>& prog, float t) const
{
    // This will set the color for the object
    glUniform3f(prog->getUniform("ke"), ke.r, ke.g, ke.b);
    glUniform3f(prog->getUniform("kd"), kd.r, kd.g, kd.b);
    glUniform3f(prog->getUniform("ks"), ks.r, ks.g, ks.b);
    glUniform1f(prog->getUniform("s"), s);
}

void Object::bindTransform(std::shared_ptr<Program> &prog, std::shared_ptr<MatrixStack> &MV)
{
    // Bind the model view matrices
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("ITMV"), 1, GL_FALSE,
                       glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));
}

void Object::bindFirstPass(std::shared_ptr<Program> &prog, std::shared_ptr<MatrixStack> &MV, float t) const
{
    // Bind material values
    glUniform3f(prog->getUniform("ke"), ke.r, ke.g, ke.b);
    glUniform3f(prog->getUniform("kd"), kd.r, kd.g, kd.b);

    // Bind the model view matrices
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("ITMV"), 1, GL_FALSE,
                       glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));
}
