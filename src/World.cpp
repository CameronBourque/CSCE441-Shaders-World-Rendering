#include "World.h"

World::World(std::string resDir) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f),
    lightCount(25)
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
                                      glm::vec3(0.1, 0.1, 0.1),
                                      glm::vec3(1.0)
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
        glm::vec3 translate = glm::vec3(getRandom() * 10.0f - 5.0f, 0.1f, getRandom() * 10.0f - 5.0f);
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

void World::draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV)
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
    float t = (float)glfwGetTime();
    for(std::shared_ptr<Object> obj : objs)
    {
        if(obj->needsNewProgram())
        {
            obj->bind(surfaceProg, t);
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
    glm::vec3 lightPos[25];
    glm::vec3 lightColor[25];
    for(size_t i = 0; i < lightCount; i++)
    {
        //glm::vec4 pos = MV->topMatrix() * glm::vec4(lights[i]->getPosition(MV), 1);
        lightPos[i] = lights[i]->getPosition(MV);
        lightColor[i] = lights[i]->getKE();
    }
    glUniform3fv(prog->getUniform("lightPos"), lightCount, glm::value_ptr(lightPos[0]));
    glUniform3fv(prog->getUniform("lightColor"), lightCount, glm::value_ptr(lightColor[0]));
}
