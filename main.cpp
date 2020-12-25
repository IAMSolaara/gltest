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
#include <camera.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "LearnOpenGL"

#define RESPATH "./res"

//vertices for our lil coote triangle👉👈
float vertices[] = {
	//big rect
	//x     y     z      r     g     b     s     t
//face3
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face1
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face4
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face5                                              
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face2
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face6
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
	//small tri
//	 0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
//	 0.8f,  0.7f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
	0,1,3,	//first  tri
	0,2,3,	//second tri

	4,5,7,	//first  tri
	4,6,7,	//second tri
	
	8,9,11,
	8,10,11,
	
	12,13,15,
	12,14,15,

	16,17,19,
	16,18,19,

	20,21,23,
	20,22,23,
	//4,5,6,  //third tri
};

float tex2Vertices[] = {
	 0.5f,  0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

unsigned int tex2Indexes[] = {
	0, 1, 3,
	0, 2, 3,
};

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
float w = 1.0f;

float angle = 0.0f;
float scale = 0.4f;
float scaleInc = 0.004f;
float angleInc = 2.0f;

//bool autoRotate = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = (float)SCREEN_WIDTH / 2;
float lastY = (float)SCREEN_HEIGHT / 2;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_pos_callback);

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

	//generate vertex buffer object and vertex array object
	unsigned int VBO2;
	unsigned int VAO2;

	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);

	//generate element buffer object
	unsigned int EBO2;
	glGenBuffers(1, &EBO2);

	//bind VAO2 to vertex arrays.
	glBindVertexArray(VAO2);

	//bind VBO2 buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	//transfer verts array into VBO2 buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex2Vertices), tex2Vertices, GL_STATIC_DRAW);

	//bind EBO2 to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tex2Indexes), tex2Indexes, GL_STATIC_DRAW);

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
	//Shader myShader2(RESPATH"/shaders/vertex/simple.vs", RESPATH"/shaders/fragment/simple.fs");

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int w,h,nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *textureData = stbi_load(RESPATH"/textures/sburnias.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
		int wQ, hQ;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
		printf("SBI Size:\t%d\t%d\n",w,h);
		printf("Queried Size\t%d\t%d\n",wQ,hQ);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureData = stbi_load(RESPATH"/textures/wall.jpg", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
		int wQ, hQ;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
		printf("SBI Size:\t%d\t%d\n",w,h);
		printf("Queried Size\t%d\t%d\n",wQ,hQ);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	myShader.use();

	glEnable(GL_DEPTH_TEST);
	
	//main render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		//clear to dark aqua-ish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//poll events
		glfwPollEvents();
		
		//handle input events
		inputHandler(window);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, glm::radians(angle * 0.3f), glm::vec3(0.0, 1.0, 0.0));
		trans = glm::rotate(trans, glm::radians(angle + 2.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		myShader.setMat4("transform", trans);
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		myShader.setInt("mytexture", 0);

		//draw triangle
		myShader.use();

		//get `coolColor` uniform location to inject r,g,b,w vars into
		//int offsetLocation = glGetUniformLocation(shaderProgram, "coolColor");
		//glUniform4f(offsetLocation, r, g, b, w);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		myShader.setMat4("transform", glm::mat4(1.0f));


		myShader.setInt("mytexture", 1);

		myShader.use();

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		//swap buffers to present to screen
		glfwSwapBuffers(window);

		
		angle += angleInc;
		//scale += scaleInc;
		//if (scale >= 1.3f || scale <= 0.4f) scaleInc *= -1.0f;
		
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

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

	//	lower red
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		//angle -= angleInc;
	}

	//	lower green
	if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
		//autoRotate = !autoRotate;
	}

	//	lower green
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		//angle += angleInc;
	}

	//	lower green
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		//scale = clamp(scale+0.006f, 0.4f, 2.0f);
	}

	//	lower green
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		//scale = clamp(scale-0.006f, 0.4f, 2.0f);
	}
	
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//angleInc *= -1.0f;
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void errorHandler(int ec, const char* msg) {
	printf("GLFW ERROR (Code %d): %s\n", ec, msg);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoff = xpos - lastX;
	float yoff = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoff, yoff);
}
