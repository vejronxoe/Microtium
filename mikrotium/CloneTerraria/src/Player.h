#pragma once

#include<vector>

#include"Blocks.h"
#include"Opengl/Shader.h"
#include"Collision.h"

enum Items 
{
	i_Nothing = 0,
	i_CooperPickaxe,
	i_CooperAxe,
	i_CooperHammer,
	i_CooperSword,
	i_Dirt,
	i_Ice,
	i_Asphalt,
	i_Platform,
};



class Player
{
private:
	void MovementEveryFrame(float deltaTime, std::vector<std::vector<Block>>& blocks);
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
	Player(unsigned int eob, unsigned int HUDTransformLocatin, unsigned int HUDScaleLocatin, float& yLocationOfFirstSlot, float& xLocationOfFirstSlot,unsigned int* texturesIDs);
	void EveryFrame(float deltaTime, std::vector< std::vector<Block>>& blocks, std::vector<DamagedBlock>& damageblocks, float* CameraCoordinates, unsigned int* texturesIDs);
	void DrawPlayer(Shader& basicSh, Shader& HUDSh, Shader& fontSh, unsigned int transformLocation, float* transform, int fontDrawData, int numberLocation, int fontTransformLocation, int fontscaleLocation, int numberTexture);
	void IventoryEveryFrame();
private:
	bool IsItStackble(unsigned short int item);
	void ItermGetToInventory(unsigned short int amount, unsigned short int item);
	void SwapItemStats();
	void CreateAllItemsTexture(unsigned int* texturesIDs);
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
	unsigned int m_AllItemTextures[9];
	unsigned short int m_PlayerSlots[52];
	unsigned short int m_AmountInSlots[52];

public:
	float m_UseItemTimer;
	bool m_CursorOnMinableBlock;
	bool m_CursorOnPlaceableSpot;
	float m_CooldownToUse;
	unsigned char m_PickaxeStreanght;
	unsigned char m_AxeSteanght;
	unsigned char m_HammerStreanght;
	unsigned char m_Range;
	short int m_Damage;
	bool m_Placeable;
};

