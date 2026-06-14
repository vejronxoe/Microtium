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
	unsigned char behaviours[t_BlocksSize];
	for (int i = 0; i < t_BlocksSize; i++)
	{
		behaviours[i] = b_BasicSolid;
	}
	behaviours[t_Ice] = b_Slippery;
	behaviours[t_Asphalt] = b_Asphalt;
	behaviours[t_Platform] = b_Platform;
	behaviours[t_DoorBlock] = b_Indestructible;
	behaviours[t_Sand] = b_Sand;
	behaviours[t_SnowPlatform] = b_Platform;
	behaviours[t_SandPlatform] = b_Platform;
	return behaviours[blocksType];
	

}
unsigned char getTypeByItem(unsigned char item)
{
	item = Clamp(item, 0, i_ItemSize-1);
	int types[i_ItemSize] = {};
	for (int i = 0; i < i_ItemSize; i++)
	{
		types[i] = t_Dirt;
	}
	types[i_Ice] = t_Ice;
	types[i_Asphalt] = t_Asphalt;
	types[i_Platform] = t_Platform;
	types[i_ForestPlank] = t_ForestPlank;
	types[i_Sand] = t_Sand;
	types[i_WallIce] = t_Ice;
	types[i_CopperOre] = t_CopperOre;
	types[i_IronOre] = t_IronOre;
	types[i_AdamantiteOre] = t_AdamantiteOre;
	types[i_GoldOre] = t_GoldOre;
	types[i_Stone] = t_Stone;
	types[i_CloudeBlock] = t_Cloude;
	types[i_TitanOre] = t_TitanOre;
	types[i_StoneBrick] = t_StoneBrick;
	types[i_HardStone] = t_HardStone;
	types[i_HardStoneBrick] = t_HardStoneBrick;
	types[i_SandBrick] = t_SandBrick;
	types[i_Terracotta] = t_Terracotta;
	types[i_SnowPlatform] = t_SnowPlatform;
	types[t_SandPlatform] = t_SandPlatform;
	types[i_WallForestPlank] = t_ForestPlank;
	types[i_WallCloude] = t_Cloude;
	types[i_WallTerracotta] = t_Terracotta;
	types[i_WallStone] = t_Stone;
	types[i_WallStoneBrick] = t_StoneBrick;
	types[i_WallHardStone] = t_HardStone;
	types[i_WallHardStoneBrick] = t_HardStoneBrick;
	types[i_WallSandBrick] = t_SandBrick;
	types[i_WallSnow] = t_Snow;
	types[i_Snow] = t_Snow;
	types[i_HeartGem] = t_HeartBlock;
	types[i_WallSandStone] = t_SandStone;
	types[i_SandStone] = t_SandStone;
	
	return types[item];
}
unsigned char GetWallItemBytype(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char WallIDs[t_BlocksSize] = {i_WallDirt};
	for (int i = 0; i < t_BlocksSize; i++)
	{
		WallIDs[i] = i_WallDirt;
	}
	WallIDs[t_Ice] = i_WallIce;
	WallIDs[t_ForestPlank] = i_WallForestPlank;
	WallIDs[t_Cloude] = i_WallCloude;
	WallIDs[t_Terracotta] = i_WallTerracotta;
	WallIDs[t_Stone] = i_WallStone;
	WallIDs[t_HardStone] = i_WallHardStone;
	WallIDs[t_HardStoneBrick] = i_WallHardStoneBrick;
	WallIDs[t_SandBrick] = i_WallSandBrick;
	WallIDs[t_StoneBrick] = i_WallStoneBrick;
	WallIDs[t_SandStone] = i_WallSandStone;
	WallIDs[t_Snow] = i_WallSnow;
	return WallIDs[blocksType];

}
unsigned char GetBlockItemByType(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char blocksIDs[t_BlocksSize] = { i_Dirt };
	for (int i = 0; i < t_BlocksSize; i++)
	{
		blocksIDs[i] = i_Dirt;
	}
	blocksIDs[t_Ice] = i_Ice;
	blocksIDs[t_Asphalt] = i_Asphalt;
	blocksIDs[t_Platform] = i_Platform;
	blocksIDs[t_ForestPlank] = i_ForestPlank;
	blocksIDs[t_Sand] = i_Sand;
	blocksIDs[t_CopperOre] = i_CopperOre;
	blocksIDs[t_IronOre] = i_IronOre;
	blocksIDs[t_AdamantiteOre] = i_AdamantiteOre;
	blocksIDs[t_GoldOre] = i_GoldOre;
	blocksIDs[t_Stone] = i_Stone;
	blocksIDs[t_Cloude] = i_CloudeBlock;
	blocksIDs[t_TitanOre] = i_TitanOre;
	blocksIDs[t_StoneBrick] = i_StoneBrick;
	blocksIDs[t_HardStone] = i_HardStone;
	blocksIDs[t_HardStoneBrick] = i_HardStoneBrick;
	blocksIDs[t_SandBrick] = i_SandBrick;
	blocksIDs[t_Terracotta] = i_Terracotta;
	blocksIDs[t_SnowPlatform] = i_SnowPlatform;
	blocksIDs[t_SandPlatform] = i_SandPlatform;
	blocksIDs[t_SandStone] = i_SandStone;

	blocksIDs[t_Snow] = i_Snow;
	blocksIDs[t_HeartBlock] = i_HeartGem;
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
	m_Type = blockType;
	m_Behavior = getBehaviorByType(blockType);
	unsigned char hardness[t_BlocksSize] ;
	for (int i = 0 ; i< t_BlocksSize;i++)
	{
		hardness[i] = 20;
	}
	hardness[t_Asphalt] = 40;
	hardness[t_Platform] = 30;
	hardness[t_ForestPlank] = 30;
	hardness[t_CopperOre] = 20;
	hardness[t_SandStone] = 40;
	hardness[t_IronOre] = 35;
	hardness[t_AdamantiteOre] = 100;
	hardness[t_GoldOre] = 20;
	hardness[t_Stone] = 35;
	hardness[t_Cloude] = 10;
	hardness[t_TitanOre] = 50;
	hardness[t_StoneBrick] = 35;
	hardness[t_HardStone] = 100;
	hardness[t_HardStoneBrick] = 100;
	hardness[t_SandBrick] = 35;
	hardness[t_Terracotta] = 40;
	hardness[t_SnowPlatform] = 30;
	hardness[t_SandPlatform] = 30;
	hardness[t_Snow] = 20;
	hardness[t_HeartBlock] = 10;
	m_Hardness = hardness[blockType];
}
Wall::Wall(unsigned char wallType
	, int y)
{
	m_Type = wallType;
	m_Y = y;
	unsigned char hardness[t_BlocksSize];
	for (int i = 0; i < t_BlocksSize; i++)
	{
		hardness[i] = 20;
	}
	hardness[t_SandStone] = 40;
	hardness[t_ForestPlank] = 30;
	hardness[t_Stone] = 35;
	hardness[t_Cloude] = 10;
	hardness[t_StoneBrick] = 35;
	hardness[t_HardStone] = 100;
	hardness[t_HardStoneBrick] = 100;
	hardness[t_SandBrick] = 35;
	hardness[t_Terracotta] = 40;
	hardness[t_Snow] = 20;

	m_Hardness = hardness[wallType];
}
void Block::Draw(Shader& basicSh
	, unsigned int* blockTex
	, int x
	, float* transform)
{
	ChangeTransform(x, m_Y, transform);
	basicSh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[m_Type]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void Wall::Draw(Shader& shadowSh
	, unsigned int* blockTex
	, int x
	, float* transform)
{
	ChangeTransform(x, m_Y, transform);
	shadowSh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[m_Type]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[t_TopGrass] = CreateTextureRepeatRGBA("res/textures/topGrassBlock.png");
	IDs[t_LeftGrass] = CreateTextureRepeatRGBA("res/textures/leftGrassBlock.png");
	IDs[t_DownGrass] = CreateTextureRepeatRGBA("res/textures/downGrassBlock.png");
	IDs[t_RightGrass] = CreateTextureRepeatRGBA("res/textures/rightGrassBlock.png");
	IDs[t_TopDownGrass] = CreateTextureRepeatRGBA("res/textures/topDownGrassBlock.png");
	IDs[t_LeftRightGrass] = CreateTextureRepeatRGBA("res/textures/leftRightGrassBlock.png");
	IDs[t_TopLeftGrass] = CreateTextureRepeatRGBA("res/textures/topLeftGrassBlock.png");
	IDs[t_DownLeftGrass] = CreateTextureRepeatRGBA("res/textures/downLeftGrassBlock.png");
	IDs[t_DownRightGrass] = CreateTextureRepeatRGBA("res/textures/downRightGrassBlock.png");
	IDs[t_TopRightGrass] = CreateTextureRepeatRGBA("res/textures/topRightGrassBlock.png");
	IDs[t_MissingTopGrass] = CreateTextureRepeatRGBA("res/textures/missingTopGrassBlock.png");
	IDs[t_MissingLeftGrass] = CreateTextureRepeatRGBA("res/textures/missingLeftGrassBlock.png");
	IDs[t_MissingDownGrass] = CreateTextureRepeatRGBA("res/textures/missingDownGrassBlock.png");
	IDs[t_MissingRightGrass] = CreateTextureRepeatRGBA("res/textures/missingRightGrassBlock.png");
	IDs[t_FullGrass] = CreateTextureRepeatRGBA("res/textures/fullGrassBlock.png");
	IDs[t_Dirt] = CreateTextureRepeatRGBA("res/textures/dirtBlock.png");
	IDs[t_Ice] = CreateTextureRepeatRGBA("res/textures/ice.png");
	IDs[t_Asphalt] = CreateTextureRepeatRGBA("res/textures/Asphalt.png");
	IDs[t_Platform] = CreateTextureRepeatRGBA("res/textures/platform.png");
	IDs[t_Platform] = CreateTextureRepeatRGBA("res/textures/platform.png");
	IDs[t_ForestPlank] = CreateTextureRepeatRGBA("res/textures/forestPlank.png");
	IDs[t_Sand] = CreateTextureRepeatRGBA("res/textures/sand.png");
	IDs[t_DoorBlock] = CreateTextureRepeatRGBA("res/textures/red.png");
	IDs[t_CopperOre] = CreateTextureRepeatRGBA("res/textures/copper.png");
	IDs[t_IronOre] = CreateTextureRepeatRGBA("res/textures/iron.png");
	IDs[t_AdamantiteOre] = CreateTextureRepeatRGBA("res/textures/AdamantiteOre.png");
	IDs[t_GoldOre] = CreateTextureRepeatRGBA("res/textures/GoldOre.png");
	IDs[t_TitanOre] = CreateTextureRepeatRGBA("res/textures/TitanOre.png");
	IDs[t_Stone] = CreateTextureRepeatRGBA("res/textures/Stone.png");
	IDs[t_Cloude] = CreateTextureRepeatRGBA("res/textures/Cloude.png");
	IDs[t_StoneBrick] = CreateTextureRepeatRGBA("res/textures/StoneBrick.png");
	IDs[t_HardStone] = CreateTextureRepeatRGBA("res/textures/HardStone.png");
	IDs[t_HardStoneBrick] = CreateTextureRepeatRGBA("res/textures/HardStoneBrick.png");
	IDs[t_SandBrick] = CreateTextureRepeatRGBA("res/textures/SandBrick.png");
	IDs[t_Terracotta] = CreateTextureRepeatRGBA("res/textures/Terracotta.png");
	IDs[t_SnowPlatform] = CreateTextureRepeatRGBA("res/textures/SnowPlatform.png");
	IDs[t_SandPlatform] = CreateTextureRepeatRGBA("res/textures/sandPlatform.png");
	IDs[t_Snow] = CreateTextureRepeatRGBA("res/textures/Snow.png");
	IDs[t_HeartBlock] = CreateTextureRepeatRGBA("res/textures/HeartBlock.png");
	IDs[t_SandStone] = CreateTextureRepeatRGBA("res/textures/SandStone.png");
}

void createWall(int x
	, int y
	, unsigned short int wallType
	, std::vector<int>& chunksToRebuildWalls
	, std::vector<std::vector<Wall>>& walls)
{
	int indexToPlace;
	for (indexToPlace = 0; indexToPlace < walls.at(x).size(); indexToPlace++)
	{
		if (walls.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuildWalls.size(); i++)
	{
		if (chunksToRebuildWalls[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuildWalls.emplace_back(chunkIndex);
	}
	walls.at(x).emplace(walls.at(x).begin() + indexToPlace, wallType, y);
}

void DestroyWall(std::vector<std::vector<Wall>>& Walls
	, std::vector<int>& chunksToRebuildWalls
	, int x
	, int y)
{
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuildWalls.size(); i++)
	{
		if (chunksToRebuildWalls[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuildWalls.emplace_back(chunkIndex);
	}
	int index;
	if (FindWall(Walls, x, y, index))
	{
		Walls.at(x).erase(Walls.at(x).begin()+index);
	}

}

void CreateBlock(int x
	, int y
	, unsigned short int blockType
	, std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX)
{
	
	int	indexOfTheWall; 
	
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
		if (blocks.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}
	blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, blockType,y);
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuild.size(); i++)
	{
		if (chunksToRebuild[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere) 
	{
		chunksToRebuild.emplace_back(chunkIndex);
	}
}
void DestroyBlock(std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX
	, int x
	, int y)
{
	int index; 

	if (FindBlock(blocks, x, y, index))
	{
		if (blocks.at(x).at(index).m_Behavior == b_Sand)
		{
		
			bool isThereSand = false;
			for (int i = 0; i < blocks.at(x).size(); i++)
			{
				if (blocks.at(x).at(i).m_Behavior == b_Sand && blocks.at(x).at(i).m_Y != y)
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
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuild.size(); i++)
	{
		if (chunksToRebuild[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuild.emplace_back(chunkIndex);
	}
}

bool FindBlock(std::vector<std::vector<Block>>& blocks
	, int x
	, int y
	, int& index)
{
	index = -1;
	for (int i = 0; i < blocks.at(x).size(); i++)
	{
		if (blocks.at(x).at(i).m_Y == y)
		{
			index = i;
			return true;
		}
		if (blocks.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return false;
}
bool FindBlock(std::vector<std::vector<Block>>& blocks
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
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int x
	, int y
	, int& index)
{
	index = -1;
	for (int i = 0; i < Walls.at(x).size(); i++)
	{
		if (Walls.at(x).at(i).m_Y == y)
		{
			index = i;
			return true;
		}
		if (Walls.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return false;
}
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < Walls.at(j).size(); i++)
		{
			if (Walls.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (Walls.at(j).at(i).m_Y <= vertices[1])
			{
				return true;
			}

		}
	}
	return false;
}
int FindChunk(int x ,int y)
{
	return x / 20 + ((y-Blocks::yMin) / 20) * 54;
}
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks)
{
	std::vector<int> chunksToRebuild;

	for (int i = 0; i < 2322; i++)
	{
		chunksToRebuild.emplace_back(i);
		
	}
	CreateChunks(chunksToRebuild, chunks, blocks);
}
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls)
{
	std::vector<int> chunksToRebuild;
	for (int i = 0; i < 2322; i++)
	{
		chunksToRebuild.emplace_back(i);
	}
	CreateChunks(chunksToRebuild, chunks, walls);
}

void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks)
{
	for (int l = 0;l < chunksToRebuild.size();l++)
	{
		int i = chunksToRebuild[l];
	
		bool map[t_BlocksSize - 1][20][20] = {};
		int transform[2] = { (i - (i / 54) * 54) * 20,((i / 54) * 20) + Blocks::yMin };
		for (int k = transform[0];k < transform[0] + 20;k++)
		{
			for (int j = 0; j < blocks.at(k).size();j++)
			{

				if (transform[1] > blocks.at(k).at(j).m_Y)
				{
					break;
				}
				else if (transform[1] + 20 > blocks.at(k).at(j).m_Y)
				{
					unsigned char blockType = blocks.at(k).at(j).m_Type;
					if (blockType < t_BlocksSize - 1 && blockType >= 0)
					{
						map[blockType][k - transform[0]][blocks.at(k).at(j).m_Y - transform[1]] = true;
					}
				}

			}
		}
	

		for (int j = 0; j < t_BlocksSize - 1;j++)
		{
			std::vector<float> vertices;
			std::vector<unsigned short> order;
			float blockVertices[4];
			int uvVertices[4];
			for (int k = 0; k < 20;k++)
			{
				for (int o = 0;o < 20; o++)
				{
					if (map[j][k][o])
					{
						blockVertices[0] = transform[0] - 0.5f + k;
						blockVertices[1] = transform[1] + 0.5f + o;
						blockVertices[2] = transform[0] + 0.5f + k;
						blockVertices[3] = transform[1] - 0.5f + o;
						uvVertices[0] = 0;
						uvVertices[1] = 1;
						uvVertices[2] = 1;
						uvVertices[3] = 0;
						o++;
						while (map[j][k][o] && o < 20)
						{
							uvVertices[1]++;
							blockVertices[1]++;
							o++;
						}
						int goToLeft = 1;
						bool canGo = true;
						for (int p = o - uvVertices[1]; p < o; p++)
						{
							canGo = canGo && map[j][k + goToLeft][p];
						}
						while (canGo && goToLeft + k < 20)
						{
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								map[j][k + goToLeft][p] = false;
							}
							uvVertices[2]++;
							blockVertices[2]++;
							goToLeft++;
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								canGo = canGo && map[j][k + goToLeft][p];
							}

						}

						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[1]);
						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[1]);
					}
				}
			}
		
			if (vertices.size() != 0)
			{
				order.resize((vertices.size() / 16) * 6);

				for (int k = 0; k < vertices.size() / 16; k++)
				{
					order.emplace_back(0 + k * 4);
					order.emplace_back(1 + k * 4);
					order.emplace_back(2 + k * 4);
					order.emplace_back(0 + k * 4);
					order.emplace_back(3 + k * 4);
					order.emplace_back(2 + k * 4);
				}
			}
			if (chunks.at(i).m_VBO[j])
			{
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glDeleteVertexArrays(1, &chunks.at(i).m_VA[j]));
			}
			chunks.at(i).m_VA[j] = 0;
			chunks.at(i).m_VBO[j] = 0;
			chunks.at(i).m_EOB[j] = 0;
			chunks.at(i).m_EOBNumber[j] = 0;
			if (vertices.size())
			{
				ErrorGL(glGenVertexArrays(1, &chunks.at(i).m_VA[j]));
				ErrorGL(glBindVertexArray(chunks.at(i).m_VA[j]));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunks.at(i).m_EOB[j]));
				ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(unsigned short), order.data(), GL_STATIC_DRAW));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, chunks.at(i).m_VBO[j]));
				ErrorGL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));

				ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
				ErrorGL(glEnableVertexAttribArray(0));
				ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
				ErrorGL(glEnableVertexAttribArray(1));


				ErrorGL(glBindVertexArray(0));
				chunks.at(i).m_EOBNumber[j] = order.size();
			}
		}
	}
	chunksToRebuild.clear();

}
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls)
{
	for (int l = 0;l < chunksToRebuild.size();l++)
	{
		int i = chunksToRebuild[l];
		bool map[t_BlocksSize - 1][20][20] = {};
		int transform[2] = { (i - (i / 54) * 54) * 20,((i / 54) * 20) + Blocks::yMin };
		for (int k = transform[0];k < transform[0] + 20;k++)
		{
			for (int j = 0; j < walls.at(k).size();j++)
			{

				if (transform[1] > walls.at(k).at(j).m_Y)
				{
					break;
				}
				else if (transform[1] + 20 > walls.at(k).at(j).m_Y)
				{
					unsigned char blockType = walls.at(k).at(j).m_Type;
					if (blockType < t_BlocksSize - 1 && blockType >= 0)
					{
						map[blockType][k - transform[0]][walls.at(k).at(j).m_Y - transform[1]] = true;
					}
				}

			}
		}
		for (int j = 0; j < t_BlocksSize - 1;j++)
		{
			std::vector<float> vertices;
			std::vector<unsigned short> order;
			float blockVertices[4];
			int uvVertices[4];
			for (int k = 0; k < 20;k++)
			{
				for (int o = 0;o < 20; o++)
				{
					if (map[j][k][o])
					{
						blockVertices[0] = transform[0] - 0.5f + k;
						blockVertices[1] = transform[1] + 0.5f + o;
						blockVertices[2] = transform[0] + 0.5f + k;
						blockVertices[3] = transform[1] - 0.5f + o;
						uvVertices[0] = 0;
						uvVertices[1] = 1;
						uvVertices[2] = 1;
						uvVertices[3] = 0;
						o++;
						while (map[j][k][o] && o < 20)
						{
							uvVertices[1]++;
							blockVertices[1]++;
							o++;
						}
						int goToLeft = 1;
						bool canGo = true;
						for (int p = o - uvVertices[1]; p < o; p++)
						{
							canGo = canGo && map[j][k + goToLeft][p];
						}
						while (canGo && goToLeft + k < 20)
						{
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								map[j][k + goToLeft][p] = false;
							}
							uvVertices[2]++;
							blockVertices[2]++;
							goToLeft++;
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								canGo = canGo && map[j][k + goToLeft][p];
							}

						}

						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[1]);
						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[1]);
					}
				}
			}
				
			if (vertices.size() != 0)
			{
				order.resize((vertices.size() / 16) * 6);

				for (int k = 0; k < vertices.size() / 16; k++)
				{
					order.emplace_back(0 + k * 4);
					order.emplace_back(1 + k * 4);
					order.emplace_back(2 + k * 4);
					order.emplace_back(0 + k * 4);
					order.emplace_back(3 + k * 4);
					order.emplace_back(2 + k * 4);
				}
			}
			if (chunks.at(i).m_VBO[j])
			{
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glDeleteVertexArrays(1, &chunks.at(i).m_VA[j]));
			}
			chunks.at(i).m_VA[j] = 0;
			chunks.at(i).m_VBO[j] = 0;
			chunks.at(i).m_EOB[j] = 0;
			chunks.at(i).m_EOBNumber[j] = 0;
			if (vertices.size())
			{
				ErrorGL(glGenVertexArrays(1, &chunks.at(i).m_VA[j]));
				ErrorGL(glBindVertexArray(chunks.at(i).m_VA[j]));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunks.at(i).m_EOB[j]));
				ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(unsigned short), order.data(), GL_STATIC_DRAW));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, chunks.at(i).m_VBO[j]));
				ErrorGL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));

				ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
				ErrorGL(glEnableVertexAttribArray(0));
				ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
				ErrorGL(glEnableVertexAttribArray(1));


				ErrorGL(glBindVertexArray(0));
				chunks.at(i).m_EOBNumber[j] = order.size();
			}
		}
	}
	chunksToRebuild.clear();

}
void DrawChunks(Shader& shadowSh
	, unsigned int* textures
	, float* trasform
	, float* cameraTransform
	, std::vector<ChunkDD>& chunksBlock
	, std::vector<ChunkDD>& chunksWall)
{
	if (Window::height)
	{
		shadowSh.Bind();
		ChangeTransform(0, 0, trasform);
		shadowSh.SetUniformMat4(basicTransform, trasform);
		shadowSh.SetUniform1i(basicSize + ShadowLocation, 1);
		float cameraVertices[4] = { floorf(cameraTransform[0] - Window::halfWidthOfGameTransform)
			, ceilf(cameraTransform[1] + Window::halfHeightOfGameTransform)
			, ceilf(cameraTransform[0] + Window::halfWidthOfGameTransform)
			, floorf(cameraTransform[1] - Window::halfHeightOfGameTransform) };
		float edgeChunks[4] = { FindChunk(cameraVertices[0] ,cameraVertices[3])
			, FindChunk(cameraVertices[2], cameraVertices[3])
			, FindChunk(cameraVertices[2], cameraVertices[1])
			, FindChunk(cameraVertices[0], cameraVertices[1]) };
		float deltaChunks[2] = { edgeChunks[1] - edgeChunks[0] ,(edgeChunks[3] - edgeChunks[0]) / 54 };

		for (int i = 0; i <= deltaChunks[1];i++)
		{
			for (int j = 0; j <= deltaChunks[0]; j++)
			{
				int chunkIndex = edgeChunks[0] + j + i * 54;

				for (int k = 0; k < t_BlocksSize - 1;k++)
				{
					if (chunksWall[chunkIndex].m_VA[k])
					{
						ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[k]));
						ErrorGL(glBindVertexArray(chunksWall[chunkIndex].m_VA[k]));
						ErrorGL(glDrawElements(GL_TRIANGLES, chunksWall[chunkIndex].m_EOBNumber[k], GL_UNSIGNED_SHORT, 0));
					}
				}
			}
		}
		shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);

		for (int i = 0; i <= deltaChunks[1];i++)
		{
			for (int j = 0; j <= deltaChunks[0]; j++)
			{
				int chunkIndex = edgeChunks[0] + j + i * 54;

				for (int k = 0; k < t_BlocksSize - 1;k++)
				{
					if (chunksBlock[chunkIndex].m_VA[k])
					{
						ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[k]));
						ErrorGL(glBindVertexArray(chunksBlock[chunkIndex].m_VA[k]));
						ErrorGL(glDrawElements(GL_TRIANGLES, chunksBlock[chunkIndex].m_EOBNumber[k], GL_UNSIGNED_SHORT, 0));
					}
				}
			}
		}
	}

}
void ClaculateLightMap(int chunkNumber
, std::vector<std::vector<Block>>& blocks
, std::vector<std::vector<Wall>>& walls)
{
	int lightMapSpace[4] = {};
	int lightMapHeight = lightMapSpace[1] - lightMapSpace[3] + 1;
	int lightMapWitdh = lightMapSpace[2] - lightMapSpace[0] + 1;
	std::vector<std::vector<int>> blockMap;
	{
		std::vector<int> fill;
		for (int i = lightMapSpace[3]; i <= lightMapSpace[1];i++)
		{
			fill.emplace_back(1);
		}
		blockMap.assign(lightMapWitdh, fill);

	}
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		for (int j = 0; j < blocks.at(i).size(); j++)
		{
			if (blocks.at(i).at(j).m_Y < lightMapSpace[3])
			{
				break;
			}
			else if (blocks.at(i).at(j).m_Y <= lightMapSpace[1])
			{
				blockMap.at(i - lightMapSpace[0]).at(blocks.at(i).at(j).m_Y - lightMapSpace[3]) = 2;
			}
		}
	}
	std::vector<int> Stack;
	std::vector<std::vector<float>> biggerLightMap;
	{
		std::vector<float> fill;
		for (int i = lightMapSpace[3]; i <= lightMapSpace[1];i++)
		{
			fill.emplace_back(0);
		}
		biggerLightMap.assign(lightMapWitdh, fill);
	}
	

	for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		int height = lightMapSpace[1];
		for (int j = 0; j < walls.at(i).size();j++)
		{
			if (height < 0)
			{
				break;
			}
			while (walls.at(i).at(j).m_Y <= height)
			{


				if (walls.at(i).at(j).m_Y != height)
				{
					biggerLightMap.at(i - lightMapSpace[0]).at(lightMapHeight - 1 - (height - lightMapSpace[3])) = 1;
					Stack.emplace_back(i - lightMapSpace[0]);
					Stack.emplace_back(lightMapHeight - 1 - (height - lightMapSpace[3]));
				}
				if (height < 0)
				{
					break;
				}
				if (height < lightMapSpace[3])
				{
					break;
				}
				height--;
			}
			if (height < lightMapSpace[3])
			{
				break;
			}
		}
		while (height >= lightMapSpace[3])
		{

			if (height < 0)
			{
				break;
			}
			biggerLightMap.at(i - lightMapSpace[0]).at(lightMapHeight - 1 - (height - lightMapSpace[3])) = 1;
			Stack.emplace_back(i - lightMapSpace[0]);
			Stack.emplace_back(lightMapHeight - 1 - (height - lightMapSpace[3]));
			height--;
		}
	}
	while (Stack.size() != 0)
	{
		int IndexY = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		int IndexX = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		float baseLight = biggerLightMap.at(IndexX).at(IndexY);


		{
			int Table[2][4] = { {0,-1 + (0 == IndexX),0,1 - (lightMapWitdh - 1 == IndexX)}, {-1 + (0 == IndexY),0,1 - (lightMapHeight - 1 == IndexY),0} };
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];

				float hold = baseLight - 0.08f * blockMap.at(CheckingX).at(CheckingY);
				if (hold > biggerLightMap.at(CheckingX).at(CheckingY))
				{
					biggerLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}
		
		{
			int Table[2][4] = { {-1 + (0 == IndexX),-1 + (0 == IndexX),1 - (lightMapWitdh - 1 == IndexX),1 - (lightMapWitdh - 1 == IndexX)}, {-1 + (0 == IndexY),1 - (lightMapHeight - 1 == IndexY),1 - (lightMapHeight - 1 == IndexY),-1 + (0 == IndexY)} };
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];
				float hold = baseLight - 0.08f *1.41f * blockMap.at(CheckingX).at(CheckingY);
				if (hold > biggerLightMap.at(CheckingX).at(CheckingY))
				{
					biggerLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}
	}


}