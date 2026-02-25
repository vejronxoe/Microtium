#pragma once

#include<vector>


#include"Opengl/Shader.h"
#include"Collision.h"
#include"DroppedItems.h"
#include"ItemList.h"
#include"BlocksAndWalls.h"
#include"flora.h"
#include"projectile.h"
#include"structures.h"

#define PLAYERHANDOFFSETX -0.5f
#define PLAYERHANDOFFSETY 0.65f

enum Effects
{
	effectWallClimb = 0
	, effectArrowBag
	, effectDoubleJump
};
enum CraftingState
{
	cantCraft = 0
	, missingToCraft
	, ReadyToCraft
};
struct Ingredient
{
	Ingredient(short int item, short int amount);
	short int m_Item;
	short int m_Amount;
	char m_CraftingState;
};

struct Recipe
{
	void CreateRecipe(short itemOutput, short amountOutput, char craftingStation);
	char m_CraftingState;
	std::vector<Ingredient> m_Ingredients;
	short m_CraftingStation;
	short m_ItemOutput;
	short m_AmountOutput;
};



class Player
{
private:
	Recipe m_Recipes[5];
	int m_NumberOfVisibleRecipes;
	Recipe m_VisibleRecipes[5];
	int m_UsingIndexRecipe;
	float m_RecipeY;
	float m_TimerCrafting;
	float m_NumberOfRecipesDone;



private:
	bool m_FloorHit;
	bool m_CeilHit;
	bool m_LeftWallHit;
	bool m_RightWallHit;
	float m_CoyoteTimer;
	float m_JumpTimer;
	bool m_CanJump;
public:
	short int m_DirectionLook;
	unsigned int m_FloorBehaviour;
	int m_JumpPower;
	float m_Gravity;
	int m_Acceleration;
	int m_Friction;
	float m_MaxMovementSpeed;
	float m_Transform[2];
	float m_Velocity[2];
	Player(unsigned int eob
		, unsigned int* texturesIDs);

	bool ItermGetToInventory(unsigned short int& amount
		, unsigned short int item);
	void DamagePlayer(float* transfromAttacker
		, int Damage);
	void EveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<wall>>& walls
		, std::vector<Enemy*>& enemies
		, std::vector<bool>& isThereSandOnX
		, std::vector<CraftStation>& craftStations
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

	void DrawPlayer(float deltaTime
		, Shader& basicSh
		, Shader& HUDSh
		, Shader& fontSh
		, Shader& animSh
		, Shader& handSh
		, Shader& particlesSh
		, float* transform
		, float* scale
		, float* rotation
		, unsigned int fontDD
		, unsigned int particlesDD
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
	
	unsigned int m_BulletsDD;
	unsigned int m_BottomAnimDD;
	unsigned int m_HandDD;
	unsigned int m_BodyAnimDD;
	unsigned int m_HeadDD;
	unsigned int m_BootsAnimTex;
	unsigned int m_ItemInHandDD;
	unsigned int m_BlockInHandDD;
	unsigned int m_HandTex;
	unsigned int m_LegAnimTex;
	unsigned int m_BodyAnimTex;
	unsigned int m_HeadTex;


	unsigned int m_HUDDD;
	unsigned int m_SlotTexture;
	unsigned int m_TrashCanSlotTexture;
	float m_SlotGap;
	float m_TimerSplitingItem;
	float m_AddNextFrameDropItem;
	float m_ItemsToTake;
	void SwapArmor(unsigned char SlotIndex, char armorPart);
	bool IsItStackble(unsigned short int item);
	char FindItemInInv(unsigned char item);
	char FindOneOfItemsInInv(unsigned char* items, int sizeOfArray);
public:

	float m_LastStandingY;
	float m_HPOffset[2];
	unsigned int m_HPTexture[5];
	float m_TimerSinceLastHit;
	float m_AddNextFrameHP;
	int m_HPRegen;
	int m_CurrentHealth;
	int m_maxHealth;


	float m_InvOffset[2];
	float m_HalfOfSlotLeanght;
	unsigned char m_HUDUseSlot;
	unsigned int m_UseSlotTexture;
	unsigned int m_MissingSlotTexture;
	unsigned int m_NothingSlotTexture;
	unsigned char m_AimingAtSlot;
	unsigned char m_UseSlot;
	bool m_IsInventoryOpen;


	unsigned int m_AllItemTextures[44];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned int m_ItemsInHandDD[3];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned int m_ItemsInHandTexture[3];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned short int m_PlayerSlots[60];
	unsigned short int m_AmountInSlots[52];


	float m_UseItemTimer;
	bool m_CursorOnMinableBlock;
	bool m_CursorOnMinableWall;
	bool m_CursorOnMinableWood;
	bool m_CursorOnPlaceableForStructure;
	bool m_CursorOnPlaceableSpot;
	char m_LocationAmmunition;
	float m_CooldownToUse;
	std::vector<unsigned int> m_HitEnemies;
	unsigned char m_WeaponType;
	unsigned char m_PickaxeStreanght;
	unsigned char m_AxeStreanght;
	unsigned char m_HammerStreanght;
	unsigned char m_Range;
	short int m_Damage;
	short int m_ArmorClass;
	bool m_Placeable;
	bool m_LargePlaceable;
	bool m_Consume;

	void SwapAccessorise(unsigned char invSlotIndex
		, unsigned char accessoriseSlotIndex);
	bool m_Effects[3];
	float m_OnFireTimer;
	bool m_CanDoubleJump;
	int m_SpeedMultiplier;
	bool m_Accessorise;
	bool m_IsBurning;
	float m_BurningTimer;
	int m_BurnDamageNextTime;
	FireParticle m_OnFire;
};

