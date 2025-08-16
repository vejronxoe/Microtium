#pragma once
#include<vector>


class Block
{
private:
	unsigned int m_te;
public:
	int m_Transform[2];
	bool m_CollisionActive;
	Block(unsigned int tex, int x, int y, bool hitboxActive);
	void DrawBlock(unsigned int blocksDrawData);
};
void LoadMap(const char* filepath, std::vector<Block>& blocks, unsigned int* texturesIDs);
void CreateAllBlockTextures(unsigned int* IDs);
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob);
