#ifndef _TEXTURE_OBJECT_H_
#define _TEXTURE_OBJECT_H_

#include <memory>
#include <map>
#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "MatrixStack.h"
#include "Program.h"

class TextureObject
{
public:
    TextureObject(std::shared_ptr<Texture> _texture);
    ~TextureObject();

    void bind(std::shared_ptr<Program> prog,std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack> MV);
    void unbind();
    void draw();

private:
    glm::mat3 T;
    std::shared_ptr<Texture> texture;
    std::vector<float> posBuf;
    std::vector<float> texBuf;
    std::vector<unsigned int> indBuf;
    std::map<std::string, GLuint> bufIDs;
    int indCount;
};

#endif