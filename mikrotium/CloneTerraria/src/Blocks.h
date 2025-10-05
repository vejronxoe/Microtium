#pragma once
#include<vector>

#include"Opengl/Shader.h"

extern int xMaxBlock;
extern int xMinBlock;
extern int yMaxBlock;
extern int yMinBlock;

enum BlockBehavior
{
	basicSolid = 0,
	air,
	slippery,
	asphalt,
	platform,
	hitboxOff
};

class Block
{
private:
	unsigned int m_te;
public:
	int m_Transform[2];
	unsigned char m_BlockBehavior;
	Block(unsigned int tex, int x, int y, unsigned int Behavior);
	void DrawBlock( Shader& basicShader, unsigned int location ,float* transform);
};
void LoadMap(const char* filepath, std::vector<std::vector<Block>>& blocks, int minX, int maxX, int minY, int maxY, unsigned int* texturesIDs);
void CreateAllBlockTextures(unsigned int* IDs);
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob);
