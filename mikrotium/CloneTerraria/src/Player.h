#pragma once

#include<vector>

#include"Blocks.h"
#include"Opengl/Shader.h"
#include"Collision.h"

enum Items 
{
	Nothing = 0,
	CooperPickaxe,
	CooperAxe,
	CooperHammer,
	CooperSword,

};
enum ToolStreanght
{
	Cooper = 0,
	Iron
};
enum ItemType
{
	Tools = 0,
	Blocks,
	Coins
};

class Player
{
private:
	void MovementEveryFrame(float deltaTime, std::vector<Block>& blocks);
	unsigned int m_PlayerDrawData;
	unsigned int m_Tex;
	bool m_FloorHit;
	bool m_CeilHit;
	bool m_WallHit;
	float m_CoyoteTimer;
	float m_JumpTimer;
	bool m_CanJump;
public:
	

	int m_JumpPower;
	float m_Gravity;
	int m_Acceleration;
	int m_Friction;
	float m_MaxMovementSpeed;
	float m_Transform[2];
	float m_Velocity[2];
	float m_Camera[16];
	float m_Scale[16];
	Player(unsigned int eob, unsigned int HUDTransformLocatin, unsigned int HUDScaleLocatin, float& yLocationOfFirstSlot, float& xLocationOfFirstSlot);
	void EveryFrame(float deltaTime, std::vector<Block>& blocks);
	void DrawPlayer(Shader& basicSh, Shader& HUDSh, unsigned int transformLocation, float* transform);

private:
	bool IsItStackble(unsigned short int item);
	void ItermGetToInventory(unsigned short int amount, unsigned short int item);
	void SwapItemStats();
	void CreateAllItemsTexture();
	unsigned int m_InventoryDrawData;  
	unsigned int m_SlotTexture;
	unsigned int m_UseSlotTexture;
	unsigned int m_TrashCanSlotTexture;
	float m_SlotVertices[4];
	float m_SlotGap;
	unsigned char m_HUDUseSlot;
	unsigned int m_HUDTransformLocation;
	unsigned int m_HUDScaleLocation;
public:
	unsigned char m_AimingAtSlot;
	unsigned char m_UseSlot;
	bool m_IsInventoryOpen;
	unsigned int m_AllItemTextures[5];
	unsigned short int m_PlayerSlots[52];
	unsigned short int m_AmountInSlots[52];
	float m_CooldownToUse;
	unsigned char m_TypeOfItem;
	unsigned char m_PickaxeStreanght;
	unsigned char m_AxeSteanght;
	unsigned char m_HammerStreanght;
	unsigned char m_Range;
	short int m_Damage;
	bool m_Placeable;
};

