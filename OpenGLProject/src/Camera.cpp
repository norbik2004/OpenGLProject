#include "../include/Camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 forward = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
		Position += speed * forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
		right.y = 0.0f;
		Position -= speed * glm::normalize(right);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 forward = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
		Position -= speed * forward;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
		right.y = 0.0f;
		Position += speed * glm::normalize(right);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.05f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.009f;
	}


	// Handles mouse inputs
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Prevents camera from jumping on start
	if (firstClick)
	{
		glfwSetCursorPos(window, (width / 2), (height / 2));
		firstClick = false;
	}

	// Get cursor position
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Calculate rotation
	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// Vertical rotation (pitch)
	glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), right);

	// Clamp pitch
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	// Horizontal rotation (yaw)
	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	// Center mouse
	glfwSetCursorPos(window, (width / 2), (height / 2));



}