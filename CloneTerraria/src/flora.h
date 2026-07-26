#pragma once
#include "Opengl/Shader.h"

#include "BlocksAndWalls.h"

#include "DroppedItems.h"

enum CrownTypes : uint8_t
{
	crown_ForestSmall = 0
	, crown_SnowSmall
	, crown_Forest
	, crown_Snow
};
struct Crown
{
public:
	int16_t m_Transform[2] = {0,0};
	int8_t m_Rotation = 0;
	uint8_t m_Type = crown_ForestSmall;
	Crown(int16_t x
		, int16_t y
		, int8_t rotation
		, uint8_t type);
};
void DestroyCrown(int* transform
	, std::vector<DroppedItem>& dropItems
	, std::vector<Crown>& crowns);
void DrawCrowns(Shader& sh
	, std::vector<Crown>& crowns
	, unsigned int* DDs
	, unsigned int* textures
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
		, std::vector<int>& chunksToRebuild
		, std::vector<std::vector<Block>>& blocks
		, std::vector<seedling>& seedlings
		, std::vector<Crown>& Crowns);
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

bool FindSeedling(std::vector<seedling>& seedlings
	, int x
	, int y
	, int& index);

bool FindSeedling(std::vector<seedling>& seedlings
	, int* vertices);

