#include"BlocksAndWalls.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
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

DamagedBlock::DamagedBlock(int x
	, int y
	, char HP)
	:m_Transform{ x, y }, m_HP(HP)
{
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



Block::Block(unsigned int tex
	, int y
	, unsigned char behavior
	, unsigned char hardness
	, unsigned short int itemDrop)
	: m_te(tex), m_Y (y) , m_BlockBehavior(behavior),m_Hardness(hardness),m_ItemDrop(itemDrop)
{}

void Block::DrawBlock(Shader& basicShader
	, int x
	, unsigned int location
	, float* transform)
{
	ChangeTransform(x, m_Y, transform);
	basicShader.SetUniformMat4(location, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_te));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}

wall::wall(unsigned int texture
	, bool render
	, unsigned short int itemDrop
	, int y
	, unsigned char hardness)
	:m_Texture(texture), m_Render(render), m_ItemDrop(itemDrop), m_Y(y), m_Hardness(hardness)
{
}
void wall::drawWalls(Shader& wallSh
	, int x
	, float* transform
	, unsigned int transformLocation)
{
	if (m_Render)
	{
		ChangeTransform(x, m_Y, transform);
		wallSh.SetUniformMat4(transformLocation, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
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
	IDs[t_Sand] = CreateTextureRGBA("res/textures/sand.png");
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


void DestroyBlock(std::vector<std::vector<Block>> blocks
	, std::vector<bool> isThereSandOnX
	, int x
	, int y)
{
	int index;
	bool blockExistence = false;
	for (index = 0; index < blocks.size(); index++)
	{
		if (blocks.at(x).at(index).m_Y == y)
		{
			blockExistence = true;
			break;
		}
	}
	if (blockExistence)
	{
		if (blocks.at(x).at(index).m_BlockBehavior == b_Sand)
		{
			isThereSandOnX.at(x) = false;
		}
		
	}
}


void LoadMapBlocksAndWalls(const char* filePathWalls
	, const char* filePathBlocks
	, std::vector<std::vector<wall>>& walls
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
	for (int i = minX; i <= maxX; i++)
	{
		std::vector<Block> emptyVector;
		blocks.push_back(emptyVector);
	}
	for (int i = Blocks::xMin; i <= Blocks::xMax; i++)
	{
		std::vector<wall> emptyVector;
		walls.push_back(emptyVector);
	}
	{
		std::ifstream map(filePathBlocks);
		std::vector<std::string> lines;
		if (!map)
		{
			std::cout << "can not open map file" << filePathBlocks << std::endl;
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
						blocks.at(x).emplace_back(GrassBlockTextureSelector(texturesIDs, lines, i, x), y, b_BasicSolid, 25, i_Dirt);
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
					case's':
						blockID = i_Sand;
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
	{
		std::ifstream map(filePathWalls);
		std::vector<std::string> lines;
		if (!map)
		{
			std::cout << "can not open map file" << filePathWalls << std::endl;
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
						wallID = i_WallDirt;
						break;
					case'i':
						wallID = i_WallIce;
						break;
					}
					createWall(x, y, render, wallID, walls, texturesIDs);
				}
				y--;
			}
		}
	}
}



void drawBlocks(std::vector<std::vector<Block>>& blocks
	, std::vector<DamagedBlock>& damagedBlocks
	, float* cameraCoordinate
	, Shader& basicSh
	, unsigned int* damageTexture
	, unsigned int transformLocation
	, float* transform
	, unsigned int cameraLocation
	, float* camera)
{
	basicSh.Bind();
	basicSh.SetUniformMat4(cameraLocation, camera);

	for (int j = floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform); j <= ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform); j++)
	{
		for (int i = 0; i < blocks.at(j).size(); i++)
		{
			if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= blocks.at(j).at(i).m_Y && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= blocks.at(j).at(i).m_Y)
			{
				blocks.at(j).at(i).DrawBlock(basicSh, j, transformLocation, transform);
			}
		}
	}

	for (int i = 0; i < damagedBlocks.size(); i++)
	{
		damagedBlocks.at(i).DrawDamage(basicSh, transformLocation, transform, damageTexture);
	}

}
void drawWalls(std::vector<DamagedBlock> damagedWalls
	, unsigned int* damageTextures
	, std::vector<std::vector<wall>>& walls
	, Shader& wallsSh
	, unsigned int shadowLocation
	, unsigned int wallsCameraLocation
	, float* camera
	, unsigned int wallsTransformLocation
	, float* transform
	, float* cameraCoordinate)
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
				if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= walls.at(j).at(i).m_Y && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= walls.at(j).at(i).m_Y)
				{
					walls.at(j).at(i).drawWalls(wallsSh, j, transform, wallsTransformLocation);

				}
			}
		}
	}
	for (int i = 0; i < damagedWalls.size(); i++)
	{
		damagedWalls.at(i).DrawDamage(wallsSh, wallsTransformLocation, transform, damageTextures);
	}

}

void createWall(int x
	, int y
	, bool render
	, unsigned short int IDOfItemWall
	, std::vector<std::vector<wall>>& walls
	, unsigned int* texturesIDs)
{
	int indexToPlace = 0;
	for (; indexToPlace < walls.at(x).size(); indexToPlace++)
	{
		if (walls.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}
	switch (IDOfItemWall)
	{
	case i_WallDirt:
		walls.at(x).emplace(walls.at(x).begin() + indexToPlace, texturesIDs[t_Dirt], render, i_WallDirt, y, 20);
		break;
	case i_WallIce:
		walls.at(x).emplace(walls.at(x).begin() + indexToPlace, texturesIDs[t_Ice], render, i_WallIce, y, 15);
		break;
	}
}

void CreateBlock(int x
	, int y
	, unsigned short int IDOfItemBlock
	, std::vector<std::vector<Block>>& blocks
	//, std::vector<bool> isThereSandOnX
	, unsigned int* texturesIDs)
{
	int indexToPlace = 0;
	for (; indexToPlace < blocks.at(x).size(); indexToPlace++)
	{
		if (blocks.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}

	switch (IDOfItemBlock)
	{
	case i_Dirt:
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_Dirt], y, b_BasicSolid, 15, i_Dirt);
		break;
	case i_Platform:
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_Platform], y, b_Platform, 20, i_Platform);
		break;
	case i_Asphalt:
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_Asphalt], y, b_Asphalt, 35, i_Asphalt);
		break;
	case i_Ice:
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_Ice], y, b_Slippery, 15, i_Ice);
		break;
	case i_ForestPlank:
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_ForestPlank], y, b_BasicSolid, 20, i_ForestPlank);
		break;
	case i_Sand:
//		isThereSandOnX.at(x) = true;
		blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, texturesIDs[t_Sand], y, b_BasicSolid, 20, i_Sand);
		break;
	}

}

int FindBlock(std::vector<std::vector<Block>>& blocks
	, int x
	, int y
	, bool& found)
{
	found = false;
	for (int i = 0; i < blocks.at(x).size(); i++)
	{
		if (blocks.at(x).at(i).m_Y == y)
		{
			found = true;
			return i;
		}
		if (blocks.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return -1;
}
bool blockInArea(std::vector<std::vector<Block>>& blocks
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < blocks.at(j).size(); i++)
		{
			if (blocks.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (blocks.at(j).at(i).m_Y <= vertices[1])
			{
				return true;
			}

		}
	}
	return false;
}
int FindWall(std::vector<std::vector<wall>>& walls
	, int x
	, int y
	, bool& found)
{
	found = false;
	for (int i = 0; i < walls.at(x).size(); i++)
	{
		if (walls.at(x).at(i).m_Y == y)
		{
			found = true;
			return i;
		}
		if (walls.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return -1;
}
bool WallInArea(std::vector<std::vector<wall>>& walls
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < walls.at(j).size(); i++)
		{
			if (walls.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (walls.at(j).at(i).m_Y <= vertices[1])
			{
				return true;
			}

		}
	}
	return false;
}
