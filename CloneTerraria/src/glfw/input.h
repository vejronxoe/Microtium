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
	extern bool LeftMouseRelease;
	extern bool RightMouseHold;
	extern bool RightMousePress;
	extern bool WHold;
	extern bool WPress;
	extern bool AHold;
	extern bool APress;
	extern bool SHold;
	extern bool SPress;
	extern bool DHold;
	extern bool DPress;
	extern bool TPress;
	extern bool THold;
	extern bool SpaceHold;
	extern bool SpacePress;
	extern bool CtrlHold;
	extern bool F8Press;
	extern bool F5Press;
	extern bool NumberPress[10];
	extern bool EscapePress;
	extern bool EPress;
	extern bool CPress;
	extern bool XPress;
	extern bool VPress;
	void OffAllButtons();
	void EndOfLoop();
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}