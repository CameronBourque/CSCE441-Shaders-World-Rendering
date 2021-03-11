#include "ShaderManager.h"

ShaderManager::ShaderManager(std::string resDir) :
    programSelection(0),
    materialSelection(0),
    lightPos(1.0f, 1.0f, 1.0f)
{
    // Set up normal shader
    std::shared_ptr<Program> normal = std::make_shared<Program>();
    normal->setShaderNames(resDir + "normal_vert.glsl", resDir + "normal_frag.glsl");
    normal->setVerbose(true);
    normal->init();
    normal->addAttribute("aPos");
    normal->addAttribute("aNor");
    normal->addUniform("MV");
    normal->addUniform("P");
    normal->setVerbose(false);
    programs.push_back(normal);

    // Set up blinn phong shader
    std::shared_ptr<Program> blinnPhong = std::make_shared<Program>();
    blinnPhong->setShaderNames(resDir + "blinn_phong_vert.glsl", resDir + "blinn_phong_frag.glsl");
    blinnPhong->setVerbose(true);
    blinnPhong->init();
    blinnPhong->addAttribute("aPos");
    blinnPhong->addAttribute("aNor");
    blinnPhong->addUniform("ITMV");
    blinnPhong->addUniform("MV");
    blinnPhong->addUniform("P");
    blinnPhong->addUniform("lightPos");
    blinnPhong->addUniform("ka");
    blinnPhong->addUniform("kd");
    blinnPhong->addUniform("ks");
    blinnPhong->addUniform("s");
    blinnPhong->setVerbose(false);
    programs.push_back(blinnPhong);

    // Set up materials
    std::shared_ptr<Material> pinkish = std::make_shared<Material>(glm::vec3(0.2f, 0.2f, 0.2f),  // Ka
                                                                   glm::vec3(0.8f, 0.7f, 0.7f),  // Kd
                                                                   glm::vec3(1.0f, 0.9f, 0.8f),  // Ks
                                                                   200                      // s
    );
    materials.push_back(pinkish);

    std::shared_ptr<Material> blueGreen = std::make_shared<Material>(glm::vec3(0.0f, 0.0f, 0.4f),  // Ka
                                                                     glm::vec3(0.0f, 0.0f, 0.6f),  // Kd
                                                                     glm::vec3(0.0f, 1.0f, 0.0f),  // Ks
                                                                     100                      // s
    );
    materials.push_back(blueGreen);


    std::shared_ptr<Material> grayish = std::make_shared<Material>(glm::vec3(0.1f, 0.1f, 0.1f),  // Ka
                                                                   glm::vec3(0.5f, 0.5f, 0.7f),  // Kd
                                                                   glm::vec3(0.1f, 0.1f, 0.1f),  // Ks
                                                                   10                       // s
    );
    materials.push_back(grayish);
}

ShaderManager::~ShaderManager()
{}

void ShaderManager::changeProgram(bool decrement)
{
    // Are we incrementing or decrementing?
    if(!decrement)
    {
        // Check if need to go to start index
        if(++programSelection == programs.size())
        {
            programSelection = 0;
        }
    }
    else
    {
        // Check if need to go to end index
        if(programSelection-- == 0)
        {
            programSelection = programs.size() - 1;
        }
    }
}

void ShaderManager::changeMaterial(bool decrement)
{
    // Are we incrementing or decrementing?
    if(!decrement)
    {
        // Check if need to go to start index
        if(++materialSelection == materials.size())
        {
            materialSelection = 0;
        }
    }
    else
    {
        // Check if need to go to end index
        if(materialSelection-- == 0)
        {
            materialSelection = materials.size() - 1;
        }
    }
}

void ShaderManager::bind(std::shared_ptr<MatrixStack>& P)
{
    // Bind and then set the perspective uniform matrix
    programs[programSelection]->bind();
    glUniformMatrix4fv(programs[programSelection]->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // If using Blinn Phong shader need to set more uniforms
    if(programSelection == 1)
    {
        glUniform3f(programs[programSelection]->getUniform("lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glm::vec3 ka = materials[materialSelection]->getKA();
        glUniform3f(programs[programSelection]->getUniform("ka"), ka.x, ka.y, ka.z);
        glm::vec3 kd = materials[materialSelection]->getKD();
        glUniform3f(programs[programSelection]->getUniform("kd"), kd.x, kd.y, kd.z);
        glm::vec3 ks = materials[materialSelection]->getKS();
        glUniform3f(programs[programSelection]->getUniform("ks"), ks.x, ks.y, ks.z);
        glUniform1f(programs[programSelection]->getUniform("s"), materials[materialSelection]->getS());
    }
}

void ShaderManager::unbind()
{
    programs[programSelection]->unbind();
}

void ShaderManager::draw(std::shared_ptr<Shape>& bunny, std::shared_ptr<MatrixStack>& MV)
{
    // Add MV
    glUniformMatrix4fv(programs[programSelection]->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));

    if(programSelection == 1)
    {
        glUniformMatrix4fv(programs[programSelection]->getUniform("ITMV"), 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));
    }

    bunny->draw(programs[programSelection]);
}
