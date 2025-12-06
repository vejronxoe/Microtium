#pragma once
#include "Opengl/Shader.h"
enum treeParts
{
	p_Log = 0,
	p_SmallCrown,
	p_Crown,
};
class tree
{
public:
	int m_Transform[2];
	char m_PartOfTree;
	unsigned short int m_ItemDrop;
	unsigned char m_Hardness;
	unsigned int m_texture;
	unsigned int m_DrawData;
	float m_Rotation;
	tree(unsigned int texture
		, unsigned int drawData
		, unsigned short int itemDrop
		, unsigned char hardness
		, char partOfTree
		, int x
		, int y
		, float rotation);
	void drawTree(Shader& sh
		,unsigned int transformLocation
		, unsigned int rotateLocation
		, float* cameraCoordinate
		, float* transform
		, float* rotation);
};
class seedling
{

};
class fragileFlowers
{

};
class damagedWood
{
public:
	int m_Transform[2];
	int m_Rotation;
	char m_HP;
	damagedWood(int x
		, int y
		, int rotation
		, char HP);
	void DrawCut(Shader& Sh
		, unsigned int transformLocation
		, unsigned int rotationLocation
		, float* rotation
		, float* transform
		, unsigned int* texture);
};