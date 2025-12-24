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

	// --- WALL SEGMENT ----------------------------------------------------

	Vertex wallsVerts[] =
	{
		// tylna
		{ {0, 0, 0},   {1,1,1}, {0,0,1}, {0, 3} },
		{ {0, 3, 0},   {1,1,1}, {0,0,1}, {0, 0} },
		{ {10, 3, 0},  {1,1,1}, {0,0,1}, {10, 0} },
		{ {10, 0, 0},  {1,1,1}, {0,0,1}, {10, 3} },
		// prawa
		{ {10, 3, 0},   {1,1,1}, {0,0,1}, {0, 0} },
		{ {10, 0, 0},   {1,1,1}, {0,0,1}, {0, 3} },
		{ {10, 0, 10},  {1,1,1}, {0,0,1}, {10, 3} },
		{ {10, 3, 10},  {1,1,1}, {0,0,1}, {10, 0} },
		// lewa
		{ {0, 3, 0},   {1,1,1}, {0,0,1}, {0, 0} },
		{ {0, 0, 0},   {1,1,1}, {0,0,1}, {0, 3} },
		{ {0, 0, 10},  {1,1,1}, {0,0,1}, {10, 3} },
		{ {0, 3, 10},  {1,1,1}, {0,0,1}, {10, 0} },

	};

	GLuint wallIndiecies[] =
	{
		// tylna
		0,1,2,
		0,2,3,
		// prawa
		4, 5, 6,
		4, 6, 7,
		8,9,10,
		8,10,11
	};

	std::vector<Vertex> verts(wallsVerts, wallsVerts + sizeof(wallsVerts) / sizeof(Vertex));
	std::vector<GLuint> indicies(wallIndiecies, wallIndiecies + sizeof(wallIndiecies) / sizeof(GLuint));
	std::vector<Texture> tex = { textures[1] };

	Mesh* wall = new Mesh(verts, indicies, tex);
	scene.AddTextureMesh(wall);

}

