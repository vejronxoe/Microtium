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
	float volume;
	float gameZoom;
	bool fullScreen;
	bool VSync;
	bool smoothShadows;
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
		VSync = false;
		smoothShadows = true;

		volume = 50;
		gameZoom = 50;
		std::ifstream file(filepath, std::ios::binary);
		
		bool corrupted = true;
		if (file.good())
		{
			corrupted = false;
			for (int i = 0; i < 6; i++)
			{
				switch (i)
				{
				case 0:
					file.read(reinterpret_cast<char*>(&windowHeight), sizeof(windowHeight));
					windowHeight = Clamp(windowHeight, 400, maxHeight);
					break;
				case 1:
					file.read(reinterpret_cast<char*>(&windowWidth), sizeof(windowWidth));
					windowWidth = Clamp(windowWidth, 400, maxHeight);
					break;
				case 2:
					file.read(reinterpret_cast<char*>(&volume), sizeof(volume));
					volume = Clamp(volume, 0, 1);
					break;
				case 3:
					file.read(reinterpret_cast<char*>(&gameZoom), sizeof(gameZoom));
					gameZoom = Clamp(gameZoom, 0, 1);
					break;
				case 4:
					file.read(reinterpret_cast<char*>(&fullScreen), sizeof(fullScreen));
					break;
				case 5:
					file.read(reinterpret_cast<char*>(&VSync), sizeof(VSync));
					break;
				case 6:
					file.read(reinterpret_cast<char*>(&smoothShadows), sizeof(smoothShadows));
					break;
				}

				if (!file.good())
				{
					corrupted = true;
					break;
				}
			}
		}
		if (!file.good())
		{
			corrupted = true;
		}
		file.close();
		
		corrupted = true;
		if (corrupted)
		{
			height = maxHeight / 2.0f;
			width = maxWidth / 2.0f;
			windowHeight = maxHeight / 2.0f;
			windowWidth = maxWidth / 2.0f;
			volume = 0.5f;
			gameZoom = 0.5f;
			fullScreen = false;
			VSync = false;
			smoothShadows = true;
			if (!SaveSetting(filepath))
			{
				std::cout << "error: fail to save settings\n";
			}
		}
		 
	
		return true;
	}
	bool SaveSetting(const char* filePath)
	{
		std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
		if (!file.good())
		{
			return false;
		}
		file.write(reinterpret_cast<char*>(&windowHeight), sizeof(windowHeight));
		file.write(reinterpret_cast<char*>(&windowWidth), sizeof(windowWidth));
		file.write(reinterpret_cast<char*>(&volume), sizeof(volume));
		file.write(reinterpret_cast<char*>(&gameZoom), sizeof(gameZoom));
		file.write(reinterpret_cast<char*>(&fullScreen), sizeof(fullScreen));
		file.write(reinterpret_cast<char*>(&VSync), sizeof(VSync));
		file.write(reinterpret_cast<char*>(&smoothShadows), sizeof(smoothShadows));
		if (!file.good())
		{
			return false;
		}
		file.close();
		return true;
	}

}