#pragma once

#include<GLFW/glfw3.h>
namespace Window
{
	extern int height;
	extern int width;
	extern int windowHeight;
	extern int windowWidth;
	extern float halfHeightOfGameTransform;
	extern float halfWidthOfGameTransform;
	extern bool fullScreen;
	extern bool VSync;
	extern float gameZoom;
	extern float volume;
	extern float lineHeight;
	extern float FontSize;
	bool GetInfoForWindow(const char* filepath);
	void SaveSetting(const char* filePath);

}