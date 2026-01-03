#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/callbacks.h"
#include <iostream>
#include <filesystem>

#include "include/shaderClass.h"
#include "include/Model.h"
#include "include/Mesh.h"
#include "include/Scene.h"
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/Texture.h"
#include "include/Camera.h"
#include "include/DrawingHelper.h"


using namespace std;

int main(void)
{
	GLFWwindow* window;
	GLFWmonitor* monitor;
	Scene scene;
	DrawingHelper drawingHelper;
	int screenWidth = 1920;
	int screenHeight = 1080;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// SET THE ERROR CALLBACK
	glfwSetErrorCallback(error_callback);

	//Setup the monitor
	monitor = glfwGetPrimaryMonitor();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(screenWidth, screenHeight, "Liszaj", NULL, NULL);


	// IF window is not created, terminate
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	//CALLBACKS
	glfwSetKeyCallback(window, key_callback);


	////////////////////////////////////////////////////////////////////////////////////
	///                        TEXTURES  & SHADERS                                   ///
	////////////////////////////////////////////////////////////////////////////////////
	string shaderDir = filesystem::current_path().string() + "/src/shaders/";
	string texturteDir = filesystem::current_path().string() + "/src/textures/";
	string modelDir = filesystem::current_path().string() + "/src/models/";

	Shader textureShader = drawingHelper.setupShaderProgram(shaderDir, "default");



	Texture textures[]
	{
		Texture((texturteDir + "floor_tile.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture((texturteDir + "wall.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
	};



	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////


	drawingHelper.floorTiles(textures, scene);
	drawingHelper.spawnArea(textures, scene);


	glm::vec3 lightPos = glm::vec3(50.0f, 50.0f, 50.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // bia³e œwiat³o

	// --- MODEL ---
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	// --- Ustawienia shaderów ---
	textureShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform3f(glGetUniformLocation(textureShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(textureShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform1i(glGetUniformLocation(textureShader.ID, "useTexture"), false);


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	//SETUP THE MODEL



	Model model((modelDir + "Human.OBJ").c_str());

	std::cout << "Meshes loaded: " << model.meshes.size() << std::endl;
	if (!model.meshes.empty())
	{
		std::cout << "Vertices in first mesh: " << model.meshes[0].vertices.size() << std::endl;
		std::cout << "Indices in first mesh: " << model.meshes[0].indices.size() << std::endl;
	}


	// Creates camera object
	Camera camera(screenWidth, screenHeight, glm::vec3(0.5f, 1.6f, 0.5f));

	glm::mat4 modelMat = glm::mat4(1.0f);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draws different meshes
		drawingHelper.drawScene(textureShader, scene, camera);
		glUniform1i(glGetUniformLocation(textureShader.ID, "useTexture"), false);
		model.Draw(textureShader, camera, modelMat);

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