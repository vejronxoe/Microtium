#pragma once
namespace Window
{
	extern int height;
	extern int width;
	extern float divisor;
	extern float multiplier;
	extern float halfHeightOfGameTransform;
	extern float halfWidthOfGameTransform;
	extern bool fullScreen;
	bool GetInfoForWindow(const char* filepath);


}