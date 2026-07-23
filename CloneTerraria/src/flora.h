#pragma once
#include "Opengl/Shader.h"

#include "BlocksAndWalls.h"


enum CrownTypes : uint8_t
{
	crown_ForestSmall = 0
	, crown_SnowSmall
	, crown_Forest
	, crown_Snow
};
struct Crown
{
	int m_Transform[2] = {0,0};
	int m_Rotation = 0;
	CrownTypes m_Type = crown_ForestSmall;
};
void DrawTrees(Shader& sh
	, float* cameraCoordinate
	, float* transform
	, float* rotation);
class seedling
{
public:
	int m_Transform[2];
	float m_Timer;
	char m_Type;
	unsigned int m_Texture;
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