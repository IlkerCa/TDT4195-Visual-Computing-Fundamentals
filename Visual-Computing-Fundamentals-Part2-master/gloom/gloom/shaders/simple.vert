#version 430 core

out vec4 fragmentColor;

layout(location=0) in vec3 position;
layout(location=1) in vec4 vertexColor;

uniform layout(location=2) float variable;
uniform layout(location=3) mat4x4 view;


void main()
{
	// task 3a
	mat4x4 matrix;
	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;

	//task 3b
	// A
	//matrix[0][0] = variable;
	// B
	//matrix[1][0] = variable;
	// C	
	//matrix[3][0] = variable;
	// D
	//matrix[0][1] = variable;
	// E
	//matrix[1][1] = variable;
	// F
	//matrix[3][1] = variable;

	// task 3a/b
	// mat * vector
	//gl_Position = matrix * vec4(position, 1.0f);

	//task 4a
	gl_Position = view * vec4(position, 1.0f);

	fragmentColor = vertexColor;
}
