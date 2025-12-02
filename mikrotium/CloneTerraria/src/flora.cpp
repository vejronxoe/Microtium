#include"flora.h"

tree::tree(tree* destroyObject1
	, tree* destroyObject2
	, tree* destroyObject3
	, unsigned int texture
	, unsigned int drawData
	, unsigned short int itemDrop
	, char partOfTree
	, int x
	, int y)
	:m_ObjectToDestroy{destroyObject1, destroyObject2, destroyObject3}
	, m_Transform{x, y}
	, m_PartOfTree(partOfTree)
	, m_texture(texture)
	, m_DrawData(drawData)
	, m_DropItem(itemDrop)
{}