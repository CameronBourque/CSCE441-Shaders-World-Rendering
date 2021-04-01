#include "World.h"

World::World(std::string resDir) :
    randEngine(std::random_device()()),
    randDist(0.0f, 1.0f)
{
}

World::~World()
{}

void World::draw(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV, double t)
{
}
