#pragma once
#include<vector>

#include"Opengl/Shader.h"
namespace Blocks
{
	extern int xMax;
	extern int xMin;
	extern int yMax;
	extern int yMin;
}
enum TexturesOfBlocks
{
	t_TopGrass = 0,
	t_LeftGrass,
	t_DownGrass,
	t_RightGrass,
	t_TopDownGrass,
	t_LeftRightGrass,
	t_TopLeftGrass,
	t_DownLeftGrass,
	t_DownRightGrass,
	t_TopRightGrass,
	t_MissingTopGrass,
	t_MissingLeftGrass,
	t_MissingDownGrass,
	t_MissingRightGrass,
	t_FullGrass,
	t_Dirt,
	t_Ice,
	t_Asphalt,
	t_Platform,

};
enum BlockBehavior
{
	b_BasicSolid = 0,
	b_Air,
	b_Slippery,
	b_Asphalt,
	b_Platform
};


class Block
{
private:
	unsigned int m_te;
public:
	int m_Transform[2];
	unsigned char m_BlockBehavior;
	unsigned char m_Hardness;
	Block(unsigned int tex, int x, int y, unsigned char Behavior, unsigned char hardness);

	void DrawBlock( Shader& basicShader, unsigned int location ,float* transform);
};
struct DamagedBlock
{
	DamagedBlock(int x, int y);
	int m_Transform[2];
	void DrawDamage(Shader& basicShader, unsigned int location, float* transform, unsigned int texture);
};
void LoadMap(const char* filepath, std::vector<std::vector<Block>>& blocks, int minX, int maxX, int minY, int maxY, unsigned int* texturesIDs);
void CreateAllBlockTextures(unsigned int* IDs);
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob);
