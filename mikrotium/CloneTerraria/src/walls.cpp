#include"walls.h"

#include<iostream>
#include<sstream>
#include<fstream>

#include"Opengl/ErrorSystem.h"
#include"math/matrix.h"
#include"ItemList.h"
#include"glfw/Window.h"


wall::wall(unsigned int texture, bool render, unsigned short int itemDrop, int x, int y, unsigned char hardness)
	:m_Texture(texture),m_Render(render), m_ItemDrop(itemDrop), m_Transform{x,y}, m_Hardness(hardness)
{}
void wall::drawWalls(Shader& wallSh, float* transform, unsigned int transformLocation)
{
	if (m_Render)
	{
		ChangeTransform(m_Transform[0], m_Transform[1], transform);
		wallSh.SetUniformMat4(transformLocation, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
}
void LoadMapWall(const char* filepath, std::vector<std::vector<Block>>& blocks, std::vector<std::vector<wall>>& walls, unsigned int* texturesIDs)
{

	for (int i = Blocks::xMin; i <= Blocks::xMax; i++)
	{
		std::vector<wall> emptyVector;
		walls.push_back(emptyVector);
	}
	std::ifstream map(filepath);
	std::vector<std::string> lines;
	if (!map)
	{
		std::cout << "can not open map file" << filepath << std::endl;
	}
	else
	{


		lines.emplace_back(" ");
		lines.emplace_back(" ");
		int i = 1;
		while (std::getline(map, lines[i]) && i <= (Blocks::yMax - Blocks::yMin + 1))
		{
			lines.at(i) = " " + lines.at(i) + " ";
			lines.emplace_back(" ");
			i++;
		}
		for (int i = 0; i < lines.size(); i++)
		{
			while (lines.at(i).length() < Blocks::xMax)
			{

				lines.at(i) += " ";
			}
			for (int j = Blocks::xMax; j < lines.at(i).length(); j++)
			{
				lines.at(i)[j] = ' ';
			}
		}
		map.close();
		int y = Blocks::yMax;
		for (int i = 0; y >= Blocks::yMin && i < lines.size(); i++)
		{
			for (int x = Blocks::xMin; x <= Blocks::xMax && x < lines.at(i).length(); x++)
			{
				
				bool render = true;
				unsigned short int wallID = 0;
				switch (lines.at(i).at(x))
				{
				case'd':
					for (int j = 0; j < blocks.at(x).size(); j++)
					{
						render = !(blocks.at(x).at(j).m_Transform[1] == y);
						if (!render)
						{
							break;
						}
					}
					wallID = i_WallDirt;
					break;
				case'i':
					for (int j = 0; j < blocks.at(x).size(); j++)
					{
						render = !(blocks.at(x).at(j).m_Transform[1] == y);
						if (!render)
						{
							break;
						}
					}
					wallID = i_WallIce;
					break;
				}
				createWall(x, y, render, wallID, walls, texturesIDs);
			}
			y--;
		}
	}
}
void createWall(int x, int y, bool render, unsigned short int IDOfItemWall, std::vector<std::vector<wall>>& walls, unsigned int* texturesIDs)
{
	switch (IDOfItemWall)
	{
	case i_WallDirt:
		walls.at(x).emplace_back(texturesIDs[t_Dirt], render, i_WallDirt, x, y, 20);
		break;
	case i_WallIce:
		walls.at(x).emplace_back(texturesIDs[t_Ice], render, i_WallIce, x, y, 15);
		break;
	}
}

void drawWalls(std::vector<DamagedBlock> damagedWalls, unsigned int* damageTextures,std::vector<std::vector<wall>>& walls,Shader& wallsSh, unsigned int shadowLocation , unsigned int wallsCameraLocation,float* camera, unsigned int wallsTransformLocation, float* transform, float* cameraCoordinate)
{ 
	wallsSh.Bind();
	wallsSh.SetUniform1i(shadowLocation, 1);
	wallsSh.SetUniformMat4(wallsCameraLocation, camera);
	for (int j = 0; j < walls.size(); j++)
	{
		if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform) < j)
		{
			break;
		}
		if (floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform) <= j)
		{
			for (int i = 0; i < walls.at(j).size(); i++)
			{
				if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= walls.at(j).at(i).m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= walls.at(j).at(i).m_Transform[1])
				{
					walls.at(j).at(i).drawWalls(wallsSh, transform, wallsTransformLocation);

				}
			}
		}
	}
	for (int i = 0; i < damagedWalls.size(); i++)
	{
		damagedWalls.at(i).DrawDamage(wallsSh, wallsTransformLocation, transform, damageTextures);
	}

}