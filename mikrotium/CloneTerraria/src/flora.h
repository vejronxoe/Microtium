#pragma once
#include "Opengl/Shader.h"
enum treeParts
{
	t_Log = 0,
	t_SmallCrown,
	t_Crown,
};
class tree
{
public:
	tree* m_ObjectToDestroy[3];
	int m_Transform[2];
	char m_PartOfTree;
	unsigned short int m_DropItem;
	unsigned int m_texture;
	unsigned int m_DrawData;
	tree(tree* destroyObject1
		, tree* destroyObject2
		, tree* destroyObject3
		, unsigned int texture
		, unsigned int drawData
		, unsigned short int itemDrop
		, char partOfTree
		, int x
		, int y);
	void drawTree(Shader& sh,unsigned int transformLocation, float* transform);
};
class seedling
{

};
class fragileFlowers
{

};