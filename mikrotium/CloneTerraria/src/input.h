#pragma once

#include<GLFW/glfw3.h>


namespace Input
{
	extern double YmousePos;
	extern double XmousePos;
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
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
}