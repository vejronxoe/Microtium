#include "Window.h"

#include<iostream>
#include<sstream>
#include<fstream>

namespace Window
{
	int height;
	int width;
	float divisor = 40;
	float multiplier = 2.5f;
	float halfHeightOfGameTransform;
	float halfWidthOfGameTransform;
	bool fullScreen;
	float zoomOfScreen;
	int scaleOfHUD;
	bool GetInfoForWindow(const char* filepath)
	{
		height = 0;
		width = 0;
		fullScreen = true;
		std::string line;
		std::ifstream settings(filepath);
		if (!settings)
		{
			std::cout << "can not open setting file " << filepath << std::endl;
			return false;
		}
		
		if (std::getline(settings, line))
		{
			if (line[0] == '0')
			{
				fullScreen = false;
			}
		}
		else
		{
			std::cout << "can not read first line in setting file " << filepath << std::endl;
			return false;
		}
	
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		if (!primary)
		{
			std::cout << "can not get primary monitor" << std::endl;
			return false;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(primary);
		if (!fullScreen)
		{
			if (std::getline(settings, line))
			{
				bool afterX = false;
				bool isItFirst = true;
				for (int i = 0; i < line.length(); i++)
				{
					int readValue = -1;
					for (int j = 0; j < 10; j++)
					{
						if (line[i] == '0' + j)
						{
							readValue = j;
							break;
						}

					}
					if (readValue == -1)
					{
						if (line[i] == 'x')
						{
							afterX = true;
							isItFirst = true;
						}
						else
						{
							std::cout << "reading setting error in second line the letter is :" << line[i] << std::endl;
							return false;
						}
					}
					else
					{
						if (afterX)
						{
							if (isItFirst)
							{
								isItFirst = false;
								height = readValue;
							}
							else
							{
								height = height * 10;
								height += readValue;
							}
						}
						else
						{
							if (isItFirst)
							{
								isItFirst = false;
								width = readValue;
							}
							else
							{
								width = width * 10;
								width += readValue;
							}
						}
					}
					
					if (height >= mode->height || width >= mode->width)
					{
						fullScreen = true;
						break;
					}
					
				}
			}
			else
			{
				std::cout << "can not read second line in setting file " << filepath << std::endl;
				return false;
			}
		}
		else
		{
			std::getline(settings, line);
		}
		if (fullScreen || height <= 0 && width <= 0)
		{
			height = mode->height;
			width = mode->width;
		}
		
		
		if (std::getline(settings, line))
		{
			bool BeforDot = true;
			if (line.length() < 5)
			{
				for (int i = 0; i < line.length(); i++)
				{
					int readValue = -1;
					for (int j = 0; j < 10; j++)
					{
						if (line[i] == '0' + j)
						{
							readValue = j;
							break;
						}

					}
					if (readValue == -1)
					{
						std::cout << "reading setting error in third line the letter is :" << line[i] << std::endl;
						return false;
					}
					if (BeforDot)
					{
						zoomOfScreen = readValue;
						BeforDot = false;
					}
					else
					{
						zoomOfScreen += readValue/std::powf(10,i);
					}

				}
			}
			else
			{
				std::cout << "to much letters or non letters (min = 1, max = 4)in third line in setting file " << filepath << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "can not read third line in setting file " << filepath << std::endl;
			return false;
		}

		if (std::getline(settings, line))
		{
			bool BeforDot = true;
			if (line.length() < 4 && line.length() > 1)
			{
				
				for (int i = 1; i < line.length(); i++)
				{
					int readValue = -1;
					for (int j = 0; j < 10; j++)
					{
						if (line[i] == '0' + j)
						{
							readValue = j;
							break;
						}

					}
					if (readValue == -1)
					{
						std::cout << "reading setting error in fourth line the letter is :" << line[i] << std::endl;
						return false;
					}
					if (BeforDot)
					{
						scaleOfHUD = readValue;
						BeforDot = false;
					}
					else
					{
						scaleOfHUD = readValue * 10;
					}
					if (scaleOfHUD > 17)
					{
						std::cout << "to Big value 0r not any value (min = 2, max = 3 letters; min = 0, max = 17 value)in fourth line in setting file " << filepath << std::endl;
						return false;
					}
				}
				if (line[0] == '+')
				{
					scaleOfHUD = scaleOfHUD;
				}
				else if (line[0] == '-')
				{
					scaleOfHUD = scaleOfHUD * -1;
				}
				else
				{
					std::cout << "reading setting error ( expected - or + )in fourth line the letter is :" << line[0] << std::endl;
					return false;
				}
			}
			else
			{
				std::cout << "to Big value 0r not any value (min = 2, max = 3 letters; min = 0, max = 17 value)in fourth line in setting file " << filepath << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "can not read fourth line in setting file " << filepath << std::endl;
			return false;
		}
		
		settings.close();

		return true;
	}
	void PostWindowSettings(GLFWwindow* window)
	{
		glfwGetWindowSize(window, &width, &height);
		halfHeightOfGameTransform = ((height / divisor) * multiplier * zoomOfScreen) / 2;
		halfWidthOfGameTransform = ((width / divisor) * multiplier * zoomOfScreen) / 2;
	}
}