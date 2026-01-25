#pragma once

#include<vector>


#include"Opengl/Shader.h"
#include"Collision.h"
#include"DroppedItems.h"
#include"ItemList.h"
#include"BlocksAndWalls.h"
#include"flora.h"
#include"projectile.h"

class Player
{

private:
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
		, float& yLocationOfFirstSlot
		, float& xLocationOfFirstSlot
		, unsigned int* texturesIDs);

	bool ItermGetToInventory(unsigned short int& amount
		, unsigned short int item);

	void EveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<wall>>& walls
		, std::vector<bool>& isThereSandOnX
		, std::vector<damagedWood>& damagedWood
		, std::vector<DamagedBlock>& damageblocks
		, std::vector<DamagedBlock>& damagedWalls
		, float* CameraCoordinates
		, unsigned int blockDD
		, unsigned int* texturesIDs
		, unsigned int* structuresTextures
		, std::vector<tree>& trees
		, std::vector<seedling>& seedlings
		, std::vector<DroppedItem>& droppedItems
		, std::vector<Projectile>& projectiles);

	void DrawPlayer(Shader& basicSh
		, Shader& HUDSh
		, Shader& fontSh
		, Shader& animSh
		, Shader& handSh
		, float* transform
		, float* scale
		, float* rotation
		, unsigned int fontDD
		, unsigned int numberTexture);

	bool HavePlayerSpace(unsigned short int item);

	void SwapItemStats();
private:
	float m_ArmTimer;
	char m_ArmPhase;
	char m_ArmsBehaviour;
	float m_WalkingTimer;
	char m_WalkingPhase;
	float m_ArmRotation;
	
	unsigned int m_BottomAnimDD;
	unsigned int m_HandDD;
	unsigned int m_BodyAnimDD;
	unsigned int m_HeadDD;
	unsigned int m_BootsAnimTex;
	unsigned int m_ItemInHandDD;
	unsigned int m_HandTex;
	unsigned int m_LegAnimTex;
	unsigned int m_BodyAnimTex;
	unsigned int m_HeadTex;
private:
	unsigned int m_InventoryDrawData;
	unsigned int m_SlotTexture;
	unsigned int m_TrashCanSlotTexture;
	float m_SlotGap;
	float m_TimerSplitingItem;
	float m_AddNextFrame;
	bool IsItStackble(unsigned short int item);
	char FindItemInInv(unsigned char item);
	char FindOneOfItemsInInv(unsigned char* items,int sizeOfArray);
public:
	
	float m_SlotVertices[4];
	unsigned char m_HUDUseSlot;
	unsigned int m_UseSlotTexture;
	unsigned char m_AimingAtSlot;
	unsigned char m_UseSlot;
	bool m_IsInventoryOpen;
	unsigned int m_AllItemTextures[18];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned short int m_PlayerSlots[52];
	unsigned short int m_AmountInSlots[52];

public:
	float m_UseItemTimer;
	bool m_CursorOnMinableBlock;
	bool m_CursorOnMinableWall;
	bool m_CursorOnMinableWood;
	bool m_CursorOnPlaceableForStructure;
	bool m_CursorOnPlaceableSpot;
	char m_LocationAmmunition;
	float m_CooldownToUse;
	unsigned char m_WeaponType;
	unsigned char m_PickaxeStreanght;
	unsigned char m_AxeStreanght;
	unsigned char m_HammerStreanght;
	unsigned char m_Range;
	short int m_Damage;
	bool m_Placeable;
	bool m_LargePlaceable;
};
