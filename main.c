//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "LearnOpenGL"

//callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//custom funcs
void inputHandler(GLFWwindow* window);

int main() {
	//initialize glfw
	glfwInit();

	//set OGL version hints so glfw will work only on supported versions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//use core profile because we're modern chads
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//macOS required workaround
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//create a SCREEN_WIDTHxSCREEN_HEIGHT window with title WINDOW_TITLE, then make it the current OGL context.
	//exit if errored
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window) {
		fprintf(stderr, "Error creating GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//set up GLAD function loader
	if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress )) {
		fprintf(stderr, "Error initializing GLAD.\n");
		return -1;
	}

	//set viewport start coords and width,height.
	//in this case it'll have the whole window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//set resize callback to handle window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//main render loop
	while (!glfwWindowShouldClose(window)) {
		//poll events
		glfwPollEvents();
		
		//handle input events
		inputHandler(window);
		
		//swap buffers to present to screen
		glfwSwapBuffers(window);
	}

	//kill glfw
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}

void inputHandler(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}
}
