#pragma once
#include<vector>

#include"Opengl/Shader.h"

#define SAFEDISTANCE 2.5f

namespace Blocks
{
	extern int xMax;
	extern int xMin;
	extern int yMax;
	extern int yMin;
}
enum blockTypes
{
	t_Air = 0
	, t_Dirt 
	, t_LeftGrass
	, t_DownGrass
	, t_DownLeftGrass
	, t_RightGrass
	, t_LeftRightGrass
	, t_DownRightGrass
	, t_MissingTopGrass
	, t_TopGrass
	, t_TopLeftGrass
	, t_TopDownGrass
	, t_MissingRightGrass
	, t_TopRightGrass
	, t_MissingDownGrass
	, t_MissingLeftGrass
	, t_FullGrass
	, t_Ice
	, t_Asphalt
	, t_Platform
	, t_ForestPlank
	, t_Sand
	, t_CopperOre
	, t_IronOre
	, t_AdamantiteOre
	, t_GoldOre
	, t_TitanOre
	, t_Stone
	, t_StoneBrick
	, t_HardStone
	, t_HardStoneBrick
	, t_SandBrick
	, t_Terracotta
	, t_Cloude
	, t_SnowPlatform
	, t_SandPlatform
	, t_Snow
	, t_HeartBlock
	, t_SandStone
	, t_LightWoodHor
	, t_LightWoodVer
	, t_DarkWoodHor
	, t_DarkWoodVer
	, t_CactusHor
	, t_CactusVer
	, t_CactusTop
	, t_SnowPlank
	, t_DoorBlock
	, t_BlocksSize

};
enum BlockBehavior
{
	b_Air = 0
	, b_Platform
	, b_Sand
	, b_Indestructible
	, b_BasicSolid
	, b_Slippery
	, b_Asphalt
	, b_Door
};

void CreateAllBlockTextures(unsigned int* IDs);
unsigned char getBehaviorByType(unsigned char blocksType);
unsigned char getTypeByItem(unsigned char item);
unsigned char GetWallItemBytype(unsigned char blocksType);
unsigned char GetBlockItemByType(unsigned char blocksType);
unsigned char GetHardnessBytype(unsigned char type);
class Block
{
public:
	unsigned char m_Type = t_Dirt;
	unsigned char m_Behavior = b_BasicSolid;

	Block(unsigned char blockType);
	void Create(unsigned char Type);

};

struct DamagedBlock
{
	DamagedBlock(int x
		, int y
		, char HP);
	int m_Transform[2];
	char m_HP;
	void DrawDamage(Shader& basicShader
		, float* transform
		, unsigned int* texture);
};

struct ChunkDD
{
	unsigned int m_VA[t_BlocksSize - 1] = {};
	unsigned int m_VBO[t_BlocksSize - 1] = {};
	unsigned int m_EOB[t_BlocksSize - 1] = {};
	unsigned int m_EOBNumber[t_BlocksSize - 1] = {};
};




void createWall(int x
	, int y
	, unsigned short int wallType
	, std::vector<int>& chunksToRebuildWalls
	, std::vector<std::vector<uint8_t>>& walls);
void DestroyWall(std::vector<std::vector<uint8_t>>& Walls
	, std::vector<int>& chunksToRebuildWalls
	, int x
	, int y);
void CreateBlock(int x
	, int y
	, unsigned short int blockType
	, std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX);
void DestroyBlock(std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX
	, int x
	, int y);

bool FindBlock(std::vector<std::vector<Block>>& blocks
	, int* vertices);
bool FindWall(std::vector<std::vector<uint8_t>>& Walls
	, int* vertices);
int FindChunk(int x, int y);
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks);
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<uint8_t>>& walls);
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks);
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<uint8_t>>& walls);
void DrawChunks(Shader& basicSh
	, unsigned int* textures
	, float* trasform
	, float* cameraTransform
	, std::vector<ChunkDD>& chunks
	, std::vector<ChunkDD>& chunksWall);
void CalculateLightMap(int chunkNumber
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap);
void CalculateLightMap(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap);
void CreateLightMap(std::vector<std::vector<float>>& StaticLightMap
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform
	, int x
	, int y
	, int width
	, int height
	, unsigned int& iD);