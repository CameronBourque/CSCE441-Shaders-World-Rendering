#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
    Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _s);
    ~Material();

    glm::vec3 getKA() { return ka; }
    glm::vec3 getKD() { return kd; }
    glm::vec3 getKS() { return ks; }
    float getS() { return s; }

private:
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float s;
};

#endif