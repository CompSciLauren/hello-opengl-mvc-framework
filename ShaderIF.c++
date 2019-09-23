// ShaderIF.c++: Basic interface to read, compile, and link GLSL Shader programs

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "ShaderIF.h"

ShaderIF::ShaderIF(const std::string& vShader, const std::string& fShader) :
	shaderPgm(0), numShaderComponents(2) // just a vertex & fragment shader
{
	shaders = new Shader[numShaderComponents];
	shaders[0].fName = vShader;
	shaders[0].sType = GL_VERTEX_SHADER;
	shaders[1].fName = fShader;
	shaders[1].sType = GL_FRAGMENT_SHADER;
	initShaders();
}

ShaderIF::ShaderIF(const ShaderSpec* shaders, int nShaders) :
	shaderPgm(0), numShaderComponents(nShaders)
{
	this->shaders = new Shader[numShaderComponents];
	for (int i=0 ; i<numShaderComponents ; i++)
	{
		this->shaders[i].fName = shaders[i].fName;
		this->shaders[i].sType = shaders[i].sType;
	}
	initShaders();
}

ShaderIF::~ShaderIF()
{
	if (shaders != nullptr)
	{
		destroy();
		delete [] shaders;
		shaders = nullptr;
	}
}

void ShaderIF::destroy()
{
	for (int si=0 ; si<numShaderComponents ; si++)
	{
		int sPgmID = shaders[si].pgmID;
		if (sPgmID > 0)
		{
			if (glIsShader(sPgmID))
				glDeleteShader(sPgmID);
			shaders[si].pgmID = 0;
		}
	}
	if (shaderPgm > 0)
	{
		if (glIsProgram(shaderPgm))
			glDeleteProgram(shaderPgm);
	}
	shaderPgm = 0;
	numShaderComponents = 0;
}

void ShaderIF::initShaders()
{
	for (int i = 0; i < numShaderComponents; i++ )
	{
		if (!readShaderSource(shaders[i]))
			return;

		shaders[i].pgmID = glCreateShader(shaders[i].sType);
		const char* src = shaders[i].source.c_str();
		glShaderSource(shaders[i].pgmID, 1, &src, nullptr);
		glCompileShader(shaders[i].pgmID);
		
		GLint  compiled;
		glGetShaderiv(shaders[i].pgmID, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			std::cerr << shaders[i].fName << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(shaders[i].pgmID, GL_INFO_LOG_LENGTH, &logSize );
			if (logSize <= 0)
				std::cerr << "No log information available (" << logSize << ")\n";
			else
			{
				try
				{
					char* logMsg = new char[logSize];
					glGetShaderInfoLog(shaders[i].pgmID, logSize, nullptr, logMsg );
					std::cerr << "ERROR LOG: '" << logMsg << "'\n";
					delete [] logMsg;
				}
				catch (std::bad_alloc bae)
				{
					std::cerr << "Could not allocate error log buffer of size: " << logSize << std::endl;
				}
			}
			destroy();
			return;
		}
	}
	
	shaderPgm = glCreateProgram();
	for (int i=0 ; i<numShaderComponents ; i++)
		glAttachShader(shaderPgm, shaders[i].pgmID);
	
	/* link  and error check */
	glLinkProgram(shaderPgm);
	
	GLint  linked;
	glGetProgramiv(shaderPgm, GL_LINK_STATUS, &linked);
	if ( !linked )
	{
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv( shaderPgm, GL_INFO_LOG_LENGTH, &logSize);
		if (logSize <= 0)
			std::cerr << "No log information available (" << logSize << ")\n";
		try
		{
			char* logMsg = new char[logSize];
			glGetProgramInfoLog( shaderPgm, logSize, nullptr, logMsg );
			std::cerr << "ERROR LOG: '" << logMsg << "'\n";
			delete [] logMsg;
		}
		catch (std::bad_alloc bae)
		{
			std::cerr << "Could not allocate error log buffer of size: " << logSize << std::endl;
		}
		destroy();
	}
}

GLint ShaderIF::ppuExists(const std::string& name) // per-primitive (uniform)
{
	if (shaderPgm <= 0)
		return -1;
	GLint loc = glGetUniformLocation(shaderPgm, name.c_str());
	return loc;
}

GLint ShaderIF::ppuLoc(const std::string& name) // per-primitive (uniform)
{
	if (shaderPgm <= 0)
	{
		std::cerr << "Shader program did not get built; cannot look up: '" << name << "'\n";
		return -1;
	}
	GLint loc = glGetUniformLocation(shaderPgm, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ShaderIF::pvaExists(const std::string& name) // per-vertex attribute
{
	if (shaderPgm <= 0)
		return -1;
	GLint loc = glGetAttribLocation(shaderPgm, name.c_str());
	return loc;
}

GLint ShaderIF::pvaLoc(const std::string& name) // per-vertex attribute
{
	if (shaderPgm <= 0)
	{
		std::cerr << "Shader program did not get built; cannot look up: '" << name << "'\n";
		return -1;
	}
	GLint loc = glGetAttribLocation(shaderPgm, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}

bool ShaderIF::readShaderSource(Shader& shader) // CLASS METHOD
{
	std::ifstream is(shader.fName.c_str());
	if (is.fail())
	{
		std::cerr << "Could not open " << shader.fName << " for reading.\n";
		return false;
	}
	std::string line;
	getline(is, line);
	while (!is.eof())
	{
		shader.source = shader.source + line + '\n';
		getline(is, line);
	}
	return true;
}
