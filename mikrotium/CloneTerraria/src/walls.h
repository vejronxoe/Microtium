#pragma once
#include"Opengl/Shader.h"
#include"Blocks.h"

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
void createWall(int x
	, int y
	, bool render
	, unsigned short int IDOfItemWall
	, std::vector<std::vector<wall>>& walls
	, unsigned int* texturesIDs);
void LoadMapWall(const char* filepath
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, unsigned int* texturesIDs);
void drawWalls(std::vector<DamagedBlock> damagedWalls
	, unsigned int* damageTextures
	, std::vector<std::vector<wall>>& walls
	, Shader& wallsSh
	, unsigned int shadowLocation
	, unsigned int wallsCameraLocation
	, float* camera
	, unsigned int wallsTransformLocation
	, float* transform
	, float* cameraCoordinate);
int FindWall(std::vector<std::vector<wall>>& walls
	, int x
	, int y);
bool WallInArea(std::vector<std::vector<wall>>& walls
	, int* vertices);