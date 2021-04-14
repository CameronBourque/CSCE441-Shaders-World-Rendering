#ifndef _SURFACE_SHAPE_H_
#define _SURFACE_SHAPE_H_

#include <map>
#include <vector>

#include "Shape.h"

#include "GLSL.h"
#include "Program.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class SurfaceShape : public Shape
{
public:
    SurfaceShape();
    ~SurfaceShape();

    void draw(std::shared_ptr<Program> prog);

private:
    std::map<std::string, GLuint> bufIDs;
    int indCount;
    std::vector<unsigned int> indBuf;
};

#endif