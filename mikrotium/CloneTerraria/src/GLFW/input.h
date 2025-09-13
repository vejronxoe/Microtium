#pragma once

#include<GLFW/glfw3.h>


namespace Input
{
	extern double YMousePos;
	extern double XMousePos;
	extern double YRawMousePos;
	extern double XRawMousePos;
	extern char MouseWheel;
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
	extern bool NumberPress[10];
	extern bool EscapePress;
	void EndOfLoop();
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}