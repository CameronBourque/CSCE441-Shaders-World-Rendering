#include "ShaderManager.h"

ShaderManager::ShaderManager(std::string resDir) :
    programSelection(0),
    materialSelection(0),
    lightSelection(0),
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
    blinnPhong->addUniform("lightColor");
    blinnPhong->addUniform("ka");
    blinnPhong->addUniform("kd");
    blinnPhong->addUniform("ks");
    blinnPhong->addUniform("s");
    blinnPhong->setVerbose(false);
    programs.push_back(blinnPhong);

    // Set up silhouette shader
    std::shared_ptr<Program> silhouette = std::make_shared<Program>();
    silhouette->setShaderNames(resDir + "silhouette_vert.glsl", resDir + "silhouette_frag.glsl");
    silhouette->setVerbose(true);
    silhouette->init();
    silhouette->addAttribute("aPos");
    silhouette->addAttribute("aNor");
    silhouette->addUniform("ITMV");
    silhouette->addUniform("MV");
    silhouette->addUniform("P");
    silhouette->setVerbose(false);
    programs.push_back(silhouette);

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
                                                                   glm::vec3(0.5f, 0.5f, 0.6f),  // Kd
                                                                   glm::vec3(0.1f, 0.1f, 0.1f),  // Ks
                                                                   10                       // s
    );
    materials.push_back(grayish);

    // Set up lights
    std::shared_ptr<Light> l1 = std::make_shared<Light>(glm::vec3(0.8, 0.8, 0.8), // color
                                                        glm::vec3(1.0, 1.0, 1.0) // light position
    );
    lights.push_back(l1);

    std::shared_ptr<Light> l2 = std::make_shared<Light>(glm::vec3(0.2, 0.2, 0.0), // color
                                                        glm::vec3(-1.0, 1.0, 1.0) // light position
    );
    lights.push_back(l2);
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

void ShaderManager::changeLight(bool decrement)
{
    // Are we incrementing or decrementing?
    if(!decrement)
    {
        // Check if need to go to start index
        if(++lightSelection == lights.size())
        {
            lightSelection = 0;
        }
    }
    else
    {
        // Check if need to go to end index
        if(lightSelection-- == 0)
        {
            lightSelection = lights.size() - 1;
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
        glm::vec3 lightPos[2];
        glm::vec3 lightColor[2];
        for(int i = 0; i < 2; i++)
        {
            lightPos[i] = lights[i]->getPosition();
            lightColor[i] = lights[i]->getColor();
        }
        glUniform3fv(programs[programSelection]->getUniform("lightPos"), 2, glm::value_ptr(lightPos[0]));
        glUniform3fv(programs[programSelection]->getUniform("lightColor"), 2, glm::value_ptr(lightColor[0]));

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

    // If using Blinn Phong or Silhouette shader need to use inverse transpose matrix
    if(programSelection == 1 || programSelection == 2)
    {
        glUniformMatrix4fv(programs[programSelection]->getUniform("ITMV"), 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));
    }

    bunny->draw(programs[programSelection]);
}
