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

    std::vector<std::shared_ptr<Shape>> objectShapes;

    std::shared_ptr<Shape> bunny = std::make_shared<Shape>();
    bunny->loadMesh(resDir + "bunny.obj");
    bunny->init();
    objectShapes.push_back(bunny);

    std::shared_ptr<Shape> teapot = std::make_shared<Shape>();
    teapot->loadMesh(resDir + "teapot.obj");
    teapot->init();
    objectShapes.push_back(teapot);

    std::shared_ptr<Shape> sphere = std::make_shared<Shape>();
    sphere->loadMesh(resDir + "sphere.obj");
    sphere->init();

    std::shared_ptr<Shape> square = std::make_shared<Shape>();
    square->loadMesh(resDir + "square.obj");
    square->init();

    // Set up the ground
    ground = std::make_shared<Object>(square,
                                      glm::vec3(0.0, 0.0, 0.0),
                                      glm::vec3(-M_PI / 2, 0.0, 0.0),
                                      glm::vec3(100.0, 1.0, 100.0),
                                      glm::vec3(0.5, 0.5, 0.5),
                                      glm::vec3(1.0),
                                      Object::ObjectShape::OTHER
    );

    // Set up world objects
    for(int i = 0; i < 100; i++)
    {
        size_t shapeIndex = (i % objectShapes.size()) ^ ((i / 10) % objectShapes.size());
        std::shared_ptr<Object> obj = std::make_shared<Object>(objectShapes[shapeIndex],
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
                                                               (Object::ObjectShape) shapeIndex
        );
        objs.push_back(obj);
    }

    // Set up lights
    for(int i = 0; i < 10; i++)
    {
        std::shared_ptr<Light> light = std::make_shared<Light>(sphere,
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
        light->getShape()->draw(prog);
        MV->popMatrix();
    }

    // Draw objects
    for(std::shared_ptr<Object> obj : objs)
    {
        obj->bind(prog);
        MV->pushMatrix();
        obj->transform(MV);
        Object::bindTransform(prog, MV);
        obj->getShape()->draw(prog);
        MV->popMatrix();
    }

    // Draw ground
    ground->bind(prog);
    MV->pushMatrix();
    ground->transform(MV);
    Object::bindTransform(prog, MV);
    ground->getShape()->draw(prog);
    MV->popMatrix();

    // Unbind program
    prog->unbind();
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
