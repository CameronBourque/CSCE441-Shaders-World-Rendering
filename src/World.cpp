#include "World.h"

#include <iostream>

World::World(std::string& resDir, int texWidth, int texHeight, GLuint& frameBufferID) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f),
    lightCount(25)
{
    // Set up first pass shaders
    prog = std::make_shared<Program>();
    prog->setShaderNames(resDir + "blinn_phong_vert.glsl", resDir + "pass1_frag.glsl");
    prog->setVerbose(true);
    prog->init();
    prog->addAttribute("aPos");
    prog->addAttribute("aNor");
    prog->addUniform("ITMV");
    prog->addUniform("MV");
    prog->addUniform("P");
    prog->addUniform("ke");
    prog->addUniform("kd");
    prog->setVerbose(false);

    surfaceProg = std::make_shared<Program>();
    surfaceProg->setShaderNames(resDir + "surface_vert.glsl", resDir + "pass1_frag.glsl");
    surfaceProg->setVerbose(true);
    surfaceProg->init();
    surfaceProg->addAttribute("aPos");
    surfaceProg->addUniform("ITMV");
    surfaceProg->addUniform("MV");
    surfaceProg->addUniform("P");
    surfaceProg->addUniform("t");
    surfaceProg->addUniform("ke");
    surfaceProg->addUniform("kd");
    surfaceProg->setVerbose(false);

    // Set up second pass shader
    secondPass = std::make_shared<Program>();
    secondPass->setShaderNames(resDir + "pass2_vert.glsl", resDir + "pass2_frag.glsl");
    secondPass->setVerbose(true);
    secondPass->init();
    secondPass->addAttribute("aPos");
    secondPass->addUniform("P");
    secondPass->addUniform("MV");
    secondPass->addUniform("posTexture");
    secondPass->addUniform("norTexture");
    secondPass->addUniform("keTexture");
    secondPass->addUniform("kdTexture");
    secondPass->addUniform("windowSize");
    secondPass->addUniform("lightPos");
    secondPass->addUniform("lightColor");
    secondPass->addUniform("ks");
    secondPass->addUniform("s");
    secondPass->setVerbose(false);
    secondPass->bind();
    glUniform1i(secondPass->getUniform("posTexture"), 0);
    glUniform1i(secondPass->getUniform("norTexture"), 1);
    glUniform1i(secondPass->getUniform("keTexture"), 2);
    glUniform1i(secondPass->getUniform("kdTexture"), 3);
    secondPass->unbind();

    // Create shapes
    int objectShapes = 4;
    std::shared_ptr<Shape> bunny = std::make_shared<Shape>();
    bunny->loadMesh(resDir + "bunny.obj");
    bunny->init();

    std::shared_ptr<Shape> teapot = std::make_shared<Shape>();
    teapot->loadMesh(resDir + "teapot.obj");
    teapot->init();

    std::shared_ptr<Shape> ball = std::make_shared<BallShape>();
    std::shared_ptr<Shape> surface = std::make_shared<SurfaceShape>();

    std::shared_ptr<Shape> square = std::make_shared<Shape>();
    square->loadMesh(resDir + "square.obj");
    square->init();

    std::shared_ptr<Shape> cube = std::make_shared<Shape>();
    cube->loadMesh(resDir + "cube.obj");
    cube->init();

    // Set up texture data for passes
    glGenFramebuffers(1, &frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

    glGenTextures(1, &posTex);
    glBindTexture(GL_TEXTURE_2D, posTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texWidth, texHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);

    glGenTextures(1, &norTex);
    glBindTexture(GL_TEXTURE_2D, norTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texWidth, texHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, norTex, 0);

    glGenTextures(1, &emisTex);
    glBindTexture(GL_TEXTURE_2D, emisTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texWidth, texHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, emisTex, 0);

    glGenTextures(1, &diffTex);
    glBindTexture(GL_TEXTURE_2D, diffTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texWidth, texHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, diffTex, 0);
    GLSL::checkError(GET_FILE_LINE);

    GLuint depthRenderBuffer;
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not ok" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLSL::checkError(GET_FILE_LINE);

    // Set up the ground
    ground = std::make_shared<Object>(square,
                                      glm::vec3(0.0, 0.0, 0.0),
                                      glm::vec3(-M_PI / 2, 0.0, 0.0),
                                      glm::vec3(25.0, 1.0, 25.0),
                                      glm::vec3(1.0, 1.0, 1.0),
                                      glm::vec3(0.25)
    );

    // Set up box
    box = std::make_shared<Object>(cube,
                                   glm::vec3(0.0),
                                   glm::vec3(0.0),
                                   glm::vec3(0.0),
                                   glm::vec3(0.0f)
    );

    // Set up world objects
    for(int i = 0; i < 100; i++)
    {
        // Find inputs for objects
        glm::vec3 translate = glm::vec3((i % 10) - 4.5, 0.0, ((i / 10) % 10) - 4.5);
        glm::vec3 angles = glm::vec3(0.0, getRandom() * 2 * M_PI, 0.0);
        glm::vec3 scale = glm::vec3(getRandom() * 0.25f + 0.125f);
        glm::vec3 kd = glm::vec3(getRandom(), getRandom(), getRandom());
        glm::vec3 ks = glm::vec3(1.0);
        float offset = getRandom() * 100;
        float offsetScale = getRandom() + 0.5f;

        // Figure out which object it is
        size_t shapeIndex = (i % objectShapes);
        std::shared_ptr<Object> obj;
        switch (shapeIndex)
        {
            case 0:
                offset = getRandom() + 0.5f;
                obj = std::make_shared<Bunny>(bunny,
                                              translate,
                                              angles,
                                              scale,
                                              kd,
                                              ks,
                                              offset,
                                              offsetScale
                );
                break;
            case 1:
                obj = std::make_shared<Teapot>(teapot,
                                               translate,
                                               angles,
                                               scale,
                                               kd,
                                               ks,
                                               offset,
                                               offsetScale
                );
                break;
            case 2:
                scale *= 0.75f;
                offsetScale *= 0.5f;
                obj = std::make_shared<Ball>(ball,
                                             translate,
                                             angles,
                                             scale,
                                             kd,
                                             ks,
                                             offset,
                                             offsetScale
                );
                break;
            case 3:
                angles.z = M_PI / 2;
                scale *= 0.2f;
                obj = std::make_shared<Surface>(surface,
                                                translate,
                                                angles,
                                                scale,
                                                kd,
                                                ks,
                                                offset,
                                                offsetScale
                );
                break;
        }
        objs.push_back(obj);
    }

    // Set up lights
    for(size_t i = 0; i < lightCount; i++)
    {
        glm::vec3 translate = glm::vec3(getRandom() * 7.5f - 3.75f, 0.1f, getRandom() * 7.5f - 3.75f);
        glm::vec3 angles = glm::vec3(0.0f, getRandom() * 2 * M_PI, 0.0f);
        glm::vec3 scale = glm::vec3(getRandom() * 0.03f + 0.02f);
        glm::vec3 ke = glm::vec3(getRandom(), getRandom(), getRandom());
        float offset = getRandom() * 100;
        float offsetScale = getRandom() * 0.25f + 0.1f;

        std::shared_ptr<Light> light = std::make_shared<Light>(ball,
                                                               translate,
                                                               angles,
                                                               scale,
                                                               ke,
                                                               offset,
                                                               offsetScale
        );
        lights.push_back(light);
    }
}

World::~World()
{}

void World::bindLights(std::shared_ptr<MatrixStack>& MV)
{
    // Set light positions
    glm::vec3 lightPos[25];
    glm::vec3 lightColor[25];
    for(size_t i = 0; i < lightCount; i++)
    {
        lightPos[i] = lights[i]->getPosition(MV);
        lightColor[i] = lights[i]->getKE();
    }
    glUniform3fv(secondPass->getUniform("lightPos"), lightCount, glm::value_ptr(lightPos[0]));
    glUniform3fv(secondPass->getUniform("lightColor"), lightCount, glm::value_ptr(lightColor[0]));
}

void World::drawFrameBuffer(std::shared_ptr<MatrixStack> &P, std::shared_ptr<MatrixStack> &MV)
{
    // Bind program
    prog->bind();

    // Set perspective
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // Draw lights
    for(std::shared_ptr<Light> light : lights)
    {
        MV->pushMatrix();
        light->transform(MV);
        light->bindFirstPass(prog, MV);
        light->draw(prog);
        MV->popMatrix();
    }

    // Draw objects
    for(std::shared_ptr<Object> obj : objs)
    {
        if(!obj->needsNewProgram())
        {
            MV->pushMatrix();
            obj->transform(MV);
            obj->bindFirstPass(prog, MV);
            obj->draw(prog);
            MV->popMatrix();
        }
    }

    // Draw ground
    MV->pushMatrix();
    ground->transform(MV);
    ground->bindFirstPass(prog, MV);
    ground->draw(prog);
    MV->popMatrix();

    // Unbind program
    prog->unbind();

    // Bind surface program
    surfaceProg->bind();

    // Set perspective
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // Draw remaining objects
    float t = (float)glfwGetTime();
    for(std::shared_ptr<Object> obj : objs)
    {
        if(obj->needsNewProgram())
        {
            MV->pushMatrix();
            obj->transform(MV);
            obj->bindFirstPass(prog, MV, t);
            obj->draw(surfaceProg);
            MV->popMatrix();
        }
    }

    // Unbind surface program
    surfaceProg->unbind();

    // Bind lights and materials to second pass
    secondPass->bind();
    bindLights(MV);
    glUniform3f(secondPass->getUniform("ks"), 1.0f, 1.0f, 1.0f);
    glUniform1f(secondPass->getUniform("s"), 10);
    secondPass->unbind();
}

void World::drawScreen(std::shared_ptr<MatrixStack> &P, std::shared_ptr<MatrixStack> &MV, glm::vec2 windowSize)
{
    // Bind program
    secondPass->bind();

    // Set perspective
    glUniformMatrix4fv(secondPass->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
    glUniformMatrix4fv(secondPass->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
    glUniform2f(secondPass->getUniform("windowSize"), windowSize.x, windowSize.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, posTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emisTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, diffTex);

    box->draw(prog);
    secondPass->unbind();
}
