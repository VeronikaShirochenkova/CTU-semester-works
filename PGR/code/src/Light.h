//----------------------------------------------------------------------------------------
/**
 * \file    Light.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Light positions/functions for setting light uniform variables
 */
 //----------------------------------------------------------------------------------------

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


glm::vec3 pointLightPositions[] = 
{
    glm::vec3( 0.355f,  1.440f,  -1.940f),          // magic table
    glm::vec3( 1.163f,  0.975f,  -1.195f),          // magic table
    glm::vec3(-0.522f,  0.650f,  -0.561f),          // near magic table
    glm::vec3( 2.140f,  2.245f,  -0.700f),          // fireplace
    glm::vec3( 2.130f,  2.370f,  -0.497f),          // fireplace
    glm::vec3(-2.300f,  1.710f,  -0.312f),          // shelf
    glm::vec3(-2.270f,  1.760f,  -0.115f),          // shelf
};

/* set uniform light variables to shader */
void setLightUniforms(Shader* mainShader, Camera* camera) 
{
    glUniform1f(glGetUniformLocation(mainShader->ID, "material.shininess"), 32.0f);

    glUniform3f(glGetUniformLocation(mainShader->ID, "dirLight.direction"), 0.0f, 3.5f, 0.0f);
    glUniform3f(glGetUniformLocation(mainShader->ID, "dirLight.ambient"), 0.02f, 0.02f, 0.02f);
    glUniform3f(glGetUniformLocation(mainShader->ID, "dirLight.diffuse"), 0.78f, 0.37f, 0.55f);
    glUniform3f(glGetUniformLocation(mainShader->ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

    for (int i = 0; i < POINT_LIGHT_POS; i++)
    {
        glUniform3fv(glGetUniformLocation(mainShader->ID, ("pointLights[" + std::to_string(i) + "].position").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
        glUniform3f(glGetUniformLocation(mainShader->ID,  ("pointLights[" + std::to_string(i) + "].ambient").c_str()), 0.02f, 0.02f, 0.02f);
        glUniform3f(glGetUniformLocation(mainShader->ID,  ("pointLights[" + std::to_string(i) + "].diffuse").c_str()), 0.39f, 0.20f, 0.10f);
        glUniform3f(glGetUniformLocation(mainShader->ID,  ("pointLights[" + std::to_string(i) + "].specular").c_str()), 0.5f, 0.5f, 0.5f);

        glUniform1f(glGetUniformLocation(mainShader->ID, ("pointLights[" + std::to_string(i) + "].constant").c_str()), 1.0f);
        glUniform1f(glGetUniformLocation(mainShader->ID, ("pointLights[" + std::to_string(i) + "].linear").c_str()), 0.22f);
        glUniform1f(glGetUniformLocation(mainShader->ID, ("pointLights[" + std::to_string(i) + "].quadratic").c_str()), 0.20f);
    }

    glUniform3fv(glGetUniformLocation(mainShader->ID, "spotLight.position"), 1, glm::value_ptr(camera->Position));
    glUniform3fv(glGetUniformLocation(mainShader->ID, "spotLight.direction"), 1, glm::value_ptr(camera->Front));

    glUniform3f(glGetUniformLocation(mainShader->ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(mainShader->ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(mainShader->ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);

    glUniform1f(glGetUniformLocation(mainShader->ID, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(mainShader->ID, "spotLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(mainShader->ID, "spotLight.quadratic"), 0.032f);
    glUniform1f(glGetUniformLocation(mainShader->ID, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(mainShader->ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
}
