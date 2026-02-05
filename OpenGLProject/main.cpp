#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/Mesh.h"
#include "include/Scene.h"
#include "include/InfectionController.h"
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Texture.h"
#include "include/Camera.h"
#include "include/DrawingHelper.h"
#include "include/Engine.h"
#include "include/Model.h"

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
	engine.showSplash(filesystem::current_path().string() + "/src/textures/splashScreen.png");

	drawingHelper.floorTiles(scene);

	Model* human = drawingHelper.humanModel();
	Camera camera(engine.getSreenWidth(), engine.getSreenHeight(), glm::vec3(0.0f, -5.5f, 20.0f));
	

	//start of infection
	InfectionController infection;
    infection.init(human->getMeshes()[0]);
	glfwSetWindowUserPointer(window, &infection);
	float lastFrame = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		infection.update(deltaTime);

		std::string title = "Liszaj | "
			"Green: " + std::to_string(infection.greenIndex) + "/" + std::to_string(infection.infectionQueue.size()) +
			(infection.greenActive ? " (Active)" : " (Stopped)") + " | "
			"Blue: " + std::to_string(infection.blueIndex) + "/" + std::to_string(infection.infectionQueue.size()) +
			(infection.blueActive ? " (Active)" : " (Stopped)") + " | "
			"Triangles/update: " + std::to_string((int)infection.trianglesPerUpdate) + " | "
			"Update interval: " + std::to_string(infection.updateInterval) + "s" + " | "
			"Status: " + (infection.paused ? "Paused" : "Running");

		glfwSetWindowTitle(window, title.c_str());

		// Draws different meshes
		drawingHelper.drawScene(textureShader, scene, camera, human);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	textureShader.Delete();
	for (auto mesh : scene.getTextureMeshes()) {
		scene.RemoveTextureMesh(mesh);
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}