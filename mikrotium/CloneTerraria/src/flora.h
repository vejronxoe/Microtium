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
	unsigned short int m_DropItem;
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