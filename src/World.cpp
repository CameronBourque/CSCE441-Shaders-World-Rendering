#include "World.h"

World::World(std::string resDir) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f)
{
    // Initialize the shader manager
    shaderManager = std::make_shared<ShaderManager>(resDir);
    shaderManager->changeProgram(); // want to use Blinn Phong shading

    // Initialize object shapes
    std::shared_ptr<Shape> bunny = std::make_shared<Shape>();
    bunny->loadMesh(resDir + "bunny.obj");
    bunny->init();

    std::shared_ptr<Shape> teapot = std::make_shared<Shape>();
    teapot->loadMesh(resDir + "teapot.obj");
    teapot->init();

    // Use vector so we can get shapes by index
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(bunny);
    shapes.push_back(teapot);

    // Fill vector of objects
    for(int i = 0; i < 100; i++)
    {
        std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.2, 0.2, 0.2),
                                                                        glm::vec3(getRandom(), getRandom(), getRandom()),
                                                                        glm::vec3(1.0, 1.0, 1.0),
                                                                        200);

        std::shared_ptr<Object> obj = std::make_shared<Object>(shapes[(i % 2) ^ ((i / 10) % 2)],
                                                               material,
                                                               glm::vec3((i % 10) - 4.5, 0, ((i / 10) % 10) - 4.5),
                                                               glm::vec3(0.0, getRandom() * 2 * M_PI, 0.0),
                                                               glm::vec3(0.25, 0.25, 0.25));
        obj->setGrowthLimiter(getRandom() * 0.75 + 0.125);
        objs.push_back(obj);
    }

    // Set up the sun
    std::shared_ptr<Shape> sphere = std::make_shared<Shape>();
    sphere->loadMesh(resDir + "sphere.obj");
    sphere->init();

    std::shared_ptr<Material> sunMat = std::make_shared<Material>(glm::vec3(1.0, 1.0, 0.0),
                                                                  glm::vec3(0.0, 0.0, 0.0),
                                                                  glm::vec3(0.0, 0.0, 0.0),
                                                                  1);
    sun = std::make_shared<Object>(sphere,
                                   sunMat,
                                   glm::vec3(-3, 3, -15),
                                   glm::vec3(0, 0, 0),
                                   glm::vec3(1, 1, 1));
    std::shared_ptr<Light> light = std::make_shared<Light>(glm::vec3(1.0, 1.0, 0.8),
                                                           sun->getTranslation());
    shaderManager->addLight(light);

    // Set up the ground
    std::shared_ptr<Shape> square = std::make_shared<Shape>();
    square->loadMesh(resDir + "square.obj");
    square->init();

    std::shared_ptr<Material> groundMat = std::make_shared<Material>(glm::vec3(0.3, 0.5, 0.1),
                                                                     glm::vec3(0.0, 0.0, 0.0),
                                                                     glm::vec3(0.0, 0.0, 0.0),
                                                                     100000);
    ground = std::make_shared<Object>(square,
                                      groundMat,
                                      glm::vec3(0, 0, 0),
                                      glm::vec3(M_PI / 2, 0, 0),
                                      glm::vec3(100, 1, 100));

    // Set up the HUD
    for(int i = 0; i < 2; i++)
    {
        std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.0f),
                                                                        glm::vec3(0.8f),
                                                                        glm::vec3(0.0f),
                                                                        10000);

        std::shared_ptr<Object> obj = std::make_shared<Object>(shapes[i % 2],
                                                               material,
                                                               glm::vec3(((i % 2) * 2 - 1) * 0.74f, 0.6f, 0.0f),
                                                               glm::vec3(0.0f, 0.0f, 0.0f),
                                                               glm::vec3(0.25f, 0.25f, 0.25f));

        hud.push_back(obj);
    }
}

World::~World()
{}

void World::draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t)
{

    // Shader manager needs to bind first
    shaderManager->bind(P, MV);

    shaderManager->draw(sun, MV);

    // Go through each object and draw
    for(std::shared_ptr<Object> obj : objs)
    {
        obj->setGrowth(0.1f * (float)std::sin(t));
        shaderManager->draw(obj, MV, true);
    }

    shaderManager->draw(ground, MV);


    // Shader manager needs to unbind now
    shaderManager->unbind();
}

void World::drawHUD(std::shared_ptr<MatrixStack> &P, double t)
{
    std::shared_ptr<MatrixStack> MV = std::make_shared<MatrixStack>();

    // Shader manager needs to bind
    shaderManager->bind(P, MV);

    // Draw HUD with different shader
    for(std::shared_ptr<Object> obj : hud)
    {
        obj->setYAngle(t * M_PI / 6);
        shaderManager->draw(obj, MV, true);
    }

    // Shader manager need to unbind
    shaderManager->unbind();
}

void World::drawTopDown(double t)
{

}
