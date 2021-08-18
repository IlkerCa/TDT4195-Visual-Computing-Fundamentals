#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "OBJLoader.hpp"
#include "toolbox.hpp"
#include "sceneGraph.hpp"
#include <iostream>

glm::mat4x4 ModelViewProjection;
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelMatrix(1.0);
glm::vec3 up_vector(0, 1, 0);


SceneNode* chessBoard = createSceneNode();
SceneNode* sceneHead = createSceneNode();
SceneNode* sceneTorso = createSceneNode();
SceneNode* sceneLeftarm = createSceneNode();
SceneNode* sceneRightarm = createSceneNode();
SceneNode* sceneLeftleg = createSceneNode();
SceneNode* sceneRightleg = createSceneNode();

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
	float y = 15.0f;
	float z = 0.0f;
	float horizontal = 0.0f;
	float vertical = -300.0f;
} camPosition;

int createVAO(float* vertices, int* indices, int verticeLength, int indiceLength, float* color, int colorLength);
int createVAO(Mesh mesh);
void updateMVP();

// loading path
Path path = Path("/home/shomeb/g/gwenaelm/Desktop/visual/gloom/gloom/shaders/coordinates_2.txt");

void visitSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {

	// Do transformations here
	glm::vec3 position(1, 0, 0);
	glm::vec3 position2(0, 1, 0);

	if (node->vertexArrayObjectID >= 0){
		node->currentTransformationMatrix =
			transformationThusFar*
			glm::translate(glm::vec3(node->position.x, node->position.y, node->position.z))*
			glm::translate(glm::vec3(node->referencePoint.x, node->referencePoint.y, node->referencePoint.z)) *
			glm::rotate(node->rotation.y, position2) *
			glm::rotate(node->rotation.x, position) *
			glm::translate(glm::vec3(-node->referencePoint.x, -node->referencePoint.y, -node->referencePoint.z));

	// Do rendering here
	// Draw your scene here
	glBindVertexArray(node->vertexArrayObjectID);
	glUniformMatrix4fv(4, 1, false, glm::value_ptr(node->currentTransformationMatrix));
	glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0);
	}
		
	for (SceneNode* child : node->children) {
		visitSceneNode(child, node->currentTransformationMatrix);
	}
}

// Function that updates the referencePoint
void updateReferencePoint() {
	sceneRightarm->referencePoint = float3(-6, 24, 0);
	sceneLeftarm->referencePoint = float3(6, 24, 0);
	sceneLeftleg->referencePoint = float3(4, 12, 0);
	sceneRightleg->referencePoint = float3(-4, 12, 0);
	sceneTorso->referencePoint = float3(0, 16 , 0);
	sceneHead->referencePoint = float3(0, 28, 0);
}


SceneNode* prepareSceneGraph(Mesh chessboardMesh, MinecraftCharacter steve) {
	int rightArmVao = createVAO(steve.rightArm);
	int leftArmVao = createVAO(steve.leftArm);
	int leftLegVao = createVAO(steve.leftLeg);
	int rightLegVao = createVAO(steve.rightLeg);
	int torsoVao = createVAO(steve.torso);
	int headVao = createVAO(steve.head);
	int chessVao = createVAO(chessboardMesh);

	SceneNode* root = createSceneNode();
	addChild(root, chessBoard);
	addChild(root, sceneHead);
	addChild(sceneHead, sceneTorso);
	addChild(sceneTorso, sceneLeftarm);
	addChild(sceneTorso, sceneRightarm);
	addChild(sceneTorso, sceneLeftleg);
	addChild(sceneTorso, sceneRightleg);
	float2 positions = path.getCurrentWaypoint(30.0);

	sceneRightarm->position = float3(0, 0, 0);
	sceneRightarm->rotation = float3(0,0,0);
	sceneRightarm->vertexArrayObjectID = rightArmVao;
	sceneRightarm->VAOIndexCount = steve.rightArm.indices.size();

	sceneLeftarm->position = float3(0, 0, 0);
	sceneLeftarm->rotation = float3(0,0,0);
	sceneLeftarm->vertexArrayObjectID = leftArmVao;
	sceneLeftarm->VAOIndexCount = steve.leftArm.indices.size();


	sceneLeftleg->position = float3(0, 0, 0);
	sceneLeftleg->rotation = float3(0,0,0);
	sceneLeftleg->vertexArrayObjectID = leftLegVao;
	sceneLeftleg->VAOIndexCount = steve.leftLeg.indices.size();

	sceneRightleg->position = float3(0, 0, 0);
	sceneRightleg->rotation = float3(0,0,0);
	sceneRightleg->vertexArrayObjectID = rightLegVao;
	sceneRightleg->VAOIndexCount = steve.rightLeg.indices.size();

	sceneTorso->position = float3(0, 0, 0);
	sceneTorso->rotation = float3(0,0,0);
	sceneTorso->vertexArrayObjectID = torsoVao;
	sceneTorso->VAOIndexCount = steve.torso.indices.size();

	sceneHead->position = float3(positions.x, 0, positions.y);
	sceneHead->rotation = float3(0,0,0);
	sceneHead->vertexArrayObjectID = headVao;
	sceneHead->VAOIndexCount = steve.head.indices.size();


	chessBoard->position = float3(0, 0, 0);
	chessBoard->referencePoint = float3(0, 0, 0);
	chessBoard->vertexArrayObjectID = chessVao;
	chessBoard->VAOIndexCount = chessboardMesh.indices.size();

	updateReferencePoint();
	return root;
}




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

	//std::string pathVert = "C://Users//gwmeb//source//repos//visual//gloom//gloom//shaders//simple.vert";
	//std::string pathFrag = "C://Users//gwmeb//source//repos//visual//gloom//gloom//shaders//simple.frag";

	std::string pathVert = "/home/shomeb/g/gwenaelm/Desktop/visual/gloom/gloom/shaders/simple.vert";
	std::string pathFrag = "/home/shomeb/g/gwenaelm/Desktop/visual/gloom/gloom/shaders/simple.frag";

	shader.makeBasicShader(pathVert, pathFrag);
	shader.activate();

	glm::mat4 matrixTopass = glm::mat4(1.0f);



	projectionMatrix = glm::perspective((90.0f * 3.1415f) / 180, 1366.0f / 768.0f, 0.1f, 200.0f);
	float counter = 0.0f;

	updateMVP();


	struct MinecraftCharacter steve = loadMinecraftCharacterModel("/home/shomeb/g/gwenaelm/Desktop/visual/gloom/gloom/shaders/steve.obj");

	/*
	unsigned int width,  // Width and height of the chessboard, measured in tiles
		unsigned int height,
		float tileWidth,     // Width and height of each tile, measured in units
		float4 tileColour1,  // Colours of the chessboard tiles.
		float4 tileColour2) */
	float4 color1 = float4(1.0, 1.0, 1.0, 1.0);
	float4 color2 = float4(0.0, 0.0, 0.0, 1.0);
	Mesh board = generateChessboard(7, 5, 30.0, color1, color2);



	SceneNode *root = prepareSceneGraph(board, steve);
	double time = getTimeDeltaSeconds();
	float num = 0.1f;



	float2 oldPoint, waypoint;
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

		//glBindVertexArray(chessVao);
		//glDrawElements(GL_TRIANGLES, chessBoard->VAOIndexCount, GL_UNSIGNED_INT, 0);




			if (sceneLeftleg->rotation.x >= 1.0f) {
				num = -0.1f;

			}
			else if (sceneLeftleg->rotation.x <= -1.0f) {
				num = 0.1f;

			}
			// Leg and arm walking movement
			sceneLeftleg->rotation.x = sceneLeftleg->rotation.x + num * time * 50;
			sceneLeftarm->rotation.x = sceneLeftarm->rotation.x - num * time * 50;
			sceneRightleg->rotation.x = sceneRightleg->rotation.x - num * time * 50;
			sceneRightarm->rotation.x = sceneRightarm->rotation.x + num * time * 50;

			//updating position
			waypoint = path.getCurrentWaypoint(30.0);

			if (sceneHead->position.x < waypoint.x)
			{
				sceneHead->position.x = sceneHead->position.x + 0.1f * time * 140;
			}

			else if (sceneHead->position.x > waypoint.x)
			{
				sceneHead->position.x = sceneHead->position.x - 0.1f * time * 140;
			}


			if (sceneHead->position.z < waypoint.y)
			{
				sceneHead->position.z = sceneHead->position.z + 0.1f * time * 140;
			}

			else if (sceneHead->position.z > waypoint.y)
			{
				sceneHead->position.z = sceneHead->position.z - 0.1f * time * 140;
			}


		time = getTimeDeltaSeconds();
		// Handle other events

		if (path.hasWaypointBeenReached(float2(sceneHead->position.x, sceneHead->position.z), 30.0)) {

			path.advanceToNextWaypoint();
			waypoint = path.getCurrentWaypoint(30.0);

			/// change the rotation

			glm::vec3 a = glm::vec3(waypoint.x - sceneHead->position.x, 0, waypoint.y - sceneHead->position.z);

			a = a.length() == 0 ? glm::normalize(a) : a;

			float angle = std::atan2(a.x, a.z);
			std::cout << angle << std::endl;
			sceneHead->rotation.y = angle;


		}
		matrixTopass = ModelViewProjection * matrixTopass;
		visitSceneNode(root, matrixTopass);

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

int createVAO(Mesh mesh) {
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
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float4), mesh.vertices.data(), GL_STATIC_DRAW);

	// Format specification of the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float4), nullptr);

	// Enabling the inputs for Vertex Buffer Object
	glEnableVertexAttribArray(0);

	// Creating Index Buffer
	unsigned int bufferID2 = 0;
	glGenBuffers(1, &bufferID2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);


	// Color for each vertices
	unsigned int colorID = 0;
	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, mesh.colours.size() * sizeof(float4), mesh.colours.data(), GL_STATIC_DRAW);

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

	ModelViewProjection = projectionMatrix * view * modelMatrix * glm::scale(glm::vec3(0.1, 0.1, 0.1));
}
