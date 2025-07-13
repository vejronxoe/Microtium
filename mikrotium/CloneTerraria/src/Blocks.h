#pragma once
#include<vector>

class Block
{
private:
	unsigned int m_te;
	unsigned int m_VB;
	unsigned int m_vao;
	float m_vertices[20];
	float m_transform[2];
public:
	Block(unsigned int tex, float x, float y, unsigned int eob);
	void DeleteBlock();
	void EveryFrame();
	void DrawBlock();
};
void LoadMap(const char* Path, std::vector<Block>& blocks, unsigned int EOB, unsigned int grass);