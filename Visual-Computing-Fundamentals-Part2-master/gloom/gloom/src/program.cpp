#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::mat4x4 ModelViewProjection;
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelMatrix(1.0);
glm::vec3 up_vector(0, 1, 0);

const float scaleFactor = 0.01f;

// direction keys
#define GLFW_KEY_W   87 // W
#define GLFW_KEY_A   65 // A
#define GLFW_KEY_S   83 // S
#define GLFW_KEY_D   68 // D

// height keys
#define GLFW_KEY_M   77 // M
#define GLFW_KEY_N   78 // N

// camera keys
#define GLFW_KEY_UP   265 // arrow up
#define GLFW_KEY_DOWN   264 // arrow down
#define GLFW_KEY_LEFT   263 // arrow left
#define GLFW_KEY_RIGHT   262 // arrow right

// saving camera coordinates
struct {
	float x = 0.0f;
	float y = 0.0f;
	float z = 1.0f;
	float horizontal = 0.0f;
	float vertical = 0.0f;
} camPosition;

int createVAO(float* vertices, int* indices, int verticeLength, int indiceLength, float* color, int colorLength);
void updateMVP();

void runProgram(GLFWwindow* window)
{
	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Configure miscellaneous OpenGL settings
	glEnable(GL_CULL_FACE);

	// Set default colour after clearing the colour buffer
	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	//Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Loading and enabling shaders
	Gloom::Shader shader;

	std::string pathVert = "C:\\Users\\gwmeb\\source\\repos\\Graphics2\\gloom\\gloom\\shaders\\simple.vert";
	std::string pathFrag = "C:\\Users\\gwmeb\\source\\repos\\Graphics2\\gloom\\gloom\\shaders\\simple.frag";

	shader.makeBasicShader(pathVert, pathFrag);
	shader.activate();

	// Set up your scene here (create Vertex Array Objects, etc.)
	//task 1a
	float vertices[] = {
		// z-coordinate must be same
		// Point 0/0/x = midpoint of the frame [-1, 1]

		-1.0f, 0.0f, -0.6f,
		-0.80f, 0.0f, -0.6f,
		-0.90f, 0.5f, -0.6f,

		-0.7f, 0.0f, -0.6f,
		-0.5f, 0.0f, -0.6f,
		-0.6f, 0.5f, -0.6f,

		-0.5f, 0.0f, -0.6f,
		-0.3f, 0.0f, -0.6f,
		-0.4f, 0.5f, -0.6f,

		-0.3f, 0.0f, -0.6f,
		-0.1f, 0.0f, -0.6f,
		-0.2f, 0.5f, -0.6f,

		0.0f, 0.0f, -0.6f,
		0.8f, 0.0f, -0.6f,
		0.4f, 0.5f, -0.6f
	};


	int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.15f, 0.17f, 0.0f, 1.0f,
		0.5f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,

		0.15f, 0.4f, 0.7f, 1.0f,
		0.1f, 0.0f, 0.8f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.35f, 0.6f, 0.9f, 0.8f,
		0.8f, 0.8f, 0.6f, 0.6f,
		0.27f, 0.22f, 0.33f, 0.4f,

		1.0f, 0.5f, 0.0f, 1.0f,
		0.95f, 0.45f, 0.3f, 0.8f,
		0.90f, 0.40f, 0.6f, 0.6f,
	};


	// Assignment 2 Task 2

	float vertices2[] = {
		// z-coordinate must be same
		// Point 0/0/x = midpoint of the frame [-1, 1]

		-1.0f, 0.0f, -0.8f,
		-0.80f, 0.0f, -0.8f,
		-0.90f, 0.8f, -0.8f,

		-1.0f, 0.0f, -0.7f,
		-0.7f, 0.0f, -0.7f,
		-0.8f, 0.5f, -0.7f,

		-1.0f, 0.0f, -0.6f,
		-0.6f, 0.0f, -0.6f,
		-0.7f, 0.2f, -0.6f,

		-0.3f, 0.0f, -0.6f,
		-0.1f, 0.0f, -0.6f,
		-0.2f, 0.5f, -0.6f,

		0.0f, 0.0f, -0.6f,
		0.8f, 0.0f, -0.6f,
		0.4f, 0.5f, -0.6f
	};

	float colors2[] = {
		0.6f, 0.0f, 0.2f, 0.4f,
		0.6f, 0.0f, 0.2f, 0.4f,
		0.6f, 0.0f, 0.2f, 0.4f,

		0.0f, 0.6f, 0.0f, 0.4f,
		0.0f, 0.6f, 0.0f, 0.4f,
		0.0f, 0.6f, 0.0f, 0.4f,

		0.0f, 0.0f, 0.6f, 0.4f,
		0.0f, 0.0f, 0.6f, 0.4f,
		0.0f, 0.0f, 0.6f, 0.4f,

		0.35f, 0.6f, 0.9f, 0.8f,
		0.8f, 0.8f, 0.6f, 0.6f,
		0.27f, 0.22f, 0.33f, 0.4f,

		1.0f, 0.5f, 0.0f, 1.0f,
		0.95f, 0.45f, 0.3f, 0.8f,
		0.90f, 0.40f, 0.6f, 0.6f,
	};

	projectionMatrix = glm::perspective((90.0f * 3.1415f) / 180, 1366.0f / 768.0f, 0.1f, 100.0f);

	// 5 triangles
	//int vao = createVAO(vertices, indices, 15 * 3, 15, colors, 15 * 4);
	//int vao = createVAO(vertices, indices, sizeof(vertices), sizeof(indices), colors, sizeof(colors));

	int vao = createVAO(vertices2, indices, sizeof(vertices2), sizeof(indices), colors2, sizeof(colors2));
	float counter = 0.0f;

	updateMVP();


	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{

		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//task 3a
		glUniform1f(2, sin(counter / 1000));
		counter++;

		//sending the current view matrix to vertex shader
		glUniformMatrix4fv(3, 1, false, glm::value_ptr(ModelViewProjection));

		// Draw your scene here
		glBindVertexArray(vao);

		// 5 triangles
		glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

		// Handle other events
		glfwPollEvents();
		handleKeyboardInput(window);

		// Flip buffers
		glfwSwapBuffers(window);
	}
}


void handleKeyboardInput(GLFWwindow* window)
{
	// Use escape key for terminating the GLFW window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	} // direction forward
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camPosition.z -= scaleFactor;
	} // direction backwards
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camPosition.z += scaleFactor;
	} // direction left 
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camPosition.x -= scaleFactor;
	} // direction right
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camPosition.x += scaleFactor;
	} // height up
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		camPosition.y += scaleFactor;
	} // height down
	else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		camPosition.y -= scaleFactor;
	} // camera up
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camPosition.vertical -= scaleFactor;
	} // camera down
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camPosition.vertical += scaleFactor;
	} // camera left rotate
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camPosition.horizontal -= scaleFactor;
	} // camera right rotate
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camPosition.horizontal += scaleFactor;
	}

	updateMVP();
}

int createVAO(float* vertices, int* indices, int verticeLength, int indiceLength, float* color, int colorLength) {
	// Creating and binding VAO
	unsigned int arrayID = 0;
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);

	// Defining and binding Vertex Buffer Object
	unsigned int bufferID = 0;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Transfering Data
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeLength, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, verticeLength, vertices, GL_STATIC_DRAW);

	// Format specification of the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enabling the inputs for Vertex Buffer Object
	glEnableVertexAttribArray(0);

	// Creating Index Buffer
	unsigned int bufferID2 = 0;
	glGenBuffers(1, &bufferID2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indiceLength, indices, GL_STATIC_DRAW);


	// Color for each vertices
	unsigned int colorID = 0;
	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, colorLength, color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Enabling the inputs for Vertex Buffer Object
	glEnableVertexAttribArray(1);

	return arrayID;
}

void updateMVP() {
	// move to origin point
	glm::mat4 viewOrigin = glm::translate(glm::vec3(-camPosition.x, -camPosition.y, -camPosition.z));
	// Rotate world horizontally
	glm::mat4 viewRotateHorizontal = glm::rotate(camPosition.horizontal, up_vector);
	// Rotate world vertically
	glm::mat4 viewRotateVertical = glm::rotate(camPosition.vertical, glm::vec3(1.0, 0.0, 0.0));  
	// combined view matrix 
	glm::mat4 view = viewRotateVertical * viewRotateHorizontal * viewOrigin;

	ModelViewProjection = projectionMatrix * view * modelMatrix;

}