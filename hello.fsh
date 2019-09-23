#version 410 core

// The first declaration of a variable with type "out vec4" will be
// used in subsequent pipeline stages as the color for this fragment:
out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(0.8, 0.8, 0.0, 1.0);
}

