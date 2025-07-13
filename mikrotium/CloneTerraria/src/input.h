#pragma once

#include<GLFW/glfw3.h>


namespace Input
{
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
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}