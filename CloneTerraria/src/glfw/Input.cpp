	#include "input.h"

#include<iostream>

#include"Window.h"

namespace Input
{
	double YMousePos;
	double XMousePos;
	double YRawMousePos;
	double XRawMousePos;
	char MouseWheel;
	bool LeftMouseHold = false;
	bool LeftMousePress = false;
	bool RightMouseHold = false;
	bool RightMousePress = false;
	bool WHold = false;
	bool WPress = false;
	bool AHold = false;
	bool APress = false;
	bool SHold = false;
	bool SPress = false;
	bool DHold = false;
	bool DPress = false;
	bool THold = false;
	bool TPress = false;
	bool SpaceHold = false;
	bool SpacePress = false;
	bool CtrlHold = false;
	bool NumberPress[10] = {false, false, false, false, false, false, false, false, false, false};
	bool EscapePress = false;

	void EndOfLoop()
	{
		MouseWheel = 0;
		LeftMousePress = false;
		RightMousePress = false;
		WPress = false;
		SPress = false;
		DPress = false;
		APress = false;
		TPress = false;
		SpacePress = false;
		for (int i = 0; i < 10; i++)
		{
			NumberPress[i] = false;
		}
		EscapePress = false;
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

		if (key == GLFW_KEY_T && action == GLFW_PRESS)
		{
			TPress = true;
			THold = true;
		}
		else if (key == GLFW_KEY_T && action == GLFW_RELEASE)
		{
			THold = false;
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

		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			CtrlHold = true;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
		{
			CtrlHold = false;
		}


		if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		{
			NumberPress[9] = true;
		}
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			NumberPress[0] = true;
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			NumberPress[1] = true;
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			NumberPress[2] = true;
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		{
			NumberPress[3] = true;
		}
		if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		{
			NumberPress[4] = true;
		}
		if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		{
			NumberPress[5] = true;
		}
		if (key == GLFW_KEY_7 && action == GLFW_PRESS)
		{
			NumberPress[6] = true;
		}
		if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		{
			NumberPress[7] = true;
		}
		if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		{
			NumberPress[8] = true;
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			EscapePress = true;
		}

	}
	void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		XRawMousePos = xpos;
		YRawMousePos =	ypos;
		YMousePos = (((-ypos / Window::height) * 2) + 1) * Window::halfHeightOfGameTransform;
		XMousePos = (((xpos / Window::width) * 2) - 1) * Window::halfWidthOfGameTransform;
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
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			RightMouseHold = true;
			RightMousePress = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			RightMouseHold = false;
		}
	}
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MouseWheel += yoffset;
	}
}

