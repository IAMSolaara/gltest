//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

//global includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local includes
#include <stb_image/stb_image.h>
#include <shader.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "LearnOpenGL"

#define RESPATH "../res"

//vertices for our lil coote triangle👉👈
float vertices[] = {
	//big rect
	//x     y     z     r     g     b     s     t
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left 
	//small tri
//	 0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
//	 0.8f,  0.7f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
	0,1,3,	//first  tri
	1,2,3,	//second tri
	//4,5,6,  //third tri
};

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
float w = 1.0f;

float angle = 0.0f;
float scale = 0.4f;
float scaleInc = 0.004f;
float rotInc = 2.0f;

//callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//custom funcs
void inputHandler(GLFWwindow* window);
void errorHandler(int ec, const char* msg);

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

int main() {
	//initialize glfw
	glfwInit();

	glfwSetErrorCallback(errorHandler);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set vertex texture position attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    //create shader program to render poly
	Shader myShader(RESPATH"/shaders/vertex/simple.vs", RESPATH"/shaders/fragment/simple.fs");

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int w,h,nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *textureData = stbi_load(RESPATH"/textures/cock.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	myShader.use();
	myShader.setInt("mytexture", 0);

	//main render loop
	while (!glfwWindowShouldClose(window)) {
		//clear to dark aqua-ish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//poll events
		glfwPollEvents();
		
		//handle input events
		inputHandler(window);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));
		myShader.setMat4("transform", trans);

		//draw triangle
		myShader.use();

		//get `coolColor` uniform location to inject r,g,b,w vars into
		//int offsetLocation = glGetUniformLocation(shaderProgram, "coolColor");
		//glUniform4f(offsetLocation, r, g, b, w);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		//swap buffers to present to screen
		glfwSwapBuffers(window);

		angle += rotInc;
		scale += scaleInc;
		if (scale >= 2.0f || scale <= 0.4f) scaleInc *= -1.0f;
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		rotInc *= -1.0f;
	}
}

void errorHandler(int ec, const char* msg) {
	printf("GLFW ERROR (Code %d): %s\n", ec, msg);
}
