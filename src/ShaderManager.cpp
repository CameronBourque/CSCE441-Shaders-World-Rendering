#include "ShaderManager.h"

ShaderManager::ShaderManager(std::string resDir) :
    programSelection(0),
    lightSelection(0)
{
    // Set up blinn phong shader
    std::shared_ptr<Program> blinnPhong = std::make_shared<Program>();
    blinnPhong->setShaderNames(resDir + "blinn_phong_vert.glsl", resDir + "blinn_phong_frag.glsl");
    blinnPhong->setVerbose(true);
    blinnPhong->init();
    blinnPhong->addAttribute("aPos");
    blinnPhong->addAttribute("aNor");
    blinnPhong->addAttribute("aTex");
    blinnPhong->addUniform("ITMV");
    blinnPhong->addUniform("MV");
    blinnPhong->addUniform("P");
    blinnPhong->addUniform("lightPos");
    blinnPhong->addUniform("lightColor");
    blinnPhong->addUniform("ka");
    blinnPhong->addUniform("kd");
    blinnPhong->addUniform("ks");
    blinnPhong->addUniform("s");
    blinnPhong->addUniform("texture");
    blinnPhong->setVerbose(false);
    programs.push_back(blinnPhong);
}

ShaderManager::~ShaderManager()
{}

void ShaderManager::changeProgram(bool decrement)
{
    // Are we incrementing or decrementing?
    if(!decrement)
    {
        // Check if need to go to start index
        if(++programSelection >= programs.size())
        {
            programSelection = 0;
        }
    }
    else
    {
        // Check if need to go to end index
        if(programSelection-- <= 0)
        {
            programSelection = programs.size() - 1;
        }
    }
}

void ShaderManager::changeLight(bool decrement)
{
    // Are we incrementing or decrementing?
    if(!decrement)
    {
        // Check if need to go to start index
        if(++lightSelection >= lights.size())
        {
            lightSelection = 0;
        }
    }
    else
    {
        // Check if need to go to end index
        if(lightSelection-- <= 0)
        {
            lightSelection = lights.size() - 1;
        }
    }
}

void ShaderManager::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
}

void ShaderManager::bind(std::shared_ptr<MatrixStack>& P, std::shared_ptr<MatrixStack>& MV)
{
    // Bind and then set the perspective uniform matrix
    programs[programSelection]->bind();
    glUniformMatrix4fv(programs[programSelection]->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

    // If using Blinn Phong or Cel shader need to set lighting
    if(programSelection == 0)
    {
        // Set lighting position
        glm::vec4 lightPos = glm::vec4(lights[lightSelection]->getPosition(), 0);
        // Convert from world space to camera space
        lightPos = glm::transpose(glm::inverse(MV->topMatrix())) * lightPos;
        glUniform3f(programs[programSelection]->getUniform("lightPos"), lightPos.x, lightPos.y, lightPos.z);
    }

}

void ShaderManager::unbind()
{
    programs[programSelection]->unbind();
}

void ShaderManager::draw(std::shared_ptr<Object>& obj, std::shared_ptr<MatrixStack>& MV, bool grounded)
{
    // Push the model view matrix
    MV->pushMatrix();

    // Apply object transformations
    obj->transform(MV, grounded);

    // Add MV
    glUniformMatrix4fv(programs[programSelection]->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));

    // If using Blinn Phong or Cel shader need to set more uniforms
    if(programSelection == 0)
    {
        // Set inverse transpose matrix
        glUniformMatrix4fv(programs[programSelection]->getUniform("ITMV"), 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(MV->topMatrix()))));

        // Set lighting color
        glm::vec3 lightColor = lights[lightSelection]->getColor();
        glUniform3f(programs[programSelection]->getUniform("lightColor"), lightColor.r, lightColor.g, lightColor.b);

        // Set material
        glm::vec3 ka = obj->getMaterial()->getKA();
        glUniform3f(programs[programSelection]->getUniform("ka"), ka.r, ka.g, ka.b);
        glm::vec3 kd = obj->getMaterial()->getKD();
        glUniform3f(programs[programSelection]->getUniform("kd"), kd.r, kd.g, kd.b);
        glm::vec3 ks = obj->getMaterial()->getKS();
        glUniform3f(programs[programSelection]->getUniform("ks"), ks.r, ks.g, ks.b);
        glUniform1f(programs[programSelection]->getUniform("s"), obj->getMaterial()->getS());
    }

    obj->getShape()->draw(programs[programSelection]);

    // Pop the model view matrix
    MV->popMatrix();
}
