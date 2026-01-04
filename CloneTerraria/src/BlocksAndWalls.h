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
	t_TopGrass = 0    
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
	, t_Dirt
	, t_Ice
	, t_Asphalt
	, t_Platform
	, t_ForestPlank
	, t_Sand
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

};

void CreateAllBlockTextures(unsigned int* IDs);

class Block
{
public:
	unsigned int m_te;
	int m_Y;
	unsigned char m_BlockBehavior;
	unsigned char m_Hardness;
	unsigned short m_ItemDrop;

	Block(unsigned int tex
		, int y
		, unsigned char Behavior
		, unsigned char hardness
		, unsigned short int itemDrop);

	void DrawBlock( Shader& basicShader
		, int x
		, unsigned int location 
		, float* transform);
};
class wall
{
	unsigned int m_Texture;
public:
	bool m_Render;
	unsigned short int m_ItemDrop;
	int m_Y;
	unsigned char m_Hardness;
	wall(unsigned int texture
		, bool render
		, unsigned short int itemDrop
		, int y
		, unsigned char hardness);

	void drawWalls(Shader& wallSh
		, int x
		, float* transform
		, unsigned int transformLocation);
};
struct DamagedBlock
{
	DamagedBlock(int x
		, int y
		, char HP);
	int m_Transform[2];
	char m_HP;
	void DrawDamage(Shader& basicShader
		, unsigned int location
		, float* transform
		, unsigned int* texture);
};




void LoadMapBlocksAndWalls(const char* filePathWalls
	, const char* filePathBlocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks
	, std::vector<bool>& isThereSandOnX
	, int minX
	, int maxX
	, int minY
	, int maxY
	, unsigned int* texturesIDs);

void drawWalls(std::vector<DamagedBlock>& damagedWalls
	, unsigned int* damageTextures
	, std::vector<std::vector<wall>>& walls
	, Shader& wallsSh
	, unsigned int shadowLocation
	, unsigned int wallsCameraLocation
	, float* camera
	, unsigned int wallsTransformLocation
	, float* transform
	, float* cameraCoordinate);
void drawBlocks(std::vector<std::vector<Block>>& blocks
	, std::vector<DamagedBlock>& damagedBlocks
	, float* cameraCoordinate
	, Shader& basicSh
	, unsigned int* damageTexture
	, unsigned int transformLocation
	, float* transform
	, unsigned int cameraLocation
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
	, std::vector<bool>& isThereSandOnX
	, unsigned int* texturesIDs);
void DestroyBlock(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<bool>& isThereSandOnX
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
