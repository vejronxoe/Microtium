#include "Window.h"

#include<iostream>
#include<sstream>
#include<fstream>

namespace Window
{
	int height;
	int width;
	float halfHeightOfGameTransform;
	float halfWidthOfGameTransform;
	bool fullScreen;
	float gameZoom;
	float UIZoom;
	float lineHeight;
	float FontSize;
	bool GetInfoForWindow(const char* filepath)
	{
		height = 800;
		width = 1200;
		fullScreen = false;
		std::string line;
		std::ifstream settings(filepath);
		if (!settings)
		{
			std::cout << "can not open setting file " << filepath << std::endl;
			return false;
		}
		 
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		if (!primary)
		{
			std::cout << "can not get primary monitor" << std::endl;
			return false;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(primary);
		if (fullScreen)
		{
			Window::height = mode->height;
			Window::width = mode->width;
		}

		settings.close();

		return true;
	}

}