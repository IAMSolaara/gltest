//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "LearnOpenGL"

//vertices for our lil coote triangle👉👈
float vertices[] = {
	//big rect
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // top left 
	//small tri
//	 0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
//	 0.8f,  0.7f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
	0,1,3,	//first  tri
	1,2,3,	//second tri
//	4,5,6,  //third tri
};

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
float w = 1.0f;

//callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//custom funcs
void inputHandler(GLFWwindow* window);
unsigned int compileVertexShader();
unsigned int compileFragmentShader();
unsigned int createShaderProgram();

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

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

	//generate vertex buffer object and vertex array object
	unsigned int VBO;
	unsigned int VAO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//generate element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//bind VAO to vertex arrays.
	glBindVertexArray(VAO);

	//bind VBO buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//transfer verts array into VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind EBO to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex coords position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

        //create shader program to render poly
	unsigned int shaderProgram = createShaderProgram();

	//main render loop
	while (!glfwWindowShouldClose(window)) {
		//clear to dark aqua-ish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//poll events
		glfwPollEvents();
		
		//handle input events
		inputHandler(window);

		//draw triangle
		glUseProgram(shaderProgram);

		//get `coolColor` uniform location to inject r,g,b,w vars into
		//int offsetLocation = glGetUniformLocation(shaderProgram, "coolColor");
		//glUniform4f(offsetLocation, r, g, b, w);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
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

	//	raise red
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) {
		r = clamp(r+0.01f, 0.0f, 1.0f);
	}

	//	lower red
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		r = clamp(r-0.01f, 0.0f, 1.0f);
	}

	//	raise blue
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		b = clamp(b+0.01f, 0.0f, 1.0f);
	}

	//	lower blue
	if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
		b = clamp(b-0.01f, 0.0f, 1.0f);
	}

	//	raise green
	if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
		g = clamp(g+0.01f, 0.0f, 1.0f);
	}

	//	lower green
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		g = clamp(g-0.01f, 0.0f, 1.0f);
	}
}

//wrapper function to handle compilation of vertex shader.
//done this way to "keep main code clean".
unsigned int compileVertexShader(const std::string path) {
	unsigned int vertexShader = -1;

	std::ifstream sourceFile(path);

	if (sourceFile.is_open()) {
		std::stringstream sourceStringStream;
		sourceStringStream << sourceFile.rdbuf();
		
		std::string sourceString = sourceStringStream.str();

		const char* source = sourceString.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n%s\n", infoLog);
		}

	}

	return vertexShader;
}

//wrapper function to handle compilation of fragment shader.
//done this way to "keep main code clean".
unsigned int compileFragmentShader(const std::string path) {
	unsigned int fragmentShader = -1;

	std::ifstream sourceFile(path);

	if (sourceFile.is_open()) {
		std::stringstream sourceStringStream;
		sourceStringStream << sourceFile.rdbuf();

		std::string sourceString = sourceStringStream.str();

		const char* source = sourceString.c_str();

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n%s\n", infoLog);
		}
	}

	return fragmentShader;
}

//wrapper function to create our shader program
unsigned int createShaderProgram() {
	unsigned int vertexShader = compileVertexShader("./res/shaders/vertex/simple.vs");
	unsigned int fragmentShader = compileFragmentShader("./res/shaders/fragment/simple.fs");

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success; char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "GL Program Link Error:\n%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
