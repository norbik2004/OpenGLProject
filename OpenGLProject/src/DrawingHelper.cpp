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

void DrawingHelper::drawScene(Shader& textureShader, Scene& scene, Camera& camera)
{

    glClearColor(0.53f, 0.68f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!scene.textureMeshes.empty()) {
        for (Mesh* m : scene.textureMeshes)
        {
            m->Draw(textureShader, camera);
        }
    }
}


void DrawingHelper::floorTiles(Texture textures[], Scene& scene)
{
    Vertex vertices[] =
    {
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(1.0f, 0.0f)}
    };

    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    Mesh* tile = new Mesh(verts, ind, tex); // dynamiczne tworzenie
    scene.AddTextureMesh(tile);
}