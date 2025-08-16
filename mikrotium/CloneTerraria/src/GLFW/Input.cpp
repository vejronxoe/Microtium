#include "input.h"

#include<iostream>

#include"Window.h"

namespace Input
{
	double YMousePos;
	double XMousePos;
	bool LeftMouseHold = false;
	bool LeftMousePress = false;
	bool WHold = false;
	bool WPress = false;
	bool AHold = false;
	bool APress = false;
	bool SHold = false;
	bool SPress = false;
	bool DHold = false;
	bool DPress = false;
	bool SpaceHold = false;
	bool SpacePress = false;
	void EndOfLoop()
	{
		LeftMousePress = false;
		WHold = false;
		SPress = false;
		DPress = false;
		APress = false;
		SpacePress = false;

	}
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{


		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			WHold = true;
			WPress = true;
		}
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			WHold = false;
		}



		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			APress = true;
			AHold = true;
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			AHold = false;
		}



		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			SPress = true;
			SHold = true;
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			SHold = false;
		}



		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			DPress = true;
			DHold = true;
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			DHold = false;
		}


		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			SpacePress = true;
			SpaceHold = true;
		}
		else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			SpaceHold = false;
		}
	}
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		YMousePos = (((-1 * ypos) / Window::divisor) * Window::multiplier) + Window::halfHeightOfGameTransform;
		XMousePos = ((xpos / Window::divisor) * Window::multiplier) - Window::halfWidthOfGameTransform;
	}
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			LeftMouseHold = true;
			LeftMousePress = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			LeftMouseHold = false;
		}
	}
}
