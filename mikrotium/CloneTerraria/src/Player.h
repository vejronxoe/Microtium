#pragma once

#include<vector>

#include"Blocks.h"
#include"Opengl/Shader.h"
#include"Collision.h"
#include"DroppedItems.h"
#include"ItemList.h"
#include"walls.h"


class Player
{
private:
	unsigned int m_PlayerDrawData;
	unsigned int m_Tex;
	bool m_FloorHit;
	bool m_CeilHit;
	bool m_WallHit;
	float m_CoyoteTimer;
	float m_JumpTimer;
	bool m_CanJump;
public:
	short int m_DirectionLook;

	int m_JumpPower;
	float m_Gravity;
	int m_Acceleration;
	int m_Friction;
	float m_MaxMovementSpeed;
	float m_Transform[2];
	float m_Velocity[2];
	Player(unsigned int eob
		, unsigned int HUDTransformLocatin
		, unsigned int HUDScaleLocatin
		, float& yLocationOfFirstSlot
		, float& xLocationOfFirstSlot
		, unsigned int* texturesIDs);

	bool ItermGetToInventory(unsigned short int& amount
		, unsigned short int item);

	void EveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<wall>>& walls
		, std::vector<DamagedBlock>& damageblocks
		, std::vector<DamagedBlock>& damagedWalls
		, float* CameraCoordinates
		, unsigned int* texturesIDs
		, std::vector<DroppedItem>& droppedItems);

	void DrawPlayer(Shader& basicSh
		, Shader& HUDSh
		, Shader& fontSh
		, unsigned int shadowLocation
		, unsigned int transformLocation
		, float* transform
		, float* scale
		, int fontDrawData
		, int numberLocation
		, int fontTransformLocation
		, int fontscaleLocation
		, int numberTexture);

	bool HavePlayerSpace(unsigned short int item);

	void SwapItemStats();

private:
	unsigned int m_InventoryDrawData;
	unsigned int m_SlotTexture;
	unsigned int m_TrashCanSlotTexture;
	float m_SlotGap;
	unsigned int m_HUDTransformLocation;
	unsigned int m_HUDScaleLocation;
	float m_TimerSplitingItem;
	float m_AddNextFrame;
	bool IsItStackble(unsigned short int item);

	void CreateAllItemsTexture(unsigned int* texturesIDs);

public:
	float m_SlotVertices[4];
	unsigned char m_HUDUseSlot;
	unsigned int m_UseSlotTexture;
	unsigned char m_AimingAtSlot;
	unsigned char m_UseSlot;
	bool m_IsInventoryOpen;
	unsigned int m_AllItemTextures[12];
	unsigned short int m_PlayerSlots[52];
	unsigned short int m_AmountInSlots[52];

public:
	float m_UseItemTimer;
	bool m_CursorOnMinableBlock;
	bool m_CursorOnMinableWall;
	bool m_CursorOnPlaceableSpot;
	float m_CooldownToUse;
	unsigned char m_PickaxeStreanght;
	unsigned char m_AxeSteanght;
	unsigned char m_HammerStreanght;
	unsigned char m_Range;
	short int m_Damage;
	bool m_Placeable;
};
