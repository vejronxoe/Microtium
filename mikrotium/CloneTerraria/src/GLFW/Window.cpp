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
						std::cout << "reading setting error in first line the letter is :"<< line[i] << std::endl;
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
			}
		}
		else
		{
			std::cout << "can not read first line in setting file " << filepath << std::endl;
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
			std::cout << "can not read second line in setting file " << filepath << std::endl;
			return false;
		}
		halfHeightOfGameTransform = ((height / divisor) * multiplier)/2;
		halfWidthOfGameTransform = ((width / divisor) * multiplier)/2;
		settings.close();

		return true;
	}
}