#include "../include/DrawingHelper.h"

Shader DrawingHelper::setupShaderProgram(const std::string& shaderDir, const std::string& name) const
{
    // Œcie¿ki do plików vertex + fragment
    std::string vertexPath = shaderDir + name + ".vert";
    std::string fragmentPath = shaderDir + name + ".frag";

    // Tworzymy shader i zwracamy
    Shader shaderProgram(vertexPath.c_str(), fragmentPath.c_str());
    return shaderProgram;
}

void DrawingHelper::drawScene(Shader& textureShader, Shader& colorShader, Shader& lightShader, Scene& scene, Camera& camera)
{

    glClearColor(1.0f, 0.13f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!scene.colorMeshes.empty()) {
        for (Mesh* m : scene.colorMeshes)
        {
            m->Draw(colorShader, camera);
        }
    }

    if (!scene.textureMeshes.empty()) {
        for (Mesh* m : scene.textureMeshes)
        {
            m->Draw(textureShader, camera);
        }
    }

    if (!scene.lightMeshes.empty()) {
        for (Mesh* m : scene.lightMeshes)
        {
            m->Draw(lightShader, camera);
        }
    }
}
