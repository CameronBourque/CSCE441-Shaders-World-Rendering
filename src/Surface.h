#ifndef _SURFACE_H_
#define _SURFACE_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Object.h"

class Surface : public Object
{
public:
    Surface(std::shared_ptr<Shape> shape, glm::vec3 translation, glm::vec3 angles, glm::vec3 scale, glm::vec3 kd,
    glm::vec3 ks, float offset, float offsetScale);
    ~Surface();

    void transform(std::shared_ptr<MatrixStack>& MV);
    void bind(std::shared_ptr<Program>& prog, float t) const;

    void bindFirstPass(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack> &MV, float t) const;

    bool needsNewProgram() { return true; }
};

#endif