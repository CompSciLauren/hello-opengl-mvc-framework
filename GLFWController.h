// GLFWController.h - a basic Controller (in Model-View-Controller sense) based
//                    on the open-source GLFW toolkit. (http://www.glfw.org)

#ifndef GLFWCONTROLLER_H
#define GLFWCONTROLLER_H

#include "GLFW/glfw3.h"

#include <iostream>
#include <string>

class ModelView;

class GLFWController
{
public:
	GLFWController();
	virtual ~GLFWController();

	void addModel(ModelView* m);
	void reportVersions(std::ostream& os) const;
	void run();

	static bool checkForErrors(std::ostream& os, const std::string& context);
protected:
	GLFWwindow* theWindow;
private:
	// do not allow copies, including pass-by-value:
	GLFWController(const GLFWController& c) {}

	ModelView* model;

	void createWindowAndRC(const std::string& windowTitle);
	virtual void handleDisplay();
};

#endif
