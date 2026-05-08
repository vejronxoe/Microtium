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
	
	t_Dirt = 0
	, t_Ice
	, t_Asphalt
	, t_Platform
	, t_ForestPlank
	, t_Sand
	, t_TopGrass
	, t_LeftGrass
	, t_DownGrass
	, t_RightGrass
	, t_TopDownGrass
	, t_LeftRightGrass
	, t_TopLeftGrass
	, t_DownLeftGrass
	, t_DownRightGrass
	, t_TopRightGrass
	, t_MissingTopGrass
	, t_MissingLeftGrass
	, t_MissingDownGrass
	, t_MissingRightGrass
	, t_FullGrass
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
	unsigned char m_BlockType = t_Dirt;
	int m_Y = 0;
	unsigned char m_BlockBehavior = b_BasicSolid;
	unsigned char m_Hardness = 15;

	Block(unsigned char blockType 
		, int y);

	void DrawBlock( Shader& basicShader
		, int x
		, float* transform);
};
class wall
{
public:
	unsigned char m_WallType;
	bool m_Render;
	int m_Y;
	unsigned char m_Hardness;
	wall(unsigned char wallType
		, bool render
		, int y);

	void drawWalls(Shader& wallSh
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




void drawWalls(std::vector<DamagedBlock> damagedWalls
	, unsigned int* damageTextures
	, std::vector<std::vector<wall>>& walls
	, Shader& wallsSh
	, float* camera
	, float* transform
	, float* cameraCoordinate);
void drawBlocks(std::vector<std::vector<Block>>& blocks
	, std::vector<DamagedBlock> damagedBlocks
	, float* cameraCoordinate
	, Shader& basicSh
	, unsigned int* damageTexture
	, float* transform
	, float* camera);

void createWall(int x
	, int y
	, unsigned short int IDOfItemWall
	, std::vector<std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks
	, unsigned int* texturesIDs);
void CreateBlock(int x
	, int y
	, unsigned short int IDOfItemBlock
	, std::vector<std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX
	, unsigned int* texturesIDs);
void DestroyBlock(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<int>& isThereSandOnX
	, int x
	, int y);

int FindBlock(std::vector<std::vector<Block>>& blocks
	, int x
	, int y
	, bool& found);
bool blockInArea(std::vector<std::vector<Block>>& blocks
	, int* vertices);
int FindWall(std::vector<std::vector<wall>>& walls
	, int x
	, int y
	, bool& found);
bool WallInArea(std::vector<std::vector<wall>>& walls
	, int* vertices);
