// ShaderIF.h: Basic interface to read, compile, and link GLSL Shader programs

#ifndef SHADERIF_H
#define SHADERIF_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include <string>

class ShaderIF
{
public:
	struct ShaderSpec
	{
		std::string fName;
		GLenum sType;
	};
	// Creating and managing shader programs:
	//   Create an instance with one of the following two constructors
	//   and use the getShaderPgmID() method to get the shader program ID.
	//   For example:
	//      ShaderIF* sIF = new ShaderIF("MyShader.vsh", "MyShader.fsh");
	//      int pgmId = sIF->getShaderPgmID();
	//   Implementation note for android deviecs: note that the Rendering
	//   Context (including, but not limited to GLSL shader programs) is
	//   sometimes destroyed while your program is running, most notably
	//   whenever the device is rotated, the "home" key pressed, etc. When
	//   this happens, you must be prepared to recognize that the shader
	//   program must be recreated.
	ShaderIF(const std::string& vShader, const std::string& fShader);
	ShaderIF(const ShaderSpec* shaders, int nShaders);
	virtual ~ShaderIF();
	int getShaderPgmID() const { return shaderPgm; }

	// Lookup routines for per-vertex and per-primitive (uniform) variables:
	GLint ppuExists(const std::string& name); // per-primitive (uniform)
	GLint ppuLoc(const std::string& name); // per-primitive (uniform)
	GLint pvaExists(const std::string& name); // per-vertex attribute
	GLint pvaLoc(const std::string& name); // per-vertex attribute
private:
	struct Shader
	{
		std::string fName;
		GLenum sType;
		std::string source;
		int pgmID;

		Shader(): fName(""), sType(0), source(""), pgmID(0) {}
	};
	void destroy();
	void initShaders();

	static bool readShaderSource(Shader& shader);

	int shaderPgm;
	int numShaderComponents;
	Shader* shaders;
};

#endif
