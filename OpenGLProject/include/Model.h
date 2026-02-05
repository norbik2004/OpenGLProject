#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Camera.h"

class Model
{
public:

	Model() = default;
	explicit Model(const std::string& path) { loadFromFile(path); }

	void loadFromFile(const std::string& path);

	// draw with shader and camera
	void Draw(Shader& shader, Camera& camera, const glm::mat4& modelMatrix = glm::mat4(1.0f));
	std::vector<Mesh>& getMeshes() { return meshes; }

private:
	std::vector<Mesh> meshes;
};

#endif