// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"


int createVAO(float* vertices, int* indices, int verticeLength, int indiceLength);

void runProgram(GLFWwindow* window)
{
	// Enable depth (Z) buffer (accept "closest" fragment)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Configure miscellaneous OpenGL settings
	glEnable(GL_CULL_FACE);

	// Set default colour after clearing the colour buffer
	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	// Loading and enabling shaders
	Gloom::Shader shader;
	
	// Task 1b
	std::string pathVert = "C:\\Users\\Ilker\\Documents\\Informatik\\Erasmus\\computervision\\gloom\\gloom\\shaders\\simple.vert";
	std::string pathFrag = "C:\\Users\\Ilker\\Documents\\Informatik\\Erasmus\\computervision\\gloom\\gloom\\shaders\\simple.frag";

	shader.makeBasicShader(pathVert, pathFrag);
	shader.activate();

	// Set up your scene here (create Vertex Array Objects, etc.)

	// Defining 5 triangles
	//Task 1c
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

	float singleVertice[] = {
	  0.6f, -0.8f, -1.2f,
	  0.0f, 0.4f, 0.0f,
	  -0.8f, -0.2f, 1.2f
	};

	int indicesSingleVertice[] = {0, 1, 2};

	// Task 2b
	// unnötig
	float verticesManipulatedIndexBuffer[] = {
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


	int indicesManipulatedIndexBuffer[] = {
		0, 1, 2,
		3, 4, 5,
		8, 7, 6, // Manipulated for task 2b
		9, 10, 11,
		12, 13, 14
	};

	// 5 triangles
	//int vao = createVAO(vertices, indices, 15 * 3, 15);

	// single triangle task 2a
	//int vao = createVAO(singleVertice, indicesSingleVertice, 9, 3);

	// Task 2b
	int vao = createVAO(verticesManipulatedIndexBuffer, indicesManipulatedIndexBuffer, 15 * 3, 15);

	// Rendering Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw your scene here
		glBindVertexArray(vao);

		// single triangle
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		
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
	}
}

// Task 1a
int createVAO(float* vertices, int* indices, int verticeLength, int indiceLength) {
	// Creating and binding VAO
	unsigned int arrayID = 0;
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);

	// Defining and binding Vertex Buffer Object
	unsigned int bufferID = 0;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Transfering Data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeLength, vertices, GL_STATIC_DRAW);

	// Format specification of the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enabling the inputs for Vertex Buffer Object
	glEnableVertexAttribArray(0);

	// Creating Index Buffer
	unsigned int bufferID2 = 0;
	glGenBuffers(1, &bufferID2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indiceLength, indices, GL_STATIC_DRAW);

	return arrayID;
}