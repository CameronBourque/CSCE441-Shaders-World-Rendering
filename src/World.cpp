#include "World.h"

World::World(std::string resDir) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f)
{
    // Set up shader
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

    std::shared_ptr<Shape> bunny = std::make_shared<Shape>();
    bunny->loadMesh(resDir + "bunny.obj");
    bunny->init();

    std::shared_ptr<Shape> sphere = std::make_shared<Shape>();
    sphere->loadMesh(resDir + "sphere.obj");
    sphere->init();

    // Set up world objects
    for(int i = 0; i < 100; i++)
    {
        std::shared_ptr<Object> obj = std::make_shared<Object>(bunny,
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
                                                               glm::vec3(1.0), // ks
                                                               1000  // s
        );
        objs.push_back(obj);
    }

    // Set up lights
    for(int i = 0; i < 10; i++)
    {
        std::shared_ptr<Light> light = std::make_shared<Light>(sphere,
                                                               glm::vec3((float)i,
                                                                                   0.0,
                                                                                   (float)i
                                                                                  ), // translation
                                                               glm::vec3(0.0), // angles
                                                               glm::vec3(0.1), // scale
                                                               glm::vec3(getRandom(),
                                                                             getRandom(),
                                                                             getRandom()
                                                                            ), // ke
                                                               10  // s
        );
        lights.push_back(light);
    }
}

World::~World()
{}

void World::draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t)
{
    prog->bind();
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    glm::vec3 lightPos[10];
    glm::vec3 lightColor[10];
    for(int i = 0; i < 10; i++)
    {
        lightPos[i] = lights[i]->getTranslation();
        lightColor[i] = lights[i]->getKE();
    }

    for(std::shared_ptr<Light> light : lights)
    {
        light->bind(prog);
        MV->pushMatrix();
        light->transform(MV);
        Object::bindTransform(prog, MV);
        glUniform3fv(prog->getUniform("lightPos"), 10, glm::value_ptr(lightPos[0]));
        glUniform3fv(prog->getUniform("lightColor"), 10, glm::value_ptr(lightColor[0]));
        light->getShape()->draw(prog);
        MV->popMatrix();
    }

    for(std::shared_ptr<Object> obj : objs)
    {
        obj->bind(prog);
        MV->pushMatrix();
        obj->transform(MV, true);
        Object::bindTransform(prog, MV);
        glUniform3fv(prog->getUniform("lightPos"), 10, glm::value_ptr(lightPos[0]));
        glUniform3fv(prog->getUniform("lightColor"), 10, glm::value_ptr(lightColor[0]));
        obj->getShape()->draw(prog);
        MV->popMatrix();
    }

    prog->unbind();
}
