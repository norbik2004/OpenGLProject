#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/Mesh.h"
#include "include/Scene.h"
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Texture.h"
#include "include/Camera.h"
#include "include/DrawingHelper.h"
#include "include/Engine.h"
#include "include/Model.h"

#include <vector>
#include <queue>
#include <chrono>
#include <cmath>
#include <algorithm>

struct InfectionController
{
    Mesh* mesh = nullptr;
    std::vector<size_t> infectionQueue; // kolejnoœæ trójk¹tów do zarazenia
    size_t currentIndex = 0;

    float trianglesPerUpdate = 500.0f; // ile trójk¹tów kolorujemy na update
    float updateInterval = 0.5f;       // czas w sekundach miêdzy update
    glm::vec3 startPoint;               // punkt startowy infekcji
    std::chrono::steady_clock::time_point lastUpdate;

    void init(Mesh& m)
    {
        mesh = &m;

        // punkt startowy = œrodek modelu
        startPoint = glm::vec3(0.0f);
        for (auto& v : m.vertices) startPoint += v.position;
        startPoint /= (float)m.vertices.size();

        // przygotuj kolejkê wszystkich trójk¹tów
        size_t triCount = m.indices.size() / 3;
        infectionQueue.resize(triCount);
        for (size_t i = 0; i < triCount; ++i) infectionQueue[i] = i;

        // sortowanie wg odleg³oœci œredniego punktu trójk¹ta od startPoint
        std::sort(infectionQueue.begin(), infectionQueue.end(), [&m, this](size_t a, size_t b) {
            glm::vec3 ca = (m.vertices[m.indices[a * 3 + 0]].position +
                m.vertices[m.indices[a * 3 + 1]].position +
                m.vertices[m.indices[a * 3 + 2]].position) / 3.0f;
            glm::vec3 cb = (m.vertices[m.indices[b * 3 + 0]].position +
                m.vertices[m.indices[b * 3 + 1]].position +
                m.vertices[m.indices[b * 3 + 2]].position) / 3.0f;
            float da = glm::distance(ca, startPoint);
            float db = glm::distance(cb, startPoint);
            return da < db;
            });

        currentIndex = 0;
        lastUpdate = std::chrono::steady_clock::now();
    }

    void update()
    {
        if (!mesh || currentIndex >= infectionQueue.size()) return;

        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - lastUpdate).count();

        if (elapsed < updateInterval) return;

        size_t trianglesToColor = std::min((size_t)trianglesPerUpdate, infectionQueue.size() - currentIndex);

        for (size_t i = 0; i < trianglesToColor; ++i)
        {
            size_t t = infectionQueue[currentIndex++];
            for (size_t j = 0; j < 3; ++j)
            {
                GLuint vIdx = mesh->indices[t * 3 + j];
                mesh->vertices[vIdx].color = glm::vec3(0.6f, 0.7f, 0.4f);
            }
        }

        mesh->UpdateVertices({});

        lastUpdate = now;
    }

    bool finished() const { return currentIndex >= infectionQueue.size(); }
};
using namespace std;

int main(void)
{
	Engine engine;
	Scene scene;
	DrawingHelper drawingHelper;

	if (!engine.initWindow(glfwGetPrimaryMonitor(), "Liszaj"))
		return -1;

	GLFWwindow* window = engine.getWindow();
	Shader textureShader = drawingHelper.setupShaderProgram(filesystem::current_path().string() + "/src/shaders/", "default");

	drawingHelper.floorTiles(scene);

	Model* human = drawingHelper.humanModel();
	Camera camera(engine.getSreenWidth(), engine.getSreenHeight(), glm::vec3(0.5f, 1.6f, 0.5f));

	InfectionController infection;
    infection.init(human->meshes[0]);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		infection.update();
		// Draws different meshes
		drawingHelper.drawScene(textureShader, scene, camera, human);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	textureShader.Delete();
	for (auto mesh : scene.textureMeshes) {
		scene.RemoveTextureMesh(mesh);
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}