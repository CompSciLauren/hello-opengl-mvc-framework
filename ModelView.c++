// ModelView.c++ - a basic combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"

ModelView::ModelView(ShaderIF* sIF) : shaderIF(sIF)
{
	initModelGeometry();
}

ModelView::~ModelView()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void ModelView::initModelGeometry()
{
	float triangleVertices[3][2] =
	{
		{  -0.75, -0.75 }, { 0.75, -0.75 }, {  0.0, 0.75 }
	};

	glGenVertexArrays(1, vao); // get a new, previously unused VAO name
	glBindVertexArray(vao[0]); // (initialize it and) make it active

	glGenBuffers(1, vbo); // get a new, previously unused VBO name
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // (initialize it and) make it active

	// Allocate space for AND copy the CPU data in "triangleVertices" to the VBO on the GPU:
	int numBytesInBuffer = 3 * 2 * sizeof(float); // 3 points; 2 coords each; float
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, triangleVertices, GL_STATIC_DRAW);

	int coordinateLocation = 0; // See "layout" in hello.vsh
	glVertexAttribPointer(coordinateLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(coordinateLocation);
}

void ModelView::render() const
{
	// save the current GLSL program in use
	int savedPgm = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &savedPgm);
	// place our vertex-fragment shader program in use
	glUseProgram(shaderIF->getShaderPgmID());

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 in buffer; process 3 vertices
	
	// restore the previous program
	glUseProgram(savedPgm);
}

