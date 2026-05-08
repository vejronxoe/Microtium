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
#include"math/VectorOperation.h"

namespace Blocks
{
	int xMax;
	int xMin;
	int yMax;
	int yMin;
};

unsigned char getBehaviorByType(unsigned char blocksType)
{
	blocksType = Clamp(blocksType,0,t_BlocksSize-1);
	unsigned char behaviours[t_BlocksSize] = { b_BasicSolid };
	behaviours[t_Ice] = b_Slippery;
	behaviours[t_Asphalt] = b_Asphalt;
	behaviours[t_Platform] = b_Platform;
	behaviours[t_DoorBlock] = b_Indestructible;
	return behaviours[blocksType];
	

}
unsigned char getTypeByItem(unsigned char item)
{
	item = Clamp(item, 0, i_ItemSize-1);
	int types[i_ItemSize] = {t_Dirt};
	types[i_Ice] = t_Ice;
	types[i_Asphalt] = t_Asphalt;
	types[i_Platform] = t_Platform;
	types[i_ForestPlank] = t_ForestPlank;
	types[i_Sand] = t_Sand;
	return types[item];
}
unsigned char GetWallItemBytype(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char wallIDs[t_BlocksSize] = {i_WallDirt};
	wallIDs[t_Ice] = i_WallIce;
	return wallIDs[blocksType];

}
unsigned char GetBlockItemByType(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char blocksIDs[t_BlocksSize] = { i_Dirt };
	blocksIDs[t_Ice] = i_Ice;
	blocksIDs[t_Asphalt] = i_Asphalt;
	blocksIDs[t_Platform] = i_Platform;
	blocksIDs[t_ForestPlank] = i_ForestPlank;
	blocksIDs[t_Sand] = i_Sand;
	return blocksIDs[blocksType];
}


DamagedBlock::DamagedBlock(int x
	, int y
	, char HP)
	:m_Transform{ x, y }, m_HP(HP)
{
}
void DamagedBlock::DrawDamage(Shader& basicShader
	, float* transform
	, unsigned int* texture)
{

	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[m_HP - 1]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}



Block::Block(unsigned char blockType
	, int y)
{
	blockType = Clamp(blockType, 0, t_BlocksSize -1);
	m_Y = y;
	m_BlockType = blockType;
	m_BlockBehavior = getBehaviorByType(blockType);
	unsigned char hardness[t_BlocksSize] = { 15 };
	hardness[t_Asphalt] = 35;
	hardness[t_Platform] = 20;
	hardness[t_ForestPlank] = 20;
	m_Hardness = hardness[blockType];
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


void drawBlocks(std::vector<std::vector<Block>>& blocks
	, std::vector<DamagedBlock> damagedBlocks
	, float* cameraCoordinate
	, Shader& basicSh
	, unsigned int* damageTexture
	, float* transform
	, float* camera)
{
	basicSh.Bind();
	basicSh.SetUniformMat4(basicCamera, camera);

	for (int j = floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform); j <= ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform); j++)
	{
		for (int i = 0; i < blocks.at(j).size(); i++)
		{
			int y = blocks.at(j).at(i).m_Y;
			if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) > y)
			{
				break;
			}
			else if (ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= y)
			{
				blocks.at(j).at(i).DrawBlock(basicSh, j, transform);

			}
		}
	}

	for (int i = 0; i < damagedBlocks.size(); i++)
	{
		damagedBlocks.at(i).DrawDamage(basicSh, transform, damageTexture);
	}

}
void drawWalls(std::vector<DamagedBlock> damagedWalls
	, unsigned int* damageTextures
	, std::vector<std::vector<wall>>& walls
	, Shader& wallsSh
	, float* camera
	, float* transform
	, float* cameraCoordinate)
{
	wallsSh.Bind();
	wallsSh.SetUniform1i(basicSize + ShadowLocation, 1);
	wallsSh.SetUniformMat4(basicCamera, camera);
	for (int j = floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform); j <= ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform); j++)
	{
		for (int i = 0; i < walls[j].size(); i++)
		{
			if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) > walls[j][i].m_Y)
			{
				break;
			}
			else if (ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= walls[j][i].m_Y)
			{
				walls[j][i].drawWalls(wallsSh, j, transform);
			}
		}
	}

	for (int i = 0; i < damagedWalls.size(); i++)
	{
		damagedWalls.at(i).DrawDamage(wallsSh, transform, damageTextures);
	}

}

void createWall(int x
	, int y
	, unsigned short int IDOfItemWall
	, std::vector<std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks
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
	bool isThereBlock;
	int blockIndex = FindBlock(blocks, x, y, isThereBlock);
	if (isThereBlock)
	{
		if (blocks.at(x).at(blockIndex).m_BlockBehavior == b_Platform)
		{
			isThereBlock = false;
		}
	}
	switch (IDOfItemWall)
	{
	case i_WallDirt:
		walls.at(x).emplace(walls.at(x).begin() + indexToPlace, texturesIDs[t_Dirt], !isThereBlock, i_WallDirt, y, 20);
		break;
	case i_WallIce:
		walls.at(x).emplace(walls.at(x).begin() + indexToPlace, texturesIDs[t_Ice], !isThereBlock, i_WallIce, y, 15);
		break;
	}
}


void CreateBlock(int x
	, int y
	, unsigned short int blockType
	, std::vector<std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX
	, unsigned int* texturesIDs)
{
	
	bool isThereWall;
	int	indexOfTheWall = FindWall(walls, x, y, isThereWall);
	if (t_Platform != blockType && t_DoorBlock != blockType)
	{
		if (isThereWall)
		{
			walls.at(x).at(indexOfTheWall).m_Render = false;
		}
	}
	if (blockType == t_Sand)
	{
		bool notThere = true;
		for (int i = 0; i < isThereSandOnX.size();i++)
		{
			if (x == isThereSandOnX[i])
			{
				notThere = false;
				break;
			}
		}
		if (notThere)
		{
			isThereSandOnX.emplace_back(x);
		}
	}
	int indexToPlace;
	for (indexToPlace = 0; indexToPlace < blocks.at(x).size(); indexToPlace++)
	{
		if (blocks[x][indexToPlace].m_Y < y)
		{
			break;
		}
	}
	blocks[x].emplace(blocks[x].begin() + indexToPlace, blockType,y);
	
}

void DestroyBlock(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<int>& isThereSandOnX
	, int x
	, int y)
{
	bool isThererBlock = false;
	int index = FindBlock(blocks, x, y, isThererBlock);

	if (isThererBlock)
	{
		if (blocks.at(x).at(index).m_BlockBehavior != b_Platform && blocks.at(x).at(index).m_BlockBehavior != b_Door)
		{
			bool isThereWall;
			int	indexOfTheWall = FindWall(walls, x, y, isThereWall);
			if (isThereWall)
			{
				walls.at(x).at(indexOfTheWall).m_Render = true;
			}
		}
		if (blocks.at(x).at(index).m_BlockBehavior == b_Sand)
		{
		
			bool isThereSand = false;
			for (int i = 0; i < blocks.at(x).size(); i++)
			{
				if (blocks.at(x).at(i).m_BlockBehavior == b_Sand && blocks.at(x).at(i).m_Y != y)
				{
					isThereSand = true;
				}
			}
			if (!isThereSand)
			{
				for (int i = 0; i < isThereSandOnX.size();i++)
				{
					if (isThereSandOnX.at(i) == x)
					{
						isThereSandOnX.erase(isThereSandOnX.begin() + i);
						break;
					}
				}
			}
		}
		blocks.at(x).erase(blocks.at(x).begin() + index);
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
		for (int i = 0; i < blocks[j].size(); i++)
		{
			if (blocks[j][i].m_Y < vertices[3])
			{
				break;
			}
			if (blocks[j][i].m_Y <= vertices[1])
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
