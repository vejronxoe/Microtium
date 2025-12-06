#include"Blocks.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"math/matrix.h"
#include"ItemList.h"
#include"glfw/Window.h"

namespace Blocks
{
	int xMax;
	int xMin;
	int yMax;
	int yMin;
};
Block::Block(unsigned int tex
	, int x
	, int y
	, unsigned char behavior
	, unsigned char hardness
	, unsigned short int itemDrop)
	: m_te(tex), m_Transform{ x,y }, m_BlockBehavior(behavior),m_Hardness(hardness),m_ItemDrop(itemDrop)
{}

void Block::DrawBlock(Shader& basicShader
	, unsigned int location
	, float* transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(location, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_te));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}



void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[t_TopGrass] = CreateTextureRGBA("res/textures/topGrassBlock.png");
	IDs[t_LeftGrass] = CreateTextureRGBA("res/textures/leftGrassBlock.png");
	IDs[t_DownGrass] = CreateTextureRGBA("res/textures/downGrassBlock.png");
	IDs[t_RightGrass] = CreateTextureRGBA("res/textures/rightGrassBlock.png");
	IDs[t_TopDownGrass] = CreateTextureRGBA("res/textures/topDownGrassBlock.png");
	IDs[t_LeftRightGrass] = CreateTextureRGBA("res/textures/leftRightGrassBlock.png");
	IDs[t_TopLeftGrass] = CreateTextureRGBA("res/textures/topLeftGrassBlock.png");
	IDs[t_DownLeftGrass] = CreateTextureRGBA("res/textures/downLeftGrassBlock.png");
	IDs[t_DownRightGrass] = CreateTextureRGBA("res/textures/downRightGrassBlock.png");
	IDs[t_TopRightGrass] = CreateTextureRGBA("res/textures/topRightGrassBlock.png");
	IDs[t_MissingTopGrass] = CreateTextureRGBA("res/textures/missingTopGrassBlock.png");
	IDs[t_MissingLeftGrass] = CreateTextureRGBA("res/textures/missingLeftGrassBlock.png");
	IDs[t_MissingDownGrass] = CreateTextureRGBA("res/textures/missingDownGrassBlock.png");
	IDs[t_MissingRightGrass] = CreateTextureRGBA("res/textures/missingRightGrassBlock.png");
	IDs[t_FullGrass] = CreateTextureRGBA("res/textures/fullGrassBlock.png");
	IDs[t_Dirt] = CreateTextureRGBA("res/textures/dirtBlock.png");
	IDs[t_Ice] = CreateTextureRGBA("res/textures/ice.png");
	IDs[t_Asphalt] = CreateTextureRGBA("res/textures/Asphalt.png");
	IDs[t_Platform] = CreateTextureRGBA("res/textures/platform.png");
	IDs[t_Platform] = CreateTextureRGBA("res/textures/platform.png");
	IDs[t_ForestPlank] = CreateTextureRGBA("res/textures/forestPlank.png");
}


unsigned int GrassBlockTextureSelector(unsigned int *TexturesIDs
	, std::vector<std::string>& lines
	, int i 
	, int j)
{
	if (lines.at(i - 1).at(j) == ' ')
	{
		if (lines.at(i + 1).at(j) == ' ')
		{
			if (lines.at(i).at(j - 1) == ' ')
			{
				if (lines.at(i).at(j + 1) == ' ')
				{
					return TexturesIDs[t_FullGrass];
				}
				else
				{
					return TexturesIDs[t_MissingRightGrass];
				}
			}
			else if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[t_MissingLeftGrass];
			}
			else
			{
				return TexturesIDs[t_TopDownGrass];
			}
		}
		else if (lines.at(i).at(j - 1) == ' ')
		{
			if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[t_MissingDownGrass];
			}
			else
			{
				return TexturesIDs[t_TopLeftGrass];
			}
		}
		else if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[t_TopRightGrass];
		}
		else
		{
			return TexturesIDs[t_TopGrass];
		}
	}
	else if (lines.at(i + 1).at(j) == ' ')
	{
		if (lines.at(i).at(j - 1) == ' ')
		{
			if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[t_MissingTopGrass];
			}
			else
			{
				return TexturesIDs[t_DownLeftGrass];
			}
		}
		else if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[t_DownRightGrass];
		}
		else
		{
			return TexturesIDs[t_DownGrass];
		}
	}
	else if (lines.at(i).at(j - 1) == ' ')
	{
		if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[t_LeftRightGrass];
		}
		else
		{
			return TexturesIDs[t_LeftGrass];
		}
	}
	else if (lines.at(i).at(j + 1) == ' ')
	{
		return TexturesIDs[t_RightGrass];
	}
	else
	{
		return TexturesIDs[t_Dirt];
	}
}

void CreateBlock(int x
	, int y
	, unsigned short int IDOfItemBlock
	, std::vector<std::vector<Block>>& blocks
	, unsigned int* texturesIDs)
{
	switch (IDOfItemBlock)
	{
	case i_Dirt:
		blocks.at(x).emplace_back(texturesIDs[t_Dirt], x, y, b_BasicSolid, 15, i_Dirt);
		break;
	case i_Platform:
		blocks.at(x).emplace_back(texturesIDs[t_Platform], x, y, b_Platform, 20, i_Platform);
		break;
	case i_Asphalt:
		blocks.at(x).emplace_back(texturesIDs[t_Asphalt], x, y, b_Asphalt, 35, i_Asphalt);
		break;
	case i_Ice:
		blocks.at(x).emplace_back(texturesIDs[t_Ice], x, y, b_Slippery, 15, i_Ice);
		break;
	}
	
}

void LoadMap(const char* filepath
	, std::vector<std::vector<Block>>& blocks
	, int minX
	, int maxX
	, int minY
	, int maxY
	, unsigned int* texturesIDs)
{
	Blocks::xMax = maxX;
	Blocks::xMin = minX;
	Blocks::yMax = maxY;
	Blocks::yMin = minY;
	for (int i = minX ; i <= maxX; i++)
	{
		std::vector<Block> emptyVector;
		blocks.push_back(emptyVector);
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
		while (std::getline(map, lines[i]) && i <= (maxY - minY + 1))
		{
			lines.at(i) = " " + lines.at(i) + " ";
			lines.emplace_back(" ");
			i++;
		}
		for (int i = 0; i < lines.size(); i++)
		{
			while (lines.at(i).length() < maxX)
			{

				lines.at(i) += " ";
			}
			for (int j = maxX; j < lines.at(i).length(); j++)
			{
				lines.at(i)[j] = ' ';
			}
		}
		map.close();
		int y = maxY;
		for (int i = 0; y >= minY && i < lines.size(); i++)
		{
			for (int x = minX; x <= maxX && x < lines.at(i).length(); x++)
			{
				unsigned short int blockID = 0;
				switch (lines.at(i).at(x))
				{
				case'd':
					blocks.at(x).emplace_back(GrassBlockTextureSelector(texturesIDs, lines, i, x) , x, y, b_BasicSolid, 25,i_Dirt);
					break;
				case'p':
					blockID = i_Platform;
					break;
				case'a':
					blockID = i_Asphalt;
					break;
				case'i':
					blockID = i_Ice;
					break;
				}
				if (blockID)
				{
					CreateBlock(x, y, blockID, blocks, texturesIDs);
				}
			}
			y--;
		}
	}
}

void DamagedBlock::DrawDamage(Shader& basicShader
	, unsigned int location
	, float* transform
	, unsigned int* texture)
{

	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(location, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[m_HP - 1]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}

DamagedBlock::DamagedBlock(int x
	, int y
	, char HP)
	:m_Transform{ x, y }, m_HP(HP)
{}

void drawBlocks(std::vector<std::vector<Block>>& blocks
	, std::vector<DamagedBlock>& damagedBlocks
	, float* cameraCoordinate
	, Shader& basicSh
	, unsigned int *damageTexture
	, unsigned int transformLocation
	, float* transform
	, unsigned int cameraLocation
	, float* camera)
{
	basicSh.Bind();
	basicSh.SetUniformMat4(cameraLocation, camera);
	for (int j = 0; j < blocks.size(); j++)
	{
		if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform) < j)
		{
			break;
		}
		else if (floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform) <= j)
		{
			for (int i = 0; i < blocks.at(j).size(); i++)
			{
				if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= blocks.at(j).at(i).m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= blocks.at(j).at(i).m_Transform[1])
				{
					blocks.at(j).at(i).DrawBlock(basicSh, transformLocation, transform);
				}
			}
		}
	}
	for (int i = 0; i < damagedBlocks.size(); i++)
	{
		damagedBlocks.at(i).DrawDamage(basicSh, transformLocation, transform, damageTexture);
	}

}