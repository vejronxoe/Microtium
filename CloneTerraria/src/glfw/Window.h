#pragma once

#include<GLFW/glfw3.h>

namespace Window
{
	extern int height;
	extern int width;
	extern float divisor;
	extern float multiplier;
	extern float halfHeightOfGameTransform;
	extern float halfWidthOfGameTransform;
	extern bool fullScreen;
	extern float zoomOfScreen;
	extern int scaleOfHUD;
	bool GetInfoForWindow(const char* filepath);
	void PostWindowSettings(GLFWwindow* window);

}