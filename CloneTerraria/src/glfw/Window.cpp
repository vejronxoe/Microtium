#include "Window.h"

#include<iostream>
#include<sstream>
#include<fstream>
#include"../math/VectorOperation.h"

namespace Window
{
	int height;
	int width;
	int windowHeight;
	int windowWidth;
	float halfHeightOfGameTransform;
	float halfWidthOfGameTransform;
	bool fullScreen;
	float gameZoom;
	float UIZoom;
	float volume;
	float lineHeight;
	float FontSize;
	bool GetInfoForWindow(const char* filepath)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		if (!primary)
		{
			std::cout << "can not get primary monitor" << std::endl;
			return false;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(primary);
	
		int maxHeight = mode->height;
		int maxWidth = mode->width;
	
		height = maxHeight / 2.0f;
		width = maxWidth / 2.0f;
		windowHeight = maxHeight / 2.0f;
		windowWidth = maxWidth / 2.0f;
		fullScreen = false;
		volume = 50;
		gameZoom = 50;
		UIZoom = 50;
		std::string line;
		std::ifstream settings(filepath);
		bool corrupted = true;
		if (settings)
		{
			corrupted = false;
			int i = 0;
			while (std::getline(settings, line))
			{
				switch (i)
				{
				case 0:
					if (line.length() == 1)
					{
						fullScreen = (line[0] == '1');
					}
					else
					{
						corrupted = true;
					}
					break;
				case 1:
					windowWidth = atoi(line.c_str());
					windowWidth = Clamp(width, 400, maxWidth);
					if(fullScreen)
					{
						width = maxWidth;
					}
					else
					{	
						width = windowWidth;
					}

					break;
				case 2:
					windowHeight = atoi(line.c_str());
					windowHeight = Clamp(height, 400, maxHeight);
					if (fullScreen)
					{
						height = maxHeight;
					}
					else
					{
						height = windowHeight;
					}
					break;
				case 3:
					volume = atof(line.c_str());
					volume = Clamp(volume, 0, 100);
					break;
				case 4:
					gameZoom = atof(line.c_str());
					gameZoom = Clamp(gameZoom, 0, 100);
					break;
				case 5:
					UIZoom = atof(line.c_str());
					UIZoom = Clamp(UIZoom, 0, 100);
					break;
				default:
					corrupted = true;
					break;
				}
				if (corrupted == true)
				{
					break;
				}
				i++;
			}
			if (i < 6)
			{
				corrupted = true;
			}
		}
		settings.close();
		if (corrupted)
		{
			height = maxHeight / 2.0f;
			width = maxWidth / 2.0f;
			fullScreen = false;
			volume = 50;
			gameZoom = 50;
			UIZoom = 50;
			SaveSetting(filepath);
		}
		 
	
		return true;
	}
	void SaveSetting(const char* filePath)
	{
		std::ofstream settings(filePath);
		settings << (fullScreen ? '1' : '0') << std::endl;
		settings << windowWidth << std::endl;
		settings << windowHeight << std::endl;
		settings << volume << std::endl;
		settings << gameZoom << std::endl;
		settings << UIZoom;
		settings.close();
	}

}