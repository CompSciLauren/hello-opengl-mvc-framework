// hello.c++: A "Hello, OpenGL" program that draws a triangle
// This version has been restructured from our "Basic" version
// in order to make use of our MVC framework.

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
	GLFWController controller;               // Step 1
	controller.reportVersions(std::cout);    //     (optional)
	ShaderIF* shaderIF = new ShaderIF("hello.vsh", "hello.fsh");
	                                         // Step 2
	ModelView* mv = new ModelView(shaderIF); // Step 3a
	controller.addModel( mv );               // Step 3b
	controller.run();                        // Step 4

	delete shaderIF;

	return 0;
}

