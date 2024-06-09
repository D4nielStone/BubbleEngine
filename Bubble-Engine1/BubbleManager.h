#ifndef BUBBLE_MANAGER_H
#define BUBBLE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "includes.h"
#include "GameObject.h"

class BubbleManager
{
private:
public:
	bool isRunning = false;
	GLFWwindow* glfwWindow;
	unsigned int shaderProgram, VAO;
	bool bubbleInitGlfw();
	void renderOpengl(GameObject* go);
};
#endif

