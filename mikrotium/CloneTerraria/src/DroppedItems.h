#pragma once
#include"Blocks.h"

class DroppedItem
{
public:
	DroppedItem(float x, float y , int looksOnWhatCoordinates,unsigned short int item, unsigned short int amount,  bool canPlayerPickUpIt);
	float m_Transform[2]; 
	float m_Velocity[2];
	unsigned short int m_Item;
	unsigned short int m_Amount;
	float m_Timer;
	int m_BlockY;
	int m_BlockIndex;
	bool m_GoToPlayer;
	bool EveryFrame(float deltaTime, std::vector<std::vector<Block>>& blocks,  float* playerTransfor, bool doesPlayerHaveSpace);
	void DrawItem(unsigned int* textureIDs, unsigned int itemDD, Shader& basicSh, unsigned int transformLocation, float* transform);
};

void ItemHitBox(DroppedItem& dropItem, std::vector<std::vector<Block>>& hitbox, float* itemVertices, float deltaTime);