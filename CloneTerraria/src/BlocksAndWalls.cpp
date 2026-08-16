#include"BlocksAndWalls.h"


#include<fstream>
#include<sstream>
#include<string>
#include<cmath>

#include"opengl/ErrorSystem.h"
#include"opengl/Texture.h"
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
	behaviours[t_Air] = b_Air;
	behaviours[t_LightWoodHor] = b_Air;
	behaviours[t_LightWoodVer] = b_Air;
	behaviours[t_DarkWoodHor] = b_Air;
	behaviours[t_DarkWoodVer] = b_Air; 
	behaviours[t_CactusVer] = b_Air;
	behaviours[t_CactusTop] = b_Air;
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
	types[i_CactusPlank] = t_CactusPlank;
	types[i_WallCactusPlank] = t_CactusPlank;
	types[i_WallSnowPlank] = t_SnowPlank;
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
	WallIDs[t_CactusPlank] = i_CactusPlank;
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
	WallIDs[t_SnowPlank] = i_WallSnowPlank;
	WallIDs[t_CactusPlank] = i_WallCactusPlank;
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
	blocksIDs[t_LightWoodHor] = i_ForestPlank;
	blocksIDs[t_LightWoodVer] = i_ForestPlank;
	blocksIDs[t_DarkWoodHor] = i_SnowPlank;
	blocksIDs[t_DarkWoodVer] = i_SnowPlank;
	blocksIDs[t_SnowPlank] = i_SnowPlank;
	blocksIDs[t_CactusVer] = i_CactusPlank;
	blocksIDs[t_CactusTop] = i_CactusSapling;
	blocksIDs[t_CactusPlank] = i_CactusPlank;
	blocksIDs[t_Snow] = i_Snow;
	blocksIDs[t_HeartBlock] = i_HeartGem;
	return blocksIDs[blocksType];
}
unsigned char GetHardnessBytype(unsigned char type)
{
	unsigned char hardness[t_BlocksSize];
	for (int i = 0; i < t_BlocksSize;i++)
	{
		hardness[i] = 20;
	}
	hardness[t_Asphalt] = 40;
	hardness[t_Platform] = 30;
	hardness[t_ForestPlank] = 30;
	hardness[t_CactusPlank] = 30;
	hardness[t_SnowPlank] = 30;
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
	return hardness[type];
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


void Block::Create(unsigned char Type)
{
	m_Type = Clamp(Type, 0, t_BlocksSize - 1);
	m_Behavior = getBehaviorByType(m_Type);

}
Block::Block(unsigned char blockType)
{
	Create(blockType);
}

void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[t_Air] = CreateTextureRepeatRGBA("res/textures/red.png");
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
	IDs[t_ForestPlank] = CreateTextureRepeatRGBA("res/textures/forestplank.png");
	IDs[t_Sand] = CreateTextureRepeatRGBA("res/textures/sand.png");
	IDs[t_DoorBlock] = CreateTextureRepeatRGBA("res/textures/red.png");
	IDs[t_CopperOre] = CreateTextureRepeatRGBA("res/textures/copper.png");
	IDs[t_IronOre] = CreateTextureRepeatRGBA("res/textures/iron.png");
	IDs[t_AdamantiteOre] = CreateTextureRepeatRGBA("res/textures/AdamantiteOre.png");
	IDs[t_GoldOre] = CreateTextureRepeatRGBA("res/textures/GoldOre.png");
	IDs[t_TitanOre] = CreateTextureRepeatRGBA("res/textures/TitanOre.png");
	IDs[t_Stone] = CreateTextureRepeatRGBA("res/textures/stone.png");
	IDs[t_Cloude] = CreateTextureRepeatRGBA("res/textures/Cloude.png");
	IDs[t_StoneBrick] = CreateTextureRepeatRGBA("res/textures/StoneBrick.png");
	IDs[t_HardStone] = CreateTextureRepeatRGBA("res/textures/HardStone.png");
	IDs[t_HardStoneBrick] = CreateTextureRepeatRGBA("res/textures/HardStoneBrick.png");
	IDs[t_SandBrick] = CreateTextureRepeatRGBA("res/textures/sandBrick.png");
	IDs[t_Terracotta] = CreateTextureRepeatRGBA("res/textures/terracotta.png");
	IDs[t_SnowPlatform] = CreateTextureRepeatRGBA("res/textures/SnowPlatform.png");
	IDs[t_SandPlatform] = CreateTextureRepeatRGBA("res/textures/SandPlatform.png");
	IDs[t_Snow] = CreateTextureRepeatRGBA("res/textures/Snow.png");
	IDs[t_HeartBlock] = CreateTextureRepeatRGBA("res/textures/HeartBlock.png");
	IDs[t_SandStone] = CreateTextureRepeatRGBA("res/textures/SandStone.png");
	IDs[t_LightWoodHor] = CreateTextureRepeatRGBA("res/textures/lightWoodHor.png");
	IDs[t_LightWoodVer] = CreateTextureRepeatRGBA("res/textures/woodLog.png");
	IDs[t_DarkWoodHor] = CreateTextureRepeatRGBA("res/textures/darkWoodHor.png");
	IDs[t_DarkWoodVer] = CreateTextureRepeatRGBA("res/textures/darkWoodVer.png");
	IDs[t_CactusPlank] = CreateTextureRepeatRGBA("res/textures/cactusPlank.png");
	IDs[t_CactusVer] = CreateTextureRepeatRGBA("res/textures/cactusVer.png");
	IDs[t_CactusTop] = CreateTextureRepeatRGBA("res/textures/cactusTop.png");
	IDs[t_SnowPlank] = CreateTextureRepeatRGBA("res/textures/snowPlank.png");
}

void createWall(int x
	, int y
	, unsigned short int wallType
	, std::vector<int>& chunksToRebuildWalls
	, std::vector<std::vector<uint8_t>>& walls)
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
	walls.at(x).at(y - Blocks::yMin) = wallType;
}

void DestroyWall(std::vector<std::vector<uint8_t>>& walls
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

	walls.at(x).at(y -Blocks::yMin) = t_Air;
}

void CreateBlock(int x
	, int y
	, unsigned short int blockType
	, std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX)
{
	
 
	
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

	blocks.at(x).at(y-Blocks::yMin).Create(blockType);
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

	
	if (blocks.at(x).at(y - Blocks::yMin).m_Behavior == b_Sand)
	{
	
		bool isThereSand = false;
		for (int i = 0; i < blocks.at(x).size(); i++)
		{
			if (blocks.at(x).at(i).m_Behavior == b_Sand && i + Blocks::yMin != y)
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
	blocks.at(x).at(y - Blocks::yMin).Create(t_Air);
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
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = vertices[3]; i <= vertices[1]; i++)
		{
			if (blocks.at(j).at(i - Blocks::yMin).m_Type != t_Air)
			{
				return true;
			}
		}
	}
	return false;
}

bool FindWall(std::vector<std::vector<uint8_t>>& walls
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = vertices[3]; i < vertices[1]; i++)
		{
			if (walls.at(j).at(i - Blocks::yMin) != t_Air)
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
	, std::vector<std::vector<uint8_t>>& walls)
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
			for (int j = transform[1]; j < transform[1] + 20;j++)
			{

				unsigned char blockType = blocks.at(k).at(j - Blocks::yMin).m_Type;
				if (blockType < t_BlocksSize - 1 && blockType > 0)
				{
					map[blockType][k - transform[0]][j - transform[1]] = true;
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
	, std::vector<std::vector<uint8_t>>& walls)
{
	for (int l = 0;l < chunksToRebuild.size();l++)
	{
		int i = chunksToRebuild[l];
		bool map[t_BlocksSize - 1][20][20] = {};
		int transform[2] = { (i - (i / 54) * 54) * 20,((i / 54) * 20) + Blocks::yMin };
	
		for (int k = transform[0];k < transform[0] + 20;k++)
		{
			for (int j = transform[1]; j < transform[1] + 20;j++)
			{

				unsigned char blockType = walls.at(k).at(j - Blocks::yMin);
				if (blockType < t_BlocksSize - 1 && blockType > 0)
				{
					map[blockType][k - transform[0]][j - transform[1]] = true;
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
		float edgeChunks[4] = { static_cast<float>(FindChunk(cameraVertices[0], cameraVertices[3]))
			, static_cast<float>(FindChunk(cameraVertices[2], cameraVertices[3]))
			, static_cast<float>(FindChunk(cameraVertices[2], cameraVertices[1]))
			, static_cast<float>(FindChunk(cameraVertices[0], cameraVertices[1])) };
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




void LightMapAlgorithm(int *lightMapSpace
, std::vector<int>& Stack
, std::vector<std::vector<uint8_t>>& walls
, std::vector<std::vector<Block>>& blocks
, std::vector<std::vector<float>> &StaticLightMap)
{
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		for (int j = lightMapSpace[1]; j >= lightMapSpace[3];j--)
		{
			if (j < 0)
			{
				break;
			}
			if (walls.at(i).at(j-Blocks::yMin) == t_Air)
			{
				StaticLightMap.at(i).at(j - Blocks::yMin) = 1;
				Stack.emplace_back(i);
				Stack.emplace_back(j - Blocks::yMin);
			}
		}
	}
	while (Stack.size() != 0)
	{
		int IndexY = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		int IndexX = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		float baseLight = StaticLightMap.at(IndexX).at(IndexY);

		int rules[4] = { (lightMapSpace[0] == IndexX) , -(lightMapSpace[1] - Blocks::yMin == IndexY), -(lightMapSpace[2] == IndexX), (lightMapSpace[3] - Blocks::yMin == IndexY) };
		{
			int Table[2][4] = { {0,-1 + rules[0],0,1 + rules[2]}, {-1 + rules[3],0,1 + rules[1],0}};
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];
				int lightBlock = 2;
				if (blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Air)
				{
					lightBlock = 1;
				}
				float hold = baseLight - 0.08f * lightBlock;
				if (hold > StaticLightMap.at(CheckingX).at(CheckingY))
				{
					StaticLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}

		{
			int Table[2][4] = { {-1 + rules[0],-1 + rules[0],1 + rules[2],1 + rules[2]}, {-1 + rules[3],1 +rules[1],1 + rules[1] ,-1 + rules[3]}};
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];
				int lightBlock = 2;
				if (blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Air)
				{
					lightBlock = 1;
				}
				float hold = baseLight - 0.08f * 1.4142f * lightBlock;
				if (hold > StaticLightMap.at(CheckingX).at(CheckingY))
				{
					StaticLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}
	}
}
void CalculateLightMap(int chunkNumber
, std::vector<std::vector<Block>>& blocks
, std::vector<std::vector<uint8_t>>& walls
, std::vector<std::vector<float>>& StaticLightMap)
{
	int lightMapSpace[4] = { (chunkNumber - (chunkNumber / 54) * 54) * 20-14,((chunkNumber / 54) * 20) + Blocks::yMin + 34,(chunkNumber - (chunkNumber / 54) * 54) * 20+34,((chunkNumber / 54) * 20) + Blocks::yMin-14 };
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			if (lightMapSpace[i] >= Blocks::xMax)
			{
				lightMapSpace[i] = Blocks::xMax-1;
			}
			else if (lightMapSpace[i] < Blocks::xMin)
			{
				lightMapSpace[i] = Blocks::xMin;
			}
		}
		else
		{
			if (lightMapSpace[i] >= Blocks::yMax)
			{
				lightMapSpace[i] = Blocks::yMax-1;
			}
			else if (lightMapSpace[i] < Blocks::yMin)
			{
				lightMapSpace[i] = Blocks::yMin;
			}
		}
	}
	int lightMapHeight = lightMapSpace[1] - lightMapSpace[3];
	int lightMapWidth = lightMapSpace[2] - lightMapSpace[0];
	std::vector<int> Stack;
	for (int i = lightMapSpace[0]+1; i < lightMapSpace[2]; i++)
	{
		for (int j = lightMapSpace[3] + 1; j < lightMapSpace[1]; j++)
		{
			StaticLightMap.at(i).at(j-Blocks::yMin) = 0;
		}
	}
	for (int i = lightMapSpace[3]; i <= lightMapSpace[1];i++)
	{
		Stack.emplace_back(lightMapSpace[0]);
		Stack.emplace_back(i - Blocks::yMin);
		Stack.emplace_back(lightMapSpace[2]);
		Stack.emplace_back(i - Blocks::yMin);
	}
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2]; i++)
	{
		Stack.emplace_back(i);
		Stack.emplace_back(lightMapSpace[1] - Blocks::yMin);
		Stack.emplace_back(i);
		Stack.emplace_back(lightMapSpace[3] - Blocks::yMin);
	}
	LightMapAlgorithm(lightMapSpace, Stack, walls, blocks, StaticLightMap);

}

void CalculateLightMap(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap)
{
	int lightMapSpace[4] = {Blocks::xMin,Blocks::yMax-1,Blocks::xMax-1,Blocks::yMin };
	std::vector<int> Stack;
	LightMapAlgorithm(lightMapSpace, Stack, walls, blocks, StaticLightMap);
}

void CreateLightMap(std::vector<std::vector<float>>& StaticLightMap
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform
	, int x
	, int y
	, int width
	, int height
	, unsigned int &iD)
{
	
	y -= Blocks::yMin;
	std::vector<uint8_t> data;
	data.assign(width * height,0);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i <  width; i++)
		{
		
			data.at(width*j + i) = ((uint8_t)(StaticLightMap.at(x + i).at(y + j)*255));

		}
	}

	std::vector<float> dynamicStack;
	/////////////////
	dynamicStack.emplace_back(playerTransform[0]);
	dynamicStack.emplace_back(playerTransform[1]);
	dynamicStack.emplace_back(1);
	///////////
	
	
	
	while (dynamicStack.size() != 0)
	{
		float emittingLight = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightY = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightX = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();

		float altTransform[2] = {lightX - x, lightY - (y + Blocks::yMin) }; 

		int index[2] = {static_cast<int>(round(lightX)) - x, static_cast<int>(round(lightY) - (y + Blocks::yMin)) };
		int maxLeanght = ceil(emittingLight/0.08f)+1;
		int SpaceOfLight[4] = {index[0] - maxLeanght, index[1] +maxLeanght, index[0] + maxLeanght, index[1] - maxLeanght};
		SpaceOfLight[0] = Clamp(SpaceOfLight[0], 0, width);
		SpaceOfLight[1] = Clamp(SpaceOfLight[1], 0, height);
		SpaceOfLight[2] = Clamp(SpaceOfLight[2], 0, width);
		SpaceOfLight[3] = Clamp(SpaceOfLight[3], 0, height);
		

		for (int i = SpaceOfLight[0]; i < SpaceOfLight[2]; i++)
		{
			for (int j = SpaceOfLight[3]; j <  SpaceOfLight[1]; j++)
			{
			
				int blockValue = 1;
				int searchIndex[2] = { i,j };
				float searchAdd[2] = {};
				if (searchIndex[1] != index[1] && searchIndex[0] != index[0])
				{
					
					float searchScale[2] = {};
					float searchLeanghts[2] = {};
					float leanght[3] = { altTransform[0] - searchIndex[0],altTransform[1] -searchIndex[1],Pyt2D(searchIndex[0] - altTransform[0], searchIndex[1] - altTransform[1])};
					searchAdd[0] = (leanght[0]) / std::abs(leanght[0]);
					searchAdd[1] = (leanght[1]) / std::abs(leanght[1]);
					searchScale[0] = Pyt2D( 1 ,leanght[1] / leanght[0]);
					searchScale[1] = Pyt2D(1, leanght[0] / leanght[1]);
					searchLeanghts[0] = searchScale[0];
					searchLeanghts[1] = searchScale[1];

					
					while (leanght[2] >= searchLeanghts[0] || leanght[2] >= searchLeanghts[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air)
						{
							blockValue = 2;
							break;
						}
						if (searchLeanghts[0] > searchLeanghts[1])
						{
							searchLeanghts[1] += searchScale[1];
							searchIndex[1] += searchAdd[1];
						}
						else
						{
							searchLeanghts[0] += searchScale[0];
							searchIndex[0] += searchAdd[0];
						}
					}
				}
				else if (searchIndex[0] != index[0])
				{
					searchAdd[0] = (index[0] - searchIndex[0]) / abs(index[0] - searchIndex[0]);
					while (searchIndex[0] != index[0])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air)
						{
							blockValue = 2;
							break;
						}
						searchIndex[0] += searchAdd[0];
					}
				}
				else if (searchIndex[1] != index[1])
				{
					searchAdd[1] = (index[1] - searchIndex[1]) / abs(index[1] - searchIndex[1]);
					while (searchIndex[1] != index[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air)
						{
							blockValue = 2;
							break;
						}
						searchIndex[1] += searchAdd[1];
					}
				}
				float hold = emittingLight - 0.08f * blockValue * Pyt2D(altTransform[0]- i, altTransform[1]-j);
				if (hold * 255 > data.at(width * j + i))
				{
					data.at(width * j + i) = hold * 255;
				}
			}
		}



	}
	
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data()));
}