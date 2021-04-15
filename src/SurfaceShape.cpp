#include "SurfaceShape.h"

SurfaceShape::SurfaceShape()
{
    // Some constants
    int vertices = 50;
    float squares = (float)vertices - 1.0f;

    // Start with vertices
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            // Push back texture coordinates into buffer
            texBuf.push_back((float)-i / squares);
            texBuf.push_back((float)-j / squares);

            // Caluclate the positions
            float theta = (float)i / squares * 2 * M_PI;
            float x = (float)j / squares * 10;

            // Push back positions into buffer
            posBuf.push_back(x);
            posBuf.push_back(theta);
            posBuf.push_back(0.0f);

            // Push back normals into buffer
            norBuf.push_back(0.0f);
            norBuf.push_back(0.0f);
            norBuf.push_back(0.0f);

        }
    }

    // Now indices (pretend its a square grid)
    for(int x = 0; x < vertices - 1; x++)
    {
        for(int y = 0; y < vertices - 1; y++)
        {
            indBuf.push_back(vertices * y + x); // bottom left
            indBuf.push_back(vertices * (y + 1) + x + 1); // top right
            indBuf.push_back(vertices * y + x + 1); // bottom right
            indBuf.push_back(vertices * y + x); // bottom left
            indBuf.push_back(vertices * (y + 1) + x); // top left
            indBuf.push_back(vertices * (y + 1) + x + 1); // top right
        }
    }

    // Total number of indices
    indCount = (int)indBuf.size();

    // Generate buffer IDs and put them in the bufIDs map.
    GLuint tmp[2];
    glGenBuffers(2, tmp);
    bufIDs["bPos"] = tmp[0];
    bufIDs["bInd"] = tmp[1];
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    assert(norBuf.size() == posBuf.size());
}

SurfaceShape::~SurfaceShape()
{
}

void SurfaceShape::draw(std::shared_ptr<Program> prog)
{
    float t = glfwGetTime();
    glUniform1f(prog->getUniform("t"), t);
    glEnableVertexAttribArray(prog->getAttribute("aPos"));
    GLSL::checkError(GET_FILE_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(prog->getAttribute("aPos"));
}
