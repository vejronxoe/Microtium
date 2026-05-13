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
#include"glfw/UI.h"
#include "glfw/Font.h"
#define PLAYERHANDOFFSETX -0.5f
#define PLAYERHANDOFFSETY 0.65f

enum RangeWeaponTypes
{
	weaponNot = 0
	, weaponMelee
	, weaponBow
	, weaponCanon
	, weaponGun
	, weaponAutomatic

};
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
enum HUDParts
{
	HUDHotBar = 0
	, HUDArmors
	, HUDInventory
	, HUDChest
	, HUDSlot
	, HUDDefault
	
};

struct Ingredient
{
	Ingredient(short int item, short int amount);
	short int m_Item;
	short int m_Amount;
	char m_CraftingState;
	int m_NumberOfPossibleCraft;
};

struct Recipe
{
	void CreateRecipe(short itemOutput, short amountOutput, char craftingStation);
	char m_CraftingState;
	std::vector<Ingredient> m_Ingredients;
	short m_CraftingStation;
	short m_ItemOutput;
	short m_AmountOutput;
	int m_MaximumCraft;
};



class Player
{
private:
	Recipe m_Recipes[5];
	int m_NumberOfVisibleRecipes;
	Recipe m_VisibleRecipes[5];
	int m_UsingIndexRecipe = 0;
	float m_RecipeY = 0;

	
private:
	bool m_FloorHit = false;
	bool m_CeilHit = false;
	bool m_LeftWallHit = false;
	bool m_RightWallHit = false;
	float m_CoyoteTimer = false;
	float m_JumpTimer = false;
	bool m_CanJump = false;
public:
	int m_AimingAtChest = -1;
	int m_IndexOfOpenChest = -1;

	int m_AimingAtDoors = -1;

	short int m_DirectionLook = -1;
	unsigned int m_FloorBehaviour = b_BasicSolid;
	int m_JumpPower = 12;

	int m_Acceleration = 0;
	int m_Friction = 0;
	float m_MaxMovementSpeed = 0;
	float m_Transform[2] = {10,30};
	float m_Velocity[2] = {0,0};

	Player(unsigned int eob
		, std::vector<Chest>& chests
		, std::vector<Letter>& Ascii
		, unsigned int* texturesIDs
		, unsigned int* structuretexs);
	void slotsSwap(float deltaTime
		, unsigned short* amount
		, unsigned short* items
		, int begin);
		void ResizeHUD(unsigned int eob
			, std::vector<Chest>& chests
			, std::vector<Letter>& Ascii);
	bool ItermGetToInventory(unsigned short int& amount
		, unsigned short int item);
	void DamagePlayer(float* transfromAttacker
		, int Damage);
	void EveryFrame(float deltaTime
		, std::vector<int>& chunksToRebuildBlocks
		, std::vector<std::vector<Block>>& blocks
		, std::vector<int>& chunksToRebuildWalls
		, std::vector<std::vector<Wall>>& Walls
		, std::vector<Enemy>& enemies
		, std::vector<int>& isThereSandOnX
		, std::vector<CraftStation>& craftStations
		, std::vector<damagedWood>& damagedWoods
		, std::vector<DamagedBlock>& damageblocks
		, std::vector<DamagedBlock>& damagedWalls
		, std::vector<Letter>& Ascii
		, float* CameraCoordinates
		, unsigned int blockDD
		, unsigned int eob
		, unsigned int* texturesIDs
		, unsigned int* structuresTextures
		, std::vector<tree>& trees
		, std::vector<seedling>& seedlings
		, std::vector<DroppedItem>& droppedItems
		, std::vector<Projectile>& projectiles
		, std::vector<Door>& doors
		, std::vector<Chest>& chests);

	void DrawPlayer(float deltaTime
		, Shader& basicSh
		, Shader& HUDSh
		, Shader& numberSh
		, Shader& fontSh
		, Shader& animSh
		, Shader& handSh
		, Shader& particlesSh
		, std::vector<Chest>& chests
		, float* transform
		, float* scale
		, float* rotation
		, float* camera
		, unsigned int fontTex
		, unsigned int fontDD
		, unsigned int particlesDD
		, unsigned int numberTexture);

	bool HavePlayerSpace(unsigned short int item);

	void SwapItemStats();
private:
	float m_ArmTimer = 0;
	char m_ArmPhase = 0;
	char m_ArmsBehaviour = 0;
	float m_WalkingTimer = 0;
	char m_WalkingPhase = 0;
	float m_ArmRotation = 0;
	
	unsigned int m_BulletsDD = 0;
	unsigned int m_BottomAnimDD = 0;
	unsigned int m_HandDD = 0;
	unsigned int m_BodyAnimDD = 0;
	unsigned int m_HeadDD = 0;
	unsigned int m_BootsAnimTex = 0;
	unsigned int m_ItemInHandDD = 0;
	unsigned int m_BlockInHandDD = 0;
	unsigned int m_HandTex = 0;
	unsigned int m_LegAnimTex = 0;
	unsigned int m_BodyAnimTex = 0;
	unsigned int m_HeadTex = 0;


	
	float m_SlotGap = 0;
	float m_TimerSpliting = 0;
	float m_AddNextFrameDrop = 0;
	float m_ItemsToTake = 0;
	float m_NumberOfRecipesDone;
	void ChangeAmountText(Text& text
		, std::vector<Letter>& ascii
		, unsigned int eob
		, int oldAmount
		, int amount
		, float x
		, float y);

	void createHUD(unsigned int eob
		, std::vector<Chest>& chests
		, std::vector<Letter>& ASCII);
	void SwapArmor(unsigned char SlotIndex, char armorPart);
	bool IsItStackble(unsigned short int item);
	char FindItemInInv(unsigned char item);
	char FindOneOfItemsInInv(unsigned char* items, int sizeOfArray);
public:
	unsigned int m_HUDDD[6] = {};
	unsigned int m_HUDVBO[6] = {};
	unsigned int m_HUDEOB[4] = {};
	unsigned int m_HUDEOBSize[4] = {};
	unsigned int m_SlotTextures = 0;
	float m_LastStandingY = 0;
	float m_HPOffset[2] = {};
	unsigned int m_HPTexture[5] = {0};
	float m_TimerSinceLastHit = 0;
	float m_AddNextFrameHP = 0;
	int m_HPRegen = 2;
	int m_CurrentHealth = 50;
	int m_maxHealth = 100;

	
	float m_InvOffset[2] = {};
	float m_HalfOfSlotLeanght = 0;
	unsigned char m_HUDUseSlot = 1;



	
	
	
	unsigned char m_AimingAtSlot = 0;
	unsigned char m_UseSlot = 0;
	bool m_IsInventoryOpen = false;


	unsigned int m_AllItemTextures[48] = {};//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned int m_ItemsInHandDD[3] = {};//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned int m_ItemsInHandTexture[3] = {};//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned short int m_PlayerSlots[60] = {};
	unsigned short int m_AmountInSlots[52] = {};
	Text m_AmountText[51];
	bool newChest = true;
	Text m_ChestAmountText[50];

	float m_UseItemTimer = 0;
	bool m_CursorOnMinableBlock = false;
	bool m_CursorOnMinableWall = false;
	bool m_CursorOnMinableWood = false;
	bool m_CursorOnPlaceableForStructure = false;
	bool m_CursorOnPlaceableSpot = false;
	char m_LocationAmmunition = -1;
	float m_CooldownToUse = 6;
	std::vector<unsigned int> m_HitEnemies;
	unsigned char m_WeaponType = weaponNot;
	unsigned char m_PickaxeStreanght = 0;
	unsigned char m_AxeStreanght = 0;
	unsigned char m_HammerStreanght = 0;
	unsigned char m_Range = 0;
	short int m_Damage = 0;
	short int m_ArmorClass = 0;
	unsigned int m_ArmorClassTex = -1;
	bool m_Placeable = false;
	bool m_LargePlaceable = false;
	bool m_Consume = false;

	void SwapAccessorise(unsigned char invSlotIndex
		, unsigned char accessoriseSlotIndex);
	bool m_Effects[3] =  {false};
	float m_OnFireTimer = 0;
	float m_DamageTimer = 0;
	bool m_CanDoubleJump = false;
	int m_SpeedMultiplier = 1;
	bool m_Accessorise = false;
	bool m_IsBurning = false;
	float m_BurningTimer = 0;
	int m_BurnDamageNextTime = 0;
	FireParticle m_OnFire;
};
void CreateAllItemTextures(unsigned int* itemTex
	, unsigned int* blockTex
	, unsigned int* structureTex);
enum SlotTextures
{
	defaultSlot = 0
	, useSlot
	, missingSlot
	, nothingSlot
	, chestSlot
	, helmetSlot
	, chestPlateSlot
	, pantsSlot
	, shoesSlot
	, trashSlot

};

void DrawItem(Shader sh
	, int sizeOfVertex
	, float* transform
	, float x
	, float y
	, unsigned int item
	, unsigned int* allItemTextures);