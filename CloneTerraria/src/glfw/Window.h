#pragma once

#include<GLFW/glfw3.h>
namespace Window
{
	extern int height;
	extern int width;
	extern float halfHeightOfGameTransform;
	extern float halfWidthOfGameTransform;
	extern bool fullScreen;
	extern float gameZoom;
	extern float UIZoom;
	extern float lineHeight;
	extern float FontSize;
	bool GetInfoForWindow(const char* filepath);
	
}