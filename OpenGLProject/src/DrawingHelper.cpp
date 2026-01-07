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
	glUniform1f(glGetUniformLocation(textureShader.ID, "uScale"), 1.0f);
	glUniform1i(glGetUniformLocation(textureShader.ID, "useTexture"), true);

	if (!scene.textureMeshes.empty()) {
		for (Mesh* m : scene.textureMeshes)
		{
			m->Draw(textureShader, camera);
		}
	}
}


void DrawingHelper::floorTiles(Texture textures[], Scene& scene)
{
	float repeatX = 40.0f;
	float repeatZ = 40.0f;

	Vertex vertices[] =
	{
		Vertex{glm::vec3(-20.0f, -7.5f, 20.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-20.0f, -7.5f, -20.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(0.0f, repeatZ)},
		Vertex{glm::vec3(20.0f, -7.5f, -20.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(repeatX, repeatZ)},
		Vertex{glm::vec3(20.0f, -7.5f, 20.0f), glm::vec3(1.0f), glm::vec3(0,1,0), glm::vec2(repeatX, 0.0f)}
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex{ textures[0] };

	Mesh* tile = new Mesh(verts, ind, tex); // dynamiczne tworzenie
	scene.AddTextureMesh(tile);
}


