#pragma once
#include<vector>

#include"Opengl/Shader.h"

class Block
{
private:
	unsigned int m_te;
public:
	int m_Transform[2];
	bool m_CollisionActive;
	Block(unsigned int tex, int x, int y, bool hitboxActive);
	void DrawBlock(unsigned int blocksDrawData, Shader& basicShader, unsigned int location ,float* transform);
};
void LoadMap(const char* filepath, std::vector<Block>& blocks, unsigned int* texturesIDs);
void CreateAllBlockTextures(unsigned int* IDs);
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob);
