#pragma once
#include<vector>

#include"Opengl/Shader.h"

enum BlockBehavior
{
	basicSolid = 0,
	air,
	slippery,
	asphalt,
	platform,
};

class Block
{
private:
	unsigned int m_te;
public:
	int m_Transform[2];
	bool m_CollisionActive;
	unsigned char m_BlockBehavior;
	Block(unsigned int tex, int x, int y, bool hitboxActive, unsigned int Behavior);
	void DrawBlock( Shader& basicShader, unsigned int location ,float* transform);
};
void LoadMap(const char* filepath, std::vector<Block>& blocks, unsigned int* texturesIDs);
void CreateAllBlockTextures(unsigned int* IDs);
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob);
