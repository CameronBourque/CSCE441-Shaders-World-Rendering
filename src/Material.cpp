#include "Material.h"

Material::Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _s) :
    ka(_ka), kd(_kd), ks(_ks), s(_s)
{}

Material::~Material()
{}