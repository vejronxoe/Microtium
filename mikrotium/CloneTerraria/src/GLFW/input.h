#pragma once

#include<GLFW/glfw3.h>


namespace Input
{
	extern double YMousePos;
	extern double XMousePos;
	extern bool LeftMouseHold;
	extern bool LeftMousePress;
	extern bool WHold;
	extern bool WPress;
	extern bool AHold;
	extern bool APress;
	extern bool SHold;
	extern bool SPress;
	extern bool DHold;
	extern bool DPress;
	extern bool SpaceHold;
	extern bool SpacePress;
	void EndOfLoop();
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
}