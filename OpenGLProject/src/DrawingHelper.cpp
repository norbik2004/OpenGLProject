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
    float repeatX = 100.0f;
    float repeatZ = 100.0f;

    Vertex vertices[] =
    {
        Vertex{glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, repeatZ)},
        Vertex{glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(repeatX, repeatZ)},
        Vertex{glm::vec3(100.0f, 0.0f, 100.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(repeatX, 0.0f)}
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

void DrawingHelper::spawnArea(Texture textures[], Scene& scene)
{
    float height = 3.0f;
    float wLeft = 1.0f;
    float wCenter = 6.0f;
    float wRight = 2.0f;

    float x0 = 0.0f;
    float x1 = x0 + wLeft;
    float x2 = x1 + wCenter;
    float x3 = x2 + wRight;

    // --- LEFT WALL SEGMENT ----------------------------------------------------
    {
        Vertex verticies[] = {
            { {x0, 0, 0}, {1,1,1}, {0,0,1}, {0,2} },
            { {x0, height, 0}, {1,1,1}, {0,0,1}, {0,0} },
            { {x1, height, 0}, {1,1,1}, {0,0,1}, {1,0} },
            { {x1, 0, 0}, {1,1,1}, {0,0,1}, {1,2} }
        };

        GLuint indicies[] = { 0,1,2, 0,2,3 };

        std::vector<Vertex> verts(verticies, verticies + sizeof(verticies) / sizeof(Vertex));
        std::vector<GLuint> ind(indicies, indicies + sizeof(indicies) / sizeof(GLuint));
        std::vector<Texture> tex = { textures[1] };

        Mesh* leftSegment = new Mesh(verts, ind, tex);
        scene.AddTextureMesh(leftSegment);
    }
}

