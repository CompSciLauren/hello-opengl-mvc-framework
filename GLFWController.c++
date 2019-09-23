// GLFWController.c++

#include "GLFWController.h"
#include "ModelView.h"

GLFWController::GLFWController() : theWindow(nullptr), model(nullptr)
{
	glfwInit();

	// Before any OpenGL graphics calls can be made, we must create an
	// OpenGL Rendering Context (RC). There is one RC per window.
	createWindowAndRC("Hello, OpenGL");
}

GLFWController::~GLFWController()
{
	if (theWindow != nullptr)
		glfwDestroyWindow(theWindow);
}

void GLFWController::addModel(ModelView* m)
{
	// for now, we just assume we only have one model:
	model = m;
}

bool GLFWController::checkForErrors(std::ostream& os, const std::string& context)
	// CLASS METHOD
{
	bool hadError = false;
	GLenum e = glGetError();
	while (e != GL_NO_ERROR)
	{
		os << "CheckForErrors (context: " <<  context
		   << "): " << ((int)e) << std::endl;
		e = glGetError();
		hadError = true;
	}
	return hadError;
}

void GLFWController::createWindowAndRC(const std::string& windowTitle)
{
	// The following calls enforce use of only non-deprecated functionality.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	int minor = 8; // Start AT LEAST one greater than where you really want to start
	while ((theWindow == nullptr) && (minor > 0))
	{
		minor--;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		theWindow = glfwCreateWindow(400, 400, windowTitle.c_str(), nullptr, nullptr);
	}
	if (theWindow == nullptr)
	{
		std::cerr << "\n**** COULD NOT CREATE A 4.x RENDERING CONTEXT ****\n\n";
		return;
	}
	glfwMakeContextCurrent(theWindow);
}

void GLFWController::handleDisplay()
{
	// clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);

	if (model != nullptr)
		model->render();

	glfwSwapBuffers(theWindow);

	checkForErrors(std::cout, "GLFWController::handleDisplay");
}

void GLFWController::reportVersions(std::ostream& os) const
{
	const char* glVer = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* glslVer = reinterpret_cast<const char*>
			(glGetString(GL_SHADING_LANGUAGE_VERSION));
	// glGetString can return nullptr if no rendering context has been created
	os << "VERSIONS: GL: ";
	if (glVer == nullptr)
		os << "nullptr (has RC been created?)\n";
	else
		os << glVer << '\n';
	os << "        GLSL: ";
	if (glslVer == nullptr)
		os << "nullptr (has RC been created?)\n";
	else
		os << glslVer << '\n';
	os << "        GLFW: " << glfwGetVersionString() << '\n';
}

void GLFWController::run()
{
	while (!glfwWindowShouldClose(theWindow))
	{
		glfwWaitEvents();
		handleDisplay();
	}
	glfwDestroyWindow(theWindow);
	theWindow = nullptr;
}
