// ModelView.h - a basic combined Model and View for OpenGL

#ifndef MODELVIEW_H
#define MODELVIEW_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ShaderIF.h"

class ModelView
{
public:
	ModelView(ShaderIF* sIF);
	virtual ~ModelView();

	void render() const;
private:
	// This simple program requires 1 vertex array object & 1 vertex buffer object
	GLuint vao[1]; // "vao" = "vertex array object"
	GLuint vbo[1]; // "vbo" = "vertex buffer object" (holds per-vertex attribute data)

	ShaderIF* shaderIF;

	void initModelGeometry();
};

#endif

