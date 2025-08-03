#pragma once
#include<vector>

#include"Collision.h"

class Block
{
private:
	unsigned int m_te;
	unsigned int m_VB;
	unsigned int m_vao;

public:
	float m_vertices[20];
	float m_transform[2];
	Block(unsigned int tex, float x, float y, unsigned int eob);
	void DeleteBlock();
	void DrawBlock();
};

void LoadMap(const char* Path, std::vector<StaticSquereHitbox>& hitbox, std::vector<Block>& blocks, unsigned int EOB, unsigned int grass);