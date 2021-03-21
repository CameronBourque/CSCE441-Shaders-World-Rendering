#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.h"
#include "MatrixStack.h"
#include "Material.h"

class Object
{
public:
    Object(std::shared_ptr<Shape> _shape, std::shared_ptr<Material> _material, glm::vec3 _translation,
           glm::vec3 _angles, glm::vec3 _scale, glm::mat4 _shear = glm::mat4(0), bool _doShear = false);
    ~Object();

    void transform(std::shared_ptr<MatrixStack>& MV, bool grounded);

    void setGrowth(float _growth) { growth = _growth; }
    void setGrowthLimiter(float _growthLimiter) { growthLimiter = _growthLimiter; }
    void setYAngle(float angle) { angles.y = angle; }

    std::shared_ptr<Material> getMaterial() { return material; }
    std::shared_ptr<Shape> getShape() { return shape; }
    glm::vec3 getTranslation() { return translation; }

private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    glm::vec3 translation;
    glm::vec3 angles;
    glm::vec3 scale;
    glm::mat4 shear;
    bool doShear;

    float growth;
    float growthLimiter;
};

#endif