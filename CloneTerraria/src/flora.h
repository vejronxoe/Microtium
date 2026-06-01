#pragma once
#include "Opengl/Shader.h"

#include "BlocksAndWalls.h"


enum treeParts
{
	part_Log = 0,
	part_SmallCrown,
	part_Crown,
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
	int m_Rotation;
	tree(unsigned int texture
		, unsigned int drawData
		, unsigned short int itemDrop
		, unsigned char hardness
		, char partOfTree
		, int x
		, int y
		, int rotation);
	void drawTree(Shader& sh
		, float* cameraCoordinate
		, float* transform
		, float* rotation);
};
class seedling
{
public:
	int m_Transform[2];
	float m_Timer;
	char m_Type;
	unsigned int m_Texture;
	int m_IndexOfGroundBlock;
	seedling(char type
		, int x
		, int y
		, unsigned int* structuresTextures
		, std::vector<std::vector<Block>>& blocks);
	bool everyFrame(float deltaTime
		, unsigned int* treeTextures
		, unsigned int* treeDD
		, std::vector<std::vector<Block>>& blocks
		, std::vector<seedling>& seedlings
		, std::vector<tree>& trees);
	void drawSeedling(Shader sh
		, float* Transform);

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
		, float* rotation
		, float* transform
		, unsigned int* texture);
};
bool FindWood(std::vector<tree>& woods
	, int x
	, int y
	, int& index);

bool FindSeedling(std::vector<seedling>& seedlings
	, int x
	, int y
	, int& index);

bool FindSeedling(std::vector<seedling>& seedlings
	, int* vertices);

bool FindWood(std::vector<tree>& woods
	, int* vertices);