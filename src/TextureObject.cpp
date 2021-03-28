#include "TextureObject.h"

TextureObject::TextureObject(std::shared_ptr<Texture> _texture) :
    texture(_texture)
{
    // Texture matrix
    T[0][0] = 1.0f;

    // Init geometry
    float textureSize = 25.0f;
    // 0 (top left)
    posBuf.push_back(-textureSize);
    posBuf.push_back(0.0f);
    posBuf.push_back(-textureSize);
    texBuf.push_back(0.0f);
    texBuf.push_back(textureSize);
    // 1 (top right)
    posBuf.push_back(textureSize);
    posBuf.push_back(0.0f);
    posBuf.push_back(-textureSize);
    texBuf.push_back(textureSize);
    texBuf.push_back(textureSize);
    // 2 (bottom left)
    posBuf.push_back(-textureSize);
    posBuf.push_back(0.0f);
    posBuf.push_back(textureSize);
    texBuf.push_back(0.0f);
    texBuf.push_back(0.0f);
    // 3 (bottom right)
    posBuf.push_back(textureSize);
    posBuf.push_back(0.0f);
    posBuf.push_back(textureSize);
    texBuf.push_back(textureSize);
    texBuf.push_back(0.0f);
    // Index
    indBuf.push_back(0);
    indBuf.push_back(1);
    indBuf.push_back(2);
    indBuf.push_back(3);
    indBuf.push_back(2);
    indBuf.push_back(1);
    indCount = (int)indBuf.size();

    // Generate buffer IDs
    GLuint temp[3];
    glGenBuffers(3, temp);
    bufIDs["bPos"] = temp[0];
    bufIDs["bTex"] = temp[1];
    bufIDs["bInd"] = temp[2];

    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bTex"]);
    glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureObject::~TextureObject()
{}

void TextureObject::bind(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack> MV)
{
    // Bind program uniforms and attributes for texture
    texture->bind(prog->getUniform("texture"));
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("T"), 1, GL_FALSE, glm::value_ptr(T));
    glEnableVertexAttribArray(prog->getAttribute("aPos"));
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(prog->getAttribute("aTex"));
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bTex"]);
    glVertexAttribPointer(prog->getAttribute("aTex"), 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
}

void TextureObject::unbind()
{
    texture->unbind();
}

void TextureObject::draw()
{
    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, (void *)0);
}