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
enum TexturesOfBlocks
{
	
	 t_Dirt =0 
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

class Block
{
public:
	unsigned char m_Type = t_Dirt;
	int m_Y = 0;
	unsigned char m_Behavior = b_BasicSolid;
	unsigned char m_Hardness = 15;

	Block(unsigned char blockType 
		, int y);

	void Draw( Shader& basicSh
		, unsigned int* blockTex
		, int x
		, float* transform);
};
class Wall
{
public:
	unsigned char m_Type;
	int m_Y;
	unsigned char m_Hardness;
	Wall(unsigned char WallType
		, int y);

	void Draw(Shader& shadowSh
		, unsigned int* blockTex
		, int x
		, float* transform);
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
	, std::vector<std::vector<Wall>>& walls);
void DestroyWall(std::vector<std::vector<Wall>>& Walls
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
	, int x
	, int y
	, int& index);
bool FindBlock(std::vector<std::vector<Block>>& blocks
	, int* vertices);
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int x
	, int y
	, int& index);
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int* vertices);
int FindChunk(int x, int y);
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks);
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls);
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks);
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls);
void DrawChunks(Shader& basicSh
	, unsigned int* textures
	, float* trasform
	, float* cameraTransform
	, std::vector<ChunkDD>& chunks
	, std::vector<ChunkDD>& chunksWall);