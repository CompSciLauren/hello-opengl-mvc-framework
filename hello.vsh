#version 410 core

layout (location = 0) in vec2 mcPosition;

void main()
{
	gl_Position = vec4(mcPosition, 0, 1);
}

