#version 430 core

in vec4 fragmentColor;
out vec4 color;

void main()
{
	
	color = fragmentColor;
	//color = vec4(1.0f, 0.0f, 0.9f, 1.0f);

    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


