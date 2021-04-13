#include "World.h"

World::World(std::string resDir) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f)
{
    // Set up shaders
    prog = std::make_shared<Program>();
    prog->setShaderNames(resDir + "blinn_phong_vert.glsl", resDir + "blinn_phong_frag.glsl");
    prog->setVerbose(true);
    prog->init();
    prog->addAttribute("aPos");
    prog->addAttribute("aNor");
    prog->addUniform("ITMV");
    prog->addUniform("MV");
    prog->addUniform("P");
    prog->addUniform("lightPos");
    prog->addUniform("lightColor");
    prog->addUniform("ke");
    prog->addUniform("kd");
    prog->addUniform("ks");
    prog->addUniform("s");
    prog->setVerbose(false);

    surfaceProg = std::make_shared<Program>();
    surfaceProg->setShaderNames(resDir + "surface_vert.glsl", resDir + "blinn_phong_frag.glsl");
    surfaceProg->setVerbose(true);
    surfaceProg->init();
    surfaceProg->addAttribute("aPos");
    surfaceProg->addUniform("ITMV");
    surfaceProg->addUniform("MV");
    surfaceProg->addUniform("P");
    surfaceProg->addUniform("t");
    surfaceProg->addUniform("lightPos");
    surfaceProg->addUniform("lightColor");
    surfaceProg->addUniform("ke");
    surfaceProg->addUniform("kd");
    surfaceProg->addUniform("ks");
    surfaceProg->addUniform("s");
    surfaceProg->setVerbose(false);

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

    // Set up the ground
    ground = std::make_shared<Object>(square,
                                      glm::vec3(0.0, 0.0, 0.0),
                                      glm::vec3(-M_PI / 2, 0.0, 0.0),
                                      glm::vec3(100.0, 1.0, 100.0),
                                      glm::vec3(0.8, 0.8, 0.8),
                                      glm::vec3(1.0)
    );

    // Set up world objects
    for(int i = 0; i < 100; i++)
    {
        size_t shapeIndex = (i % objectShapes);
        std::shared_ptr<Object> obj;
        switch (shapeIndex)
        {
            case 0:
                obj = std::make_shared<Bunny>(bunny, // shape
                                              glm::vec3((i % 10) - 4.5,
                                                        0.0,
                                                        ((i / 10) % 10) - 4.5
                                              ), // translation
                                              glm::vec3(0.0), // angles
                                              glm::vec3(0.25), // scale
                                              glm::vec3(getRandom(),
                                                        getRandom(),
                                                        getRandom()
                                              ), //kd
                                              glm::vec3(1.0) // ks
                );
                break;
            case 1:
                obj = std::make_shared<Teapot>(teapot, // shape
                                               glm::vec3((i % 10) - 4.5,
                                                         0.0,
                                                         ((i / 10) % 10) - 4.5
                                               ), // translation
                                               glm::vec3(0.0), // angles
                                               glm::vec3(0.25), // scale
                                               glm::vec3(getRandom(),
                                                         getRandom(),
                                                         getRandom()
                                               ), //kd
                                               glm::vec3(1.0) // ks
                );
                break;
            case 2:
                obj = std::make_shared<Ball>(ball, // shape
                                             glm::vec3((i % 10) - 4.5,
                                                       0.0,
                                                       ((i / 10) % 10) - 4.5
                                             ), // translation
                                             glm::vec3(0.0), // angles
                                             glm::vec3(0.25), // scale
                                             glm::vec3(getRandom(),
                                                       getRandom(),
                                                       getRandom()
                                             ), //kd
                                             glm::vec3(1.0) // ks
                );
                break;
            case 3:
                obj = std::make_shared<Surface>(surface, // shape
                                                glm::vec3((i % 10) - 4.5,
                                                          0.0,
                                                          ((i / 10) % 10) - 4.5
                                                ), // translation
                                                glm::vec3(0.0,
                                                          0.0,
                                                          -M_PI / 2), // angles
                                                glm::vec3(0.05), // scale
                                                glm::vec3(getRandom(),
                                                          getRandom(),
                                                          getRandom()
                                                ), //kd
                                                glm::vec3(1.0) // ks
                );
                break;
        }
        objs.push_back(obj);
    }

    // Set up lights
    for(int i = 0; i < 10; i++)
    {
        std::shared_ptr<Light> light = std::make_shared<Light>(ball,
                                                               glm::vec3((float)i - 4,
                                                                         0.1,
                                                                         (float)i - 4
                                                                        ), // translation
                                                               glm::vec3(0.0), // angles
                                                               glm::vec3(0.1), // scale
                                                               glm::vec3(getRandom(),
                                                                         getRandom(),
                                                                         getRandom()
                                                                        ) // ke
        );
        lights.push_back(light);
    }
}

World::~World()
{}

void World::draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t)
{
    // Bind program
    prog->bind();

    // Set perspective
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // Bind lights
    bindLights(MV);

    // Draw lights
    for(std::shared_ptr<Light> light : lights)
    {
        light->bind(prog);
        MV->pushMatrix();
        light->transform(MV);
        Object::bindTransform(prog, MV);
        light->draw(prog);
        MV->popMatrix();
    }

    // Draw objects
    for(std::shared_ptr<Object> obj : objs)
    {
        if(!obj->needsNewProgram())
        {
            obj->bind(prog);
            MV->pushMatrix();
            obj->transform(MV);
            Object::bindTransform(prog, MV);
            obj->draw(prog);
            MV->popMatrix();
        }
    }

    // Draw ground
    ground->bind(prog);
    MV->pushMatrix();
    ground->transform(MV);
    Object::bindTransform(prog, MV);
    ground->draw(prog);
    MV->popMatrix();

    // Unbind program
    prog->unbind();

    // Bind surface program
    surfaceProg->bind();

    // Set perspective
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // Bind lights
    bindLights(MV);

    // Draw remaining objects
    for(std::shared_ptr<Object> obj : objs)
    {
        if(obj->needsNewProgram())
        {
            glUniform1f(surfaceProg->getUniform("t"), (float)glfwGetTime());
            obj->bind(surfaceProg);
            MV->pushMatrix();
            obj->transform(MV);
            Object::bindTransform(surfaceProg, MV);
            obj->draw(surfaceProg);
            MV->popMatrix();
        }
    }

    // Unbind surface program
    surfaceProg->unbind();
}

void World::bindLights(std::shared_ptr<MatrixStack>& MV)
{
    // Set light positions
    glm::vec3 lightPos[10];
    glm::vec3 lightColor[10];
    for(int i = 0; i < 10; i++)
    {
        glm::vec4 pos = MV->topMatrix() * glm::vec4(lights[i]->getTranslation(), 1);
        lightPos[i] = pos;
        lightColor[i] = lights[i]->getKE();
    }
    glUniform3fv(prog->getUniform("lightPos"), 10, glm::value_ptr(lightPos[0]));
    glUniform3fv(prog->getUniform("lightColor"), 10, glm::value_ptr(lightColor[0]));
}
