#include "BallShape.h"

BallShape::BallShape()
{
    int vertices = 50;
    float squares = (float)vertices - 1.0f;

    // Fill normal and position buffers
    for(int i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            // Calculate x, y, and z
            float theta = (float)j / squares * M_PI;
            float phi = (float)i / squares * 2 * M_PI;
            float x = sin(theta) * sin(phi);
            float y = -cos(theta);
            float z = sin(theta) * cos(phi);

            // Push into buffers
            posBuf.push_back(x);
            posBuf.push_back(y);
            posBuf.push_back(z);
            norBuf.push_back(x);
            norBuf.push_back(y);
            norBuf.push_back(z);
        }
    }

    // Now set up indices
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
    GLuint tmp[3];
    glGenBuffers(3, tmp);
    bufIDs["bPos"] = tmp[0];
    bufIDs["bNor"] = tmp[1];
    bufIDs["bInd"] = tmp[2];
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bNor"]);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    assert(norBuf.size() == posBuf.size());
}

BallShape::~BallShape()
{
}

void BallShape::draw(std::shared_ptr<Program> prog)
{
    glEnableVertexAttribArray(prog->getAttribute("aPos"));
    GLSL::checkError(GET_FILE_LINE);
    glEnableVertexAttribArray(prog->getAttribute("aNor"));
    GLSL::checkError(GET_FILE_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bNor"]);
    glVertexAttribPointer(prog->getAttribute("aNor"), 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIDs["bInd"]);
    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, (void *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(prog->getAttribute("aNor"));
    glDisableVertexAttribArray(prog->getAttribute("aPos"));
}