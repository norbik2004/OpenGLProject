#include "../include/DrawingHelper.h"
#include "../include/Model.h"
#include <filesystem>
using namespace std;

Shader DrawingHelper::setupShaderProgram(const std::string& shaderDir, const std::string& name) const
{
	// Œcie¿ki do plików vertex + fragment
	std::string vertexPath = shaderDir + name + ".vert";
	std::string fragmentPath = shaderDir + name + ".frag";

	// Tworzymy shader i zwracamy
	Shader shaderProgram(vertexPath.c_str(), fragmentPath.c_str());

	glm::vec3 lightPos = glm::vec3(0.0f, 100.0f, 0.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // bia³e œwiat³o

	// --- MODEL ---
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	// --- Ustawienia shaderów ---
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform3f(glGetUniformLocation(shaderProgram.getID(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram.getID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform1i(glGetUniformLocation(shaderProgram.getID(), "useTexture"), false);
	return shaderProgram;
}

void DrawingHelper::drawScene(Shader& textureShader, Scene& scene, Camera& camera, Model* human)
{
	glClearColor(0.53f, 0.68f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1f(glGetUniformLocation(textureShader.getID(), "uScale"), 1.0f);
	glUniform1i(glGetUniformLocation(textureShader.getID(), "useTexture"), true);

	if (!scene.getTextureMeshes().empty()) {
		for (Mesh* m : scene.getTextureMeshes())
		{
			m->Draw(textureShader, camera);
		}
	}

	glUniform1i(glGetUniformLocation(textureShader.getID(), "useTexture"), false);
	if (human)
		human->Draw(textureShader, camera, glm::mat4(1.0f));
}


void DrawingHelper::floorTiles(Scene& scene)
{
	Texture textures[]
	{
		Texture((filesystem::current_path().string() + "/src/textures/" + "floor_tile.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((filesystem::current_path().string() + "/src/textures/" + "wall.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
	};

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

Model* DrawingHelper::humanModel()
{
	std::string path = filesystem::current_path().string() + "/src/models/Human.OBJ";

	Model* model = new Model(path.c_str());

	std::cout << "Meshes loaded: " << model->getMeshes().size() << std::endl;
	if (!model->getMeshes().empty())
	{
		std::cout << "Vertices in first mesh: " << model->getMeshes()[0].getVertices().size() << std::endl;
		std::cout << "Indices in first mesh: " << model->getMeshes()[0].getIndices().size() << std::endl;
	}

	return model;
}


