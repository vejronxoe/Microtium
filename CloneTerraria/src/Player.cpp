   #include"Player.h"

#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"glfw/Input.h"
#include"glfw/window.h"
#include"math/matrix.h"
#include"math/VectorOperation.h"
#include"NumberRender.h"

#define SPEEDOFANIM 0.5f
#define ARROWSTYPES 4
#define CANNONBALLSTYPES 5
#define BULLETSTYPES 4
#define REGENCOLDDOWN 4
#define CRAFTINGOFFSET 3
#define CHESTANDDOORSREACH 8.0f

enum RangeWeaponTypes
{
	weaponNot = 0
	, weaponMelee
	, weaponBow
	, weaponCanon
	, weaponGun
	, weaponAutomatic
	
};
enum ArmBehaviour
{
	ArmStanding = 0
	,ArmUsing
	,ArmRun
	
};
enum InHandDDTex
{
	InHandBow = 0 
	, InHandCanon
	, InHandPistol
};
enum PartsOfArmor
{
	armorHelmet = 0
	, armorChestPlate
	, armorPants
	, armorShoes
};

Ingredient::Ingredient(short int item, short int amount)
	:m_Item(item)
	, m_Amount(amount)
{}
void Recipe::CreateRecipe(short itemOutput, short amountOutput, char craftingStation)
{
	 m_CraftingStation = craftingStation;
	 m_ItemOutput = itemOutput;
	 m_AmountOutput = amountOutput;
}
unsigned char AmmunicionToProjectileType(unsigned char ammo)
{
	switch (ammo)
	{
	case i_BasicCannonBall:
		return p_BasicCannonBall;
	case i_PierceCannonBall:	
		return p_PierceCannonBall;
	case i_BouncingCannonBall:
		return p_BouncingCannonBall;
	case i_FireCannonBall:
		return p_FireCannonBall;
	case i_Sand:
		return p_Sand;
	case i_BasicArrow:
		return p_BasicArrow;
	case i_PierceArrow:
		return p_PierceArrow;
	case i_BouncingArrow:
		return p_BouncingArrow;
	case i_FireArrow:
		return p_FireArrow;
	case i_BasicBullet:
		return p_BasicBullet;
	case i_PierceBullet:
		return p_PierceBullet;
	case i_BouncingBullet:
		return p_BouncingBullet;
	case i_FireBullet:
		return p_FireBullet;
	default:
		std::cout << "Error player.cpp Dont know this Ammo: " << ammo << std::endl;
		return -1;
	}
}

short int ArmorClass(unsigned char item)
{
	switch (item)
	{
	case i_WoodHelmet:
		return 1;
	case i_WoodChestPlate:
		return 2;
	case i_WoodPants:
		return 1;
	case i_WoodShoes:
		return 1;
	case i_Nothing:
		return 0;
	default:
		std::cout << "Error Player.cpp UnKnow ArmorClass" << std::endl;
		break;
	}
	return 0;
}
char WhatPartOfArmor(unsigned char item)
{
	switch (item)
	{
	case i_WoodHelmet:
		return armorHelmet;
	case i_WoodChestPlate:
		return armorChestPlate;
	case i_WoodPants:
		return armorPants;
	case i_WoodShoes:
		return armorShoes;
	case i_Nothing:
		return -1;
	default:
		return -2;
	}

}

void Player::slotsSwap(float deltaTime
	, unsigned short* amount
	, unsigned short* items
	, int begin)
{
	if (items[m_AimingAtSlot] == i_Nothing && (m_UseSlot == 0 || m_PlayerSlots[0] == i_Nothing))
	{
		m_AimingAtSlot = begin;
	}
	else if (Input::RightMouseHold && m_AimingAtSlot != begin && items[m_AimingAtSlot] && (m_PlayerSlots[0] == items[m_AimingAtSlot] || m_PlayerSlots[0] == i_Nothing || m_UseSlot == 0))
	{
		if (m_TimerSplitingItem == 1 && (m_AmountInSlots[0] != 9999 || m_UseSlot == 0))
		{
			if (m_UseSlot == 0)
			{
				m_AmountInSlots[0] = 0;
			}
			amount[m_AimingAtSlot]--;
			m_AmountInSlots[0]++;
			m_PlayerSlots[0] = items[m_AimingAtSlot];
			if (amount[m_AimingAtSlot] <= 0)
			{
				items[m_AimingAtSlot] == i_Nothing;
			}
			m_UseSlot = m_AimingAtSlot;
			SwapItemStats();
		}
		else if (amount[m_AimingAtSlot] - m_ItemsToTake < 0)
		{
			m_AmountInSlots[0] += amount[m_AimingAtSlot];
			amount[m_AimingAtSlot] = 0;
			m_AddNextFrameDropItem = 0;
			items[m_AimingAtSlot] = i_Nothing;
		}
		else if (m_AmountInSlots[0] + floorf(m_ItemsToTake) >= 9999)
		{
			amount[m_AimingAtSlot] -= 9999 - m_AmountInSlots[0];
			m_AmountInSlots[0] = 9999;
			m_AddNextFrameDropItem = 0;
		}
		else
		{
			m_AmountInSlots[0] += floorf(m_ItemsToTake);
			amount[m_AimingAtSlot] -= floorf(m_ItemsToTake);
			m_AddNextFrameDropItem = m_ItemsToTake - floorf(m_ItemsToTake);
		}
		float oldTimer = m_TimerSplitingItem;
		m_TimerSplitingItem += deltaTime;
		m_ItemsToTake = 2 * m_TimerSplitingItem * deltaTime + 1.0f / 2.0f * (2 * m_TimerSplitingItem - oldTimer) * deltaTime + m_AddNextFrameDropItem;
	}
	else if (Input::LeftMouseHold && m_AimingAtSlot != begin)
	{
		if (m_UseSlot == 0)
		{
			m_PlayerSlots[0] = i_Nothing;
			m_AmountInSlots[0] = 0;
		}

		if (m_AimingAtSlot == 51)
		{

			m_UseSlot = 51;
			if (m_PlayerSlots[0] == i_Nothing)
			{
				m_PlayerSlots[0] = items[m_UseSlot];
				m_AmountInSlots[0] = amount[m_UseSlot];
				items[m_UseSlot] = i_Nothing;
				amount[m_UseSlot] = 0;
			}
			else
			{
				items[m_UseSlot] = m_PlayerSlots[0];
				amount[m_UseSlot] = m_AmountInSlots[0];
				m_UseSlot = 0;
				m_PlayerSlots[0] = items[m_HUDUseSlot];
				m_AmountInSlots[0] = amount[m_HUDUseSlot];
			}
			SwapItemStats();
		}
		else if (Input::CtrlHold)
		{
			if (items[m_AimingAtSlot] != i_Nothing)
			{
				items[51] = items[m_AimingAtSlot];
				amount[51] = amount[m_AimingAtSlot];
				items[m_AimingAtSlot] = i_Nothing;
				amount[m_AimingAtSlot] = 0;
				if (m_UseSlot == 0)
				{
					m_PlayerSlots[0] = items[m_HUDUseSlot];
					m_AmountInSlots[0] = amount[m_HUDUseSlot];
					SwapItemStats();
				}
			}
		}
		else
		{
			m_UseSlot = m_AimingAtSlot;
			if (items[m_UseSlot] == m_PlayerSlots[0] && IsItStackble(m_PlayerSlots[0]) && m_AmountInSlots[0] != 9999 && amount[m_UseSlot] != 9999)
			{
				if (m_AmountInSlots[0] + amount[m_UseSlot] <= 9999)
				{
					amount[m_UseSlot] += m_AmountInSlots[0];
					m_UseSlot = 0;
					m_PlayerSlots[0] = 0;
					m_AmountInSlots[0] = 0;
				}
				else
				{
					short int holdForAmountInSlot = m_AmountInSlots[0] + amount[m_UseSlot] - 9999;
					amount[m_UseSlot] += m_AmountInSlots[0] - holdForAmountInSlot;
					m_AmountInSlots[0] = holdForAmountInSlot;
				}
			}
			else if (m_PlayerSlots[0] == i_Nothing)
			{
				m_PlayerSlots[0] = items[m_UseSlot];
				m_AmountInSlots[0] = amount[m_UseSlot];
				items[m_UseSlot] = i_Nothing;
				amount[m_UseSlot] = 0;
			}
			else if (items[m_UseSlot] == i_Nothing)
			{
				items[m_UseSlot] = m_PlayerSlots[0];
				amount[m_UseSlot] = m_AmountInSlots[0];
				m_UseSlot = 0;
				m_PlayerSlots[0] = items[m_HUDUseSlot];
				m_AmountInSlots[0] = amount[m_HUDUseSlot];
			}
			else
			{
				unsigned short int holdForPlyerSlot = items[m_UseSlot];
				unsigned short int holdForAmountInSlot = amount[m_UseSlot];
				items[m_UseSlot] = m_PlayerSlots[0];
				amount[m_UseSlot] = m_AmountInSlots[0];
				m_PlayerSlots[0] = holdForPlyerSlot;
				m_AmountInSlots[0] = holdForAmountInSlot;
			}
			SwapItemStats();
		}
	}

}

Player::Player(unsigned int eob
	, unsigned int* texturesIDs
	, unsigned int* structuretexs)
{
	m_Recipes[0].CreateRecipe(i_CraftingTable, 1, -1);
	m_Recipes[0].m_Ingredients.emplace_back(i_ForestPlank, 10);

	m_Recipes[1].CreateRecipe(i_CopperIngot, 1, s_Forge);
	m_Recipes[1].m_Ingredients.emplace_back(i_CopperOre, 3);

	m_Recipes[2].CreateRecipe(i_CopperPickaxe, 1, s_Anvil);
	m_Recipes[2].m_Ingredients.emplace_back(i_CopperIngot, 5);
	m_Recipes[2].m_Ingredients.emplace_back(i_ForestPlank, 5);

	m_Recipes[3].CreateRecipe(i_WoodHelmet, 1, s_CraftingTable);
	m_Recipes[3].m_Ingredients.emplace_back(i_ForestPlank, 8);

	m_Recipes[4].CreateRecipe(i_BasicArrow, 2, -1);
	m_Recipes[4].m_Ingredients.emplace_back(i_ForestPlank, 3);

	m_UsingIndexRecipe = 0;
	m_RecipeY = 0;


	m_FloorHit = false;
	m_CeilHit = false;
	m_LeftWallHit = false;
	m_RightWallHit = false;
	m_CoyoteTimer = 0;
	m_JumpTimer = 0;
	m_ArmorClass = 0;
	m_CanJump = false;
	m_DirectionLook = -1;
	m_JumpPower = 12;
	m_Gravity = GRAVITY;
	m_Acceleration = 0;
	m_Friction = 0;
	m_MaxMovementSpeed = 0;
	m_Transform[0] = 150;
	m_Transform[1] = 30;
	m_Velocity[0] = 0;
	m_Velocity[1] = -2;
	m_ArmTimer = 0;
	m_ArmPhase = 0;
	m_ArmsBehaviour = 0;
	m_WalkingTimer = 0;
	m_WalkingPhase = 0;
	m_ArmRotation = 0;
	m_IsInventoryOpen = false;
	m_TimerSplitingItem = 0;
	m_AddNextFrameDropItem = 0;
	m_UseSlot = 0;
	m_HUDUseSlot = 1;
	m_AimingAtSlot = -1;
	for (int i = 0; i < 60; i++)
	{
		m_PlayerSlots[i] = i_Nothing;
	}
	for (int i = 0; i < 52; i++)
	{
		m_AmountInSlots[i] = 0;
	}
	m_UseItemTimer = 0;
	m_CursorOnMinableBlock = false;
	m_CursorOnMinableWall = false;
	m_CursorOnMinableWood = false;
	m_CursorOnPlaceableForStructure = false;
	m_CursorOnPlaceableSpot = false;
	m_CooldownToUse = 6;
	m_WeaponType = weaponNot;
	m_PickaxeStreanght = 0;
	m_AxeStreanght = 0;
	m_HammerStreanght = 0;
	m_Range = 0;
	m_Damage = 0;
	m_Placeable = 0;
	m_LargePlaceable = 0;
	m_LocationAmmunition = -1;
	m_TimerSinceLastHit = 0;
	m_AddNextFrameHP = 0;
	m_HPRegen = 2;
	m_CurrentHealth = 55;
	m_maxHealth = 100;
	m_LastStandingY = 0;
	m_CanDoubleJump = false;
	m_Accessorise = false;
	m_Effects[0] = false;
	m_Effects[1] = false;
	m_Effects[2] = false;
	m_OnFireTimer = 0;
	m_SpeedMultiplier = 1;
	m_IsBurning = false;
	m_AimingAtChest = -1;
	m_IndexOfOpenChest = -1;


	m_BurningTimer = 0;
	m_BurnDamageNextTime = 0;
	float playerVertice[4] = { -1,1.5f,1,-1.5f };
	m_OnFire.constructorFire(playerVertice, 4, 0.2f);
	m_AimingAtDoors = -1;


	m_PlayerSlots[0] = i_Cannon;
	m_AmountInSlots[0] = 1;
	m_PlayerSlots[1] = i_Cannon;
	m_AmountInSlots[1] = 1;
	m_PlayerSlots[2] = i_WoodBow;
	m_AmountInSlots[2] = 1;
	m_PlayerSlots[3] = i_Pistol;
	m_AmountInSlots[3] = 1;

	m_PlayerSlots[4] = i_BasicArrow;
	m_PlayerSlots[5] = i_PierceArrow;
	m_PlayerSlots[6] = i_BouncingArrow;
	m_PlayerSlots[7] = i_FireArrow;
	m_PlayerSlots[8] = i_BasicCannonBall;
	m_PlayerSlots[9] = i_PierceCannonBall;
	m_PlayerSlots[10] = i_BouncingCannonBall;
	m_PlayerSlots[11] = i_FireCannonBall;
	m_PlayerSlots[12] = i_BasicBullet;
	m_PlayerSlots[13] = i_PierceBullet;
	m_PlayerSlots[14] = i_BouncingBullet;
	m_PlayerSlots[15] = i_FireBullet;
	m_PlayerSlots[16] = i_Chest;
	m_PlayerSlots[17] = i_WoodChestPlate;
	m_PlayerSlots[18] = i_WoodPants;
	m_PlayerSlots[19] = i_WoodShoes;
	m_PlayerSlots[20] = i_AccessoriseArrowBag;
	m_PlayerSlots[21] = i_AccessoriseWallClimb;
	m_PlayerSlots[22] = i_AccessoriseFastShoes;
	m_PlayerSlots[23] = i_AccessoriseShackle;
	m_PlayerSlots[24] = i_Forge;
	m_PlayerSlots[25] = i_CraftingTable;
	m_PlayerSlots[26] = i_Anvil;
	m_PlayerSlots[27] = i_CopperIngot;
	m_PlayerSlots[28] = i_ForestPlank;
	m_AmountInSlots[4] = 9999;
	m_AmountInSlots[5] = 20;
	m_AmountInSlots[6] = 20;
	m_AmountInSlots[7] = 20;
	m_AmountInSlots[8] = 20;
	m_AmountInSlots[9] = 20;
	m_AmountInSlots[10] = 20;
	m_AmountInSlots[11] = 20;
	m_AmountInSlots[12] = 20;
	m_AmountInSlots[13] = 20;
	m_AmountInSlots[14] = 20;
	m_AmountInSlots[15] = 20;
	m_AmountInSlots[16] = 1;
	m_AmountInSlots[17] = 1;
	m_AmountInSlots[18] = 1;
	m_AmountInSlots[19] = 1;
	m_AmountInSlots[20] = 1;
	m_AmountInSlots[21] = 1;
	m_AmountInSlots[22] = 1;
	m_AmountInSlots[23] = 1;
	m_AmountInSlots[24] = 1;
	m_AmountInSlots[25] = 1;
	m_AmountInSlots[26] = 1;
	m_AmountInSlots[27] = 8;
	m_AmountInSlots[28] = 3000;


	m_PlayerSlots[41] = i_CopperSword;
	m_PlayerSlots[42] = i_CopperPickaxe;
	m_PlayerSlots[43] = i_CopperAxe;
	m_PlayerSlots[44] = i_CopperHammer;
	m_PlayerSlots[45] = i_Dirt;
	m_PlayerSlots[46] = i_Sand;
	m_PlayerSlots[47] = i_Sapling;
	m_AmountInSlots[41] = 1;
	m_AmountInSlots[42] = 1;
	m_AmountInSlots[43] = 1;
	m_AmountInSlots[44] = 1;
	m_AmountInSlots[45] = 9999;
	m_AmountInSlots[46] = 20;
	m_AmountInSlots[47] = 20;

	m_BootsAnimTex = CreateTextureRGBA("res/textures/bootsAnimDefault.png");
	m_LegAnimTex = CreateTextureRGBA("res/textures/legAnimDefault.png");
	m_BodyAnimTex = CreateTextureRGBA("res/textures/bodyAnimDefault.png");
	m_HeadTex = CreateTextureRGBA("res/textures/headDefault.png");
	m_HandTex = CreateTextureRGBA("res/textures/handDefault.png");

	m_AllItemTextures[i_CopperPickaxe] = CreateTextureRGBA("res/textures/cooperPickaxe.png");
	m_AllItemTextures[i_CopperAxe] = CreateTextureRGBA("res/textures/cooperAxe.png");
	m_AllItemTextures[i_CopperHammer] = CreateTextureRGBA("res/textures/cooperHammer.png");
	m_AllItemTextures[i_CopperSword] = CreateTextureRGBA("res/textures/cooperSword.png");
	m_AllItemTextures[i_ForestPlank] = texturesIDs[t_ForestPlank];
	m_AllItemTextures[i_Dirt] = texturesIDs[t_Dirt];
	m_AllItemTextures[i_Ice] = texturesIDs[t_Ice];
	m_AllItemTextures[i_Asphalt] = texturesIDs[t_Asphalt];
	m_AllItemTextures[i_Platform] = texturesIDs[t_Platform];
	m_AllItemTextures[i_WallDirt] = texturesIDs[t_Dirt];
	m_AllItemTextures[i_WallIce] = texturesIDs[t_Ice];
	m_AllItemTextures[i_Sand] = texturesIDs[t_Sand];
	m_AllItemTextures[i_Sapling] = CreateTextureRGBA("res/textures/saplingInv.png");
	m_AllItemTextures[i_WoodBow] = CreateTextureRGBA("res/textures/bowInInv.png");
	m_AllItemTextures[i_BasicArrow] = CreateTextureRGBA("res/textures/basicArrow.png");
	m_AllItemTextures[i_PierceArrow] = CreateTextureRGBA("res/textures/BleedArrow.png");
	m_AllItemTextures[i_BouncingArrow] = CreateTextureRGBA("res/textures/bouncingArrow.png");
	m_AllItemTextures[i_FireArrow] = CreateTextureRGBA("res/textures/fireArrow.png");
	m_AllItemTextures[i_Cannon] = CreateTextureRGBA("res/textures/canonInv.png");
	m_AllItemTextures[i_BasicCannonBall] = CreateTextureRGBA("res/textures/basicCanonBall.png");
	m_AllItemTextures[i_PierceCannonBall] = CreateTextureRGBA("res/textures/scrapCannonBall.png");
	m_AllItemTextures[i_BouncingCannonBall] = CreateTextureRGBA("res/textures/bouncingCannonBall.png");
	m_AllItemTextures[i_FireCannonBall] = CreateTextureRGBA("res/textures/fireCannonBall.png");
	m_AllItemTextures[i_Pistol] = CreateTextureRGBA("res/textures/pistolInv.png");
	m_AllItemTextures[i_BasicBullet] = CreateTextureRGBA("res/textures/basicBullet.png");
	m_AllItemTextures[i_PierceBullet] = CreateTextureRGBA("res/textures/BleedBullet.png");
	m_AllItemTextures[i_BouncingBullet] = CreateTextureRGBA("res/textures/bouncingBullet.png");
	m_AllItemTextures[i_FireBullet] = CreateTextureRGBA("res/textures/fireBullet.png");
	m_AllItemTextures[i_WoodHelmet] = CreateTextureRGBA("res/textures/woodHelmet.png");
	m_AllItemTextures[i_WoodChestPlate] = CreateTextureRGBA("res/textures/woodChestPlate.png");
	m_AllItemTextures[i_WoodPants] = CreateTextureRGBA("res/textures/woodPants.png");
	m_AllItemTextures[i_WoodShoes] = CreateTextureRGBA("res/textures/woodShoes.png");
	m_AllItemTextures[i_AccessoriseArrowBag] = CreateTextureRGBA("res/textures/accessoriseArrowBag.png");
	m_AllItemTextures[i_AccessoriseWallClimb] = CreateTextureRGBA("res/textures/accessoriseWallClimb.png");
	m_AllItemTextures[i_AccessoriseFastShoes] = CreateTextureRGBA("res/textures/accessoriseFastShoes.png");
	m_AllItemTextures[i_Chest] = structuretexs[s_Chest];
	m_AllItemTextures[i_AccessoriseShackle] = CreateTextureRGBA("res/textures/accessoriseShackle.png");
	m_AllItemTextures[i_IronOre] = CreateTextureRGBA("res/textures/iron.png");
	m_AllItemTextures[i_CopperOre] = CreateTextureRGBA("res/textures/copper.png");
	m_AllItemTextures[i_IronIngot] = CreateTextureRGBA("res/textures/ironIngot.png");
	m_AllItemTextures[i_CopperIngot] = CreateTextureRGBA("res/textures/copperIngot.png");
	m_AllItemTextures[i_CraftingTable] = CreateTextureRGBA("res/textures/benchInv.png");
	m_AllItemTextures[i_Forge] = CreateTextureRGBA("res/textures/forgeInv.png");
	m_AllItemTextures[i_Anvil] = CreateTextureRGBA("res/textures/anvilInv.png");

	m_ArmorSlotsTex[0] = CreateTextureRGBA("res/textures/HelmetSlot.png");
	m_ArmorSlotsTex[1] = CreateTextureRGBA("res/textures/ChestPlateSlot.png");
	m_ArmorSlotsTex[2] = CreateTextureRGBA("res/textures/PantsSlot.png");
	m_ArmorSlotsTex[3] = CreateTextureRGBA("res/textures/ShoesSlot.png");
	m_ArmorClassTex = CreateTextureRGBA("res/textures/ArmorClass.png");

	m_BulletsDD  = CreateDrawData(eob,0.3f,-0.3f,0.2f,-0.2f);
	m_ItemsInHandDD[InHandBow] = CreateDrawData(eob, 1.5f, 0.5f, 1, -1);
	m_ItemsInHandTexture[InHandBow] = CreateTextureRGBA("res/textures/bowInHand.png");
	m_ItemsInHandDD[InHandCanon] = CreateDrawData(eob, 3, 1, 0.5f, -0.5f);
	m_ItemsInHandTexture[InHandCanon] = CreateTextureRGBA("res/textures/canonHand.png");
	m_ItemsInHandDD[InHandPistol] = CreateDrawData(eob, 2.5f, 1, 0.5f, -0.7f);
	m_ItemsInHandTexture[InHandPistol] = CreateTextureRGBA("res/textures/pistolHand.png");

	float vertices[16];
	vertices[0] = 0; vertices[1] = 3; vertices[2] = 0; vertices[3] = 1;
	vertices[4] = 1; vertices[5] = 2; vertices[6] = 1; vertices[7] = 1;
	vertices[8] = 0; vertices[9] = 1; vertices[10] = 1; vertices[11] = 0;
	vertices[12] = -1; vertices[13] = 2;  vertices[14] = 0; vertices[15] = 0;
	unsigned int vertexBuffer;

	ErrorGL(glGenVertexArrays(1, &m_ItemInHandDD));
	ErrorGL(glBindVertexArray(m_ItemInHandDD));
	ErrorGL(glGenBuffers(1, &vertexBuffer));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

	ErrorGL(glBindVertexArray(0));
	m_BlockInHandDD = CreateDrawData(eob, 2, 1, 0, -1);
	m_HandDD = CreateDrawData(eob, 1.5f, 0, -0.2f, 0.2f, 0, 1);
	m_BottomAnimDD = CreateDrawData(eob, -0.2f, -1.5f, -1, 1, 1, 0,  0, 1.0f / 5.0f);
	m_BodyAnimDD = CreateDrawData(eob, 1.5f, -0.3, -1, 1, 1, 0, 0, 1.0f / 5.0f);
	m_HeadDD = CreateDrawData(eob, 1.5f, -0.3, -1, 1, 1, 0, 0, 1);
	unsigned int inventoryVertexBuffer;
	float inventoryVertices[20];
	float left = 0.02f * Window::width;
	float top = Window::height - 0.01f * Window::height;
	float workSpace = (Window::width / 2.0f) - left;
	float canNotDivide = ((int)std::floor(workSpace) % (18 + Window::scaleOfHUD)) - std::floor(workSpace) + workSpace;
	float right = ((workSpace - canNotDivide) / (18.0f + Window::scaleOfHUD)) + left;
	float down = top - (right - left);
	m_SlotGap = right - left + (right - left)/16.0f;
	
	
	m_InvOffset[0] = (left + right) / 2.0f;
	m_HPOffset[0] = Window::width - (left + right) / 2.0f;
	m_InvOffset[1] = (top + down) / 2.0f;
	m_HPOffset[1] = m_InvOffset[1];
	down -= m_InvOffset[1];
	top -= m_InvOffset[1];
	left -= m_InvOffset[0];
	right -= m_InvOffset[0];
	m_HalfOfSlotLeanght = -left;
	m_HUDDD = CreateDrawData(eob, top, down, right, left);
	m_HPTexture[0] = CreateTextureRGBA("res/textures/0To5HP.png");
	m_HPTexture[1] = CreateTextureRGBA("res/textures/5To10HP.png");
	m_HPTexture[2] = CreateTextureRGBA("res/textures/10To15HP.png");
	m_HPTexture[3] = CreateTextureRGBA("res/textures/15To20HP.png");
	m_HPTexture[4] = CreateTextureRGBA("res/textures/20To25HP.png");


	m_SlotTexture = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_UseSlotTexture = CreateTextureRGBA("res/textures/useInventorySlot.png");
	m_TrashCanSlotTexture = CreateTextureRGBA("res/textures/trash.png");
	m_MissingSlotTexture = CreateTextureRGBA("res/textures/missingIngredientSlot.png");
	m_NothingSlotTexture = CreateTextureRGBA("res/textures/nothingIngredientSlot.png");
	m_ChestSlotTexture = CreateTextureRGBA("res/textures/ChestSlot.png");

	SwapItemStats();
	
}
void Player::DamagePlayer(float* transfromAttacker
	, int Damage)
{
	if (transfromAttacker)
	{
		int x = transfromAttacker[0] - m_Transform[0];
		int y = transfromAttacker[1] - m_Transform[1];
		if (x)
		{
			x = abs(x) / x;
		}
		else
		{
			x = 0;
		}

		if (y)
		{
			m_Velocity[1] = 5;
		}


		m_Velocity[0] = 10 * -x;
		if (Damage - m_ArmorClass >= 0)
		{
			m_CurrentHealth -= Damage - m_ArmorClass;
		}
		else
		{
			m_CurrentHealth--;
		}
	}
	else
	{
		m_CurrentHealth -= Damage;

	}
	m_TimerSinceLastHit = 0;

	if (m_CurrentHealth < 0)
	{
		m_IsBurning = false;
		m_BurningTimer = 0;
		m_Transform[0] = Blocks::xMax / 2.0f;
		m_Transform[1] = 15;
		m_LastStandingY = 15;
		m_CurrentHealth = m_maxHealth/2;
	}
}
void Player::SwapItemStats()
{
	m_CooldownToUse = 0.4;
	m_PickaxeStreanght = 0;
	m_AxeStreanght = 0;
	m_HammerStreanght = 0;
	m_Range = 4;
	m_Damage = 0;
	m_WeaponType = weaponNot;
	m_Consume = false;
	m_Placeable = false;
	m_LargePlaceable = false;
	m_Accessorise = false;

	switch (m_PlayerSlots[0])
	{
		break;
		case(i_CopperSword):
			m_Damage = 2;
		break;
		
		case(i_CopperAxe):
			m_AxeStreanght = 35;
			m_Damage = 1;
		break;
		

		case(i_CopperPickaxe):
			m_PickaxeStreanght = 35;
			m_Damage = 1;
		break;
		
		case(i_CopperHammer):
			m_HammerStreanght = 35;
			m_Damage = 1;
		break;
		case i_Sapling:
		case i_CraftingTable:
		case i_Forge:
		case i_Anvil:
		case i_Chest:
			m_CooldownToUse = 0.1f;
			m_LargePlaceable = true;
			break;
		case i_WoodBow:
			m_Range = 0;
			m_Damage = 4;
			m_WeaponType = weaponBow;
			m_CooldownToUse = 0.8;
			break;
		case i_Cannon:
			m_Range = 0;
			m_Damage = 10;
			m_WeaponType = weaponCanon;
			m_CooldownToUse = 1.2;
			break;
		case i_Pistol:
			m_Range = 0;
			m_Damage = 15;
			m_WeaponType = weaponGun;
			m_CooldownToUse = 0.4;
			break;
		case i_WoodHelmet:
		case i_WoodChestPlate:
		case i_WoodPants:
		case i_WoodShoes:
			m_Range = 0;
			m_Consume = true;
			m_CooldownToUse = 0.8;
			break;
		case i_AccessoriseArrowBag:
		case i_AccessoriseWallClimb:
		case i_AccessoriseFastShoes:
		case i_AccessoriseShackle:
			m_CooldownToUse = 0;
			m_Range = 0;
			m_Accessorise = true;
			break;
		case i_CopperOre:
		case i_CopperIngot:
		case i_IronOre:
		case i_IronIngot:
		case i_PierceArrow:
		case i_BasicArrow:
		case i_BouncingArrow:
		case i_FireArrow:
		case i_BasicCannonBall:
		case i_PierceCannonBall:
		case i_BouncingCannonBall:
		case i_FireCannonBall:
		case i_BasicBullet:
		case i_PierceBullet:
		case i_BouncingBullet:
		case i_FireBullet:
		case i_Nothing:
			m_CooldownToUse = 10;
			m_Range = 0;
			break;
		default:
			m_CooldownToUse = 0.1f;
			m_Placeable = true;
			break;
	}
	if (m_WeaponType == weaponBow && m_Effects[effectArrowBag])
	{
		m_WeaponType = weaponBow + weaponAutomatic;
	}

}

void Player::SwapArmor(unsigned char SlotIndex
	, char armorPart)
{
	int holder = m_PlayerSlots[SlotIndex];

	switch (armorPart)
	{
	case armorHelmet:
		m_PlayerSlots[SlotIndex] = m_PlayerSlots[52];
		m_PlayerSlots[52] = holder;
		ErrorGL(glDeleteTextures(1, &m_HeadTex));
		switch (holder)
		{
		case i_Nothing:
			m_HeadTex = CreateTextureRGBA("res/textures/headDefault.png");
			break;
		case i_WoodHelmet:
			m_HeadTex = CreateTextureRGBA("res/textures/headWood.png");
			break;
		default:
			std::cout << "Error Player.cpp unknow Helmet" << holder << std::endl;
			m_HeadTex = CreateTextureRGBA("res/textures/headDefault.png");
			break;
		}
		m_ArmorClass += ArmorClass(m_PlayerSlots[52]);
		break;
	case armorChestPlate:
		m_PlayerSlots[SlotIndex] = m_PlayerSlots[53];
		m_PlayerSlots[53] = holder;
		ErrorGL(glDeleteTextures(1, &m_BodyAnimTex));
		ErrorGL(glDeleteTextures(1, &m_HandTex));
		switch (holder)
		{
		case i_Nothing:
			m_HandTex = CreateTextureRGBA("res/textures/handDefault.png");
			m_BodyAnimTex = CreateTextureRGBA("res/textures/bodyAnimDefault.png");
			break;
		case i_WoodChestPlate:
			m_HandTex = CreateTextureRGBA("res/textures/handWood.png");
			m_BodyAnimTex = CreateTextureRGBA("res/textures/bodyAnimWood.png");
			break;
		default:
			std::cout << "Error Player.cpp unknow ChestPlate" << holder << std::endl;
			m_HandTex = CreateTextureRGBA("res/textures/handDefault.png");
			m_BodyAnimTex = CreateTextureRGBA("res/textures/bodyAnimDefault.png");
			break;
		}
		m_ArmorClass += ArmorClass(m_PlayerSlots[53]);
		break;
	case armorPants:
		m_PlayerSlots[SlotIndex] = m_PlayerSlots[54];
		m_PlayerSlots[54] = holder;
		ErrorGL(glDeleteTextures(1, &m_LegAnimTex));
		switch (holder)
		{
		case i_Nothing:
			m_LegAnimTex = CreateTextureRGBA("res/textures/legAnimDefault.png");
			break;
		case i_WoodPants:
			m_LegAnimTex = CreateTextureRGBA("res/textures/legAnimWood.png");
			break;
		default:
			std::cout << "Error Player.cpp unknow Pants" << holder << std::endl;
			m_LegAnimTex = CreateTextureRGBA("res/textures/legAnimDefault.png");
			break;
		}
		m_ArmorClass += ArmorClass(m_PlayerSlots[54]);
		break;
	case armorShoes:
		m_PlayerSlots[SlotIndex] = m_PlayerSlots[55];
		m_PlayerSlots[55] = holder;
		ErrorGL(glDeleteTextures(1, &m_BootsAnimTex));
		switch (holder)
		{
		case i_Nothing:
			m_BootsAnimTex = CreateTextureRGBA("res/textures/bootsAnimDefault.png");
			break;
		case i_WoodShoes:
			m_BootsAnimTex = CreateTextureRGBA("res/textures/bootsAnimWood.png");
			break;
		default:
			std::cout << "Error Player.cpp unknow Shoes" << holder << std::endl;
			m_BootsAnimTex = CreateTextureRGBA("res/textures/bootsAnimDefault.png");
			break;
		}
		m_ArmorClass += ArmorClass(m_PlayerSlots[55]);
		break;
	default:
		std::cout << "Error Player.cpp unknow armor part: " << armorPart << std::endl;
		break;
	}
	m_ArmorClass -= ArmorClass(m_PlayerSlots[SlotIndex]);
	if (m_PlayerSlots[SlotIndex] == i_Nothing)
	{
		m_AmountInSlots[SlotIndex] = 0;
	}
}
void Player::SwapAccessorise(unsigned char invSlotIndex
	, unsigned char accessoriseSlotIndex)
{

	switch (m_PlayerSlots[invSlotIndex])
	{
	case i_AccessoriseArrowBag:
		m_Effects[effectArrowBag] = true;
		break;
	case i_AccessoriseFastShoes:
		m_SpeedMultiplier *= 2;
		break;
	case i_AccessoriseWallClimb:
		m_Effects[effectWallClimb] = true;
		break;
	case i_AccessoriseShackle:
		m_ArmorClass += 2;
		break;
	case i_Nothing:
		break;
	default:
		std::cout << "Error Player.cpp unknnow Accessorise Input : " << m_PlayerSlots[invSlotIndex] << std::endl;
		break;
	}
	switch (m_PlayerSlots[accessoriseSlotIndex])
	{
	case i_AccessoriseArrowBag:
		m_Effects[effectArrowBag] = false;
		break;
	case i_AccessoriseFastShoes:
		m_SpeedMultiplier /= 2;
		break;
	case i_AccessoriseWallClimb:
		m_Effects[effectWallClimb] = false;
		break;
	case i_AccessoriseShackle:
		m_ArmorClass -= 2;
		break;
	case i_Nothing:
		break;
	default:
		std::cout << "Error Player.cpp unknnow Accessorise output : " << m_PlayerSlots[accessoriseSlotIndex] << std::endl;
		break;
	}
	int holder = m_PlayerSlots[invSlotIndex];
	m_PlayerSlots[invSlotIndex] = m_PlayerSlots[accessoriseSlotIndex];
	m_PlayerSlots[accessoriseSlotIndex] = holder;
	if (m_PlayerSlots[invSlotIndex] == i_Nothing)
	{
		m_AmountInSlots[invSlotIndex] = 0;
	}
	else
	{
		m_AmountInSlots[invSlotIndex] = 1;
	}

}

bool Player::IsItStackble(unsigned short int item)
{
	switch (item)
	{
	case i_CopperHammer:
	case i_CopperPickaxe:
	case i_CopperAxe:
	case i_CopperSword:
	case i_WoodBow:
	case i_Pistol:
	case i_Cannon:
	case i_WoodHelmet:
	case i_WoodChestPlate:
	case i_WoodPants:
	case i_WoodShoes:
		return false;
	default: 
		return true;
	}
}
char Player::FindItemInInv(unsigned char item)
{
	for (int i = 0; i < 52; i++)
	{
		if (m_PlayerSlots[i] == item)
		{
			return i;
		}
	}
	return -1;
}
char Player::FindOneOfItemsInInv(unsigned char* items
	, int sizeOfArray)
{
	for (int i = 0; i < 52; i++)
	{
		for (int j = 0; j < sizeOfArray; j++)
		{
			if (m_PlayerSlots[i] == items[j])
			{
				return i;
			}
		}
	}
	return -1;
}
bool Player::HavePlayerSpace(unsigned short int item)
{
	if (IsItStackble(item))
	{
		for (int i = 1; i < 51; i++)
		{
			if (m_PlayerSlots[i] == i_Nothing)
			{
				return true;
			}
			else if (m_PlayerSlots[i] == item && m_AmountInSlots[i] < 9999)
			{
				return true;
			}
		}
	}
	else
	{
		for (int i = 1; i < 51; i++)
		{
			if (m_PlayerSlots[i] == i_Nothing)
			{
				return true;
			}
		}
	}
	return false;
}
bool Player::ItermGetToInventory(unsigned short int& amount
	, unsigned short int item)
{
	bool isItDone = false;
	if (IsItStackble(item))
	{
		unsigned char freeSlot = 0;
		
		for (int i = 1; i < 51; i++)
		{
			if (m_PlayerSlots[i] == i_Nothing && freeSlot == 0)
			{
				freeSlot = i;
			}
			else if (m_PlayerSlots[i] == item)
			{
				if (m_AmountInSlots[i] < 9999)
				{
					unsigned short int amountSlotOverflow = m_AmountInSlots[i] + amount;
					if (amountSlotOverflow <= 9999)
					{
						m_AmountInSlots[i] = amountSlotOverflow;
						isItDone = true;
						break;
					}
					else
					{
						amountSlotOverflow =  amountSlotOverflow - 9999;
						m_AmountInSlots[i] = m_AmountInSlots[i] + amount - amountSlotOverflow;
						amount = amountSlotOverflow;
					}
				}
			}
		}
		if (!isItDone && freeSlot)
		{
			m_AmountInSlots[freeSlot] = amount;
			m_PlayerSlots[freeSlot] = item;
			isItDone = true;
		}
	}
	else
	{
		for (int i = 1; i < 51; i++)
		{
			if (m_PlayerSlots[i] == i_Nothing)
			{
				m_PlayerSlots[i] = item;
				m_AmountInSlots[i] = amount;
				isItDone = true;
				break;
			}
		}
	}
	return isItDone;
}

void Player::EveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<Enemy>& enemies
	, std::vector<int>& isThereSandOnX
	, std::vector<CraftStation>& craftStations
	, std::vector<damagedWood>& damagedWoods
	, std::vector<DamagedBlock>& damageblocks
	, std::vector<DamagedBlock>& damagedWalls
	, float* CameraCoordinates
	, unsigned int blockDD
	, unsigned int* texturesIDs
	, unsigned int* structuresTextures
	, std::vector<tree>& trees
	, std::vector<seedling>& seedlings
	, std::vector<DroppedItem>& droppedItems
	, std::vector<Projectile>& projectiles
	, std::vector<Door>& doors
	, std::vector<Chest>& chests)
{
	float oldVelocity[2] = { m_Velocity[0], m_Velocity[1] };
	if (chests.size() > m_IndexOfOpenChest && m_IndexOfOpenChest != -1)
	{
		if (CHESTANDDOORSREACH < Pyt2D(m_Transform[0] - chests.at(m_IndexOfOpenChest).m_Transform[0], m_Transform[1] - chests.at(m_IndexOfOpenChest).m_Transform[1]))
		{
			chests.at(m_IndexOfOpenChest).m_Open = false;
			m_IndexOfOpenChest = -1;
		}
	}
	else
	{
		m_IndexOfOpenChest = -1;
	}
	//Crafting 
	if (m_IsInventoryOpen)
	{
		bool isCloseToCraftStation[3];
		isCloseToCraftStation[s_CraftingTable] = false;
		isCloseToCraftStation[s_Forge] = false;
		isCloseToCraftStation[s_Anvil] = false;

		for (int i = 0; i < craftStations.size(); i++)
		{
			if (m_Transform[1] == craftStations.at(i).m_Transform[1] + 1)
			{
				int vertices[4];
				getStructureVertices(craftStations.at(i).m_Transform[0], craftStations.at(i).m_Transform[1], craftStations.at(i).m_CraftStationtype, vertices);
				if (abs(m_Transform[0] - (float)(vertices[0] + vertices[2]) / 2.0f) < 5)
				{
					Assert(2 < craftStations.at(i).m_CraftStationtype);
					isCloseToCraftStation[craftStations.at(i).m_CraftStationtype] = true;
				}
			}
		}
		m_NumberOfVisibleRecipes = 0;
		for (int l = 0; l < sizeof(m_Recipes) / sizeof(m_Recipes[0]); l++)
		{
			if (m_Recipes[l].m_CraftingStation != -1)
			{
				Assert(3 <= m_Recipes[l].m_CraftingStation);
				if (!isCloseToCraftStation[m_Recipes[l].m_CraftingStation])
				{
					m_Recipes[l].m_CraftingState = cantCraft;
					continue;
				}
			}
			bool canCraft = true;
			bool missingCraft = false;

			for (int i = 0; i < m_Recipes[l].m_Ingredients.size(); i++)
			{

				int amount = 0;
				for (int j = 1; j < 51; j++)
				{
					if (m_Recipes[l].m_Ingredients.at(i).m_Item == m_PlayerSlots[j])
					{

						amount += m_AmountInSlots[j];
					}
				}
				float numberOfCraft = (float)amount / (float)m_Recipes[l].m_Ingredients.at(i).m_Amount;
				m_Recipes[l].m_Ingredients.at(i).m_NumberOfPossibleCraft = floorf(numberOfCraft);
				if (numberOfCraft >= 1)
				{
					missingCraft = true;
					m_Recipes[l].m_Ingredients.at(i).m_CraftingState = ReadyToCraft;
				}
				else if (numberOfCraft == 0)
				{
					canCraft = false;
					m_Recipes[l].m_Ingredients.at(i).m_CraftingState = cantCraft;
				}
				else 
				{
					canCraft = false;
					missingCraft = true;
					m_Recipes[l].m_Ingredients.at(i).m_CraftingState = missingToCraft;
				}
			}
			if (canCraft)
			{

				m_Recipes[l].m_CraftingState = ReadyToCraft;
				m_Recipes[l].m_MaximumCraft = m_Recipes[l].m_Ingredients.at(0).m_NumberOfPossibleCraft;
				for (int i = 1; i < m_Recipes[l].m_Ingredients.size(); i++)
				{
					if (m_Recipes[l].m_MaximumCraft > m_Recipes[l].m_Ingredients.at(0).m_NumberOfPossibleCraft)
					{
						m_Recipes[l].m_MaximumCraft = m_Recipes[l].m_Ingredients.at(0).m_NumberOfPossibleCraft;
					}
				}
				m_VisibleRecipes[m_NumberOfVisibleRecipes] = m_Recipes[l];
				m_NumberOfVisibleRecipes++;
				
			}
			else if (missingCraft)
			{
				m_Recipes[l].m_CraftingState = missingToCraft;
				m_VisibleRecipes[m_NumberOfVisibleRecipes] = m_Recipes[l];
				m_NumberOfVisibleRecipes++;
			}
			else
			{
				m_Recipes[l].m_CraftingState = cantCraft;
			}

		}
		if (m_UsingIndexRecipe >= m_NumberOfVisibleRecipes && m_NumberOfVisibleRecipes != 0)
		{
			m_UsingIndexRecipe = m_NumberOfVisibleRecipes - 1;
		}
		if (m_RecipeY != m_UsingIndexRecipe)
		{
			float distanceBefore = m_UsingIndexRecipe - m_RecipeY;
			m_RecipeY += (abs(distanceBefore) / distanceBefore * (abs(distanceBefore) + 1)) * 5 * deltaTime;
			float distanceAfter = m_UsingIndexRecipe - m_RecipeY;
			if (distanceAfter)
			{
				if (abs(distanceAfter) / distanceAfter != abs(distanceBefore) / distanceBefore)
					m_RecipeY = m_UsingIndexRecipe;
			}

		}
		
		if (m_RecipeY <= 0)
		{
			m_RecipeY = 0;
		}
		else if (m_RecipeY >= m_NumberOfVisibleRecipes)
		{
			m_RecipeY = (m_NumberOfVisibleRecipes - 1);
		}
		if (Input::MouseWheel)
		{
			m_UsingIndexRecipe += Input::MouseWheel;

			if (m_UsingIndexRecipe <= 0)
			{
				m_UsingIndexRecipe = 0;
			}
			else if (m_UsingIndexRecipe >= m_NumberOfVisibleRecipes)
			{
				m_UsingIndexRecipe = m_NumberOfVisibleRecipes - 1;
			}
		}
	}

	// inventory moving 
	if (m_ArmsBehaviour != ArmUsing && (!Input::LeftMouseHold || Input::LeftMousePress || m_TimerCrafting != 1) && !Input::LeftMouseRelease)
	{
		m_AimingAtSlot = 0;
		int slotCoordinates[2] = { -1,-1 };
		// get slotCoordinates
		{
			float slotVertices[4] = { m_InvOffset[0] - m_HalfOfSlotLeanght
									, Window::height - m_InvOffset[1] - m_HalfOfSlotLeanght
									, m_InvOffset[0] + m_HalfOfSlotLeanght
									, Window::height - m_InvOffset[1] + m_HalfOfSlotLeanght };
			if (slotVertices[0] + m_SlotGap * 10 + CRAFTINGOFFSET * (m_SlotGap - 2 * m_HalfOfSlotLeanght) <= Input::XRawMousePos &&
				slotVertices[2] + m_SlotGap * 10 + CRAFTINGOFFSET * (m_SlotGap - 2 * m_HalfOfSlotLeanght) >= Input::XRawMousePos)
			{
				slotCoordinates[0] = 10;
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					if (slotVertices[0] + i * m_SlotGap <= Input::XRawMousePos && slotVertices[2] + i * m_SlotGap >= Input::XRawMousePos)
					{
						slotCoordinates[0] = i;
						break;
					}
				}
			}
			if (m_IndexOfOpenChest != -1)
			{
				for (int i = 0; i < 11; i++)
				{
					if (slotVertices[1] + i * m_SlotGap <= Input::YRawMousePos && slotVertices[3] + i * m_SlotGap >= Input::YRawMousePos)
					{
						slotCoordinates[1] = i;
						break;
					}
				}
			}
			else
			{
			for (int i = 0; i < 6; i++)
			{
				if (slotVertices[1] + i * m_SlotGap <= Input::YRawMousePos && slotVertices[3] + i * m_SlotGap >= Input::YRawMousePos)
				{
					slotCoordinates[1] = i;
					break;
				}
			}

			}
			slotVertices[0] = m_HPOffset[0] - m_HalfOfSlotLeanght;
			slotVertices[1] = Window::height - m_HPOffset[1] - m_HalfOfSlotLeanght;
			slotVertices[2] = m_HPOffset[0] + m_HalfOfSlotLeanght;
			slotVertices[3] = Window::height - m_HPOffset[1] + m_HalfOfSlotLeanght;
			if (slotVertices[0] <= Input::XRawMousePos && slotVertices[2] >= Input::XRawMousePos)
			{
				slotCoordinates[0] = 11;
				for (int i = 3; i < 11; i++)
				{
					if (slotVertices[1] + i * m_SlotGap <= Input::YRawMousePos && slotVertices[3] + i * m_SlotGap >= Input::YRawMousePos)
					{
						slotCoordinates[1] = i - 3;
						break;
					}
				}
			}
		}
		if (slotCoordinates[0] != -1 && slotCoordinates[1] != -1 )
		{
			if (m_IsInventoryOpen)
			{

				if (slotCoordinates[0] == 11)
				{
					// armor accessorise swap
					m_AimingAtSlot = slotCoordinates[1] + 52;

					if (m_PlayerSlots[m_AimingAtSlot] == i_Nothing && (m_UseSlot == 0 || m_PlayerSlots[0] == i_Nothing))
					{
						m_AimingAtSlot = 0;
					}
					if (Input::LeftMouseHold && m_AimingAtSlot)
					{
						char typeOfArmor = WhatPartOfArmor(m_PlayerSlots[0]);

						if (m_UseSlot == 0)
						{
							m_PlayerSlots[0] = i_Nothing;
							m_AmountInSlots[0] = 0;
						}

						if ((m_Accessorise || m_PlayerSlots[0] == i_Nothing) && m_AimingAtSlot > 55)
						{
							SwapAccessorise(0, m_AimingAtSlot);
							if (m_UseSlot == 0)
							{
								m_UseSlot = 1;
							}
							SwapItemStats();
						}
						else if (typeOfArmor == -1)
						{

							switch (m_AimingAtSlot)
							{
							case 52:
								SwapArmor(0, armorHelmet);
								break;
							case 53:
								SwapArmor(0, armorChestPlate);
								break;
							case 54:
								SwapArmor(0, armorPants);
								break;
							case 55:
								SwapArmor(0, armorShoes);
								break;
							}
							if (m_UseSlot == 0)
							{
								m_UseSlot = 1;
							}
							SwapItemStats();
						}
						else if (typeOfArmor >= 0)
						{
							SwapArmor(0, typeOfArmor);
							if (m_UseSlot == 0)
							{
								m_UseSlot = 1;
							}
							SwapItemStats();
						}
					}

				}
				else if (slotCoordinates[0] == 10 && slotCoordinates[1] != 5 && m_RecipeY == m_UsingIndexRecipe)
				{
					// crafting

					m_AimingAtSlot = slotCoordinates[1] + 1;

					int aimingSlot = m_AimingAtSlot - 3;
					if (-m_UsingIndexRecipe > aimingSlot || m_NumberOfVisibleRecipes - m_UsingIndexRecipe <= aimingSlot)
					{
						m_AimingAtSlot = 0;
						aimingSlot = 0;
					}
					if (m_AimingAtSlot && Input::LeftMouseHold)
					{
						if (0 == aimingSlot && m_VisibleRecipes[m_UsingIndexRecipe].m_CraftingState == ReadyToCraft && (m_VisibleRecipes[m_UsingIndexRecipe].m_ItemOutput == m_PlayerSlots[0] && IsItStackble(m_VisibleRecipes[m_UsingIndexRecipe].m_ItemOutput) || m_UseSlot == 0 || m_PlayerSlots[0] == i_Nothing))
						{
							if (m_TimerCrafting == 1 && (m_AmountInSlots[0] + m_VisibleRecipes[m_UsingIndexRecipe].m_AmountOutput <= 9999 || m_UseSlot == 0))
							{
								m_TimerCrafting += deltaTime;

								if (m_UseSlot == 0)
								{
									m_AmountInSlots[0] = 0;
								}
								m_AmountInSlots[0] += m_VisibleRecipes[m_UsingIndexRecipe].m_AmountOutput;
								m_PlayerSlots[0] = m_VisibleRecipes[m_UsingIndexRecipe].m_ItemOutput;

								m_UseSlot = 1;

								for (int i = 0; i < m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.size(); i++)
								{
									int amountLeft = m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.at(i).m_Amount;
									for (int j = 1; j < 51; j++)
									{
										if (m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.at(i).m_Item == m_PlayerSlots[j])
										{

											amountLeft -= m_AmountInSlots[j];
											if (amountLeft <= 0)
											{
												m_AmountInSlots[j] = abs(amountLeft);
												break;
											}
											else if (amountLeft == 0)
											{
												m_PlayerSlots[j] = i_Nothing;
												m_AmountInSlots[j] = 0;
												break;
											}
										}
									}
								}
								SwapItemStats();

							}
							else if (m_AmountInSlots[0] + m_VisibleRecipes[m_UsingIndexRecipe].m_AmountOutput * floorf(m_NumberOfRecipesDone) < 9999)
							{
								if (m_VisibleRecipes[m_UsingIndexRecipe].m_MaximumCraft < floorf(m_NumberOfRecipesDone))
								{
									m_NumberOfRecipesDone = m_VisibleRecipes[m_UsingIndexRecipe].m_MaximumCraft;
								}

								m_AmountInSlots[0] += m_VisibleRecipes[m_UsingIndexRecipe].m_AmountOutput * floorf(m_NumberOfRecipesDone);
								m_AddNextFrameDropItem = m_NumberOfRecipesDone - floorf(m_NumberOfRecipesDone);
								for (int i = 0; i < m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.size(); i++)
								{
									int amountLeft = floorf(m_NumberOfRecipesDone) * m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.at(i).m_Amount;
									for (int j = 1; j < 51; j++)
									{
										if (m_VisibleRecipes[m_UsingIndexRecipe].m_Ingredients.at(i).m_Item == m_PlayerSlots[j])
										{

											amountLeft -= m_AmountInSlots[j];
											if (amountLeft < 0)
											{
												m_AmountInSlots[j] = abs(amountLeft);
												break;
											}
											else if (amountLeft == 0)
											{
												m_PlayerSlots[j] = i_Nothing;
												m_AmountInSlots[j] = 0;
												break;
											}
										}
									}
								}
							}
							float oldTimer = 2 * m_TimerCrafting;
							m_TimerCrafting += deltaTime;
							m_NumberOfRecipesDone = 2 * m_TimerCrafting * deltaTime + 1.0f / 2.0f * (2 * m_TimerCrafting - oldTimer) * deltaTime + m_AddNextFrameDropItem;

						}
						m_UsingIndexRecipe += aimingSlot;
					}
				}
				else if (slotCoordinates[0] != 10)
				{
					//inventory
					if (slotCoordinates[1] < 6)
					{
						if (slotCoordinates[1] == 5)
						{
							m_AimingAtSlot = 51;
						}
						else
						{
							m_AimingAtSlot = slotCoordinates[0] + slotCoordinates[1] * 10 + 1;
						}
						slotsSwap(deltaTime, m_AmountInSlots ,m_PlayerSlots, 0);

					}
					else if (m_IndexOfOpenChest != -1)
					{
						m_AimingAtSlot = slotCoordinates[0] + (slotCoordinates[1] - 6) * 10;

						slotsSwap(deltaTime, chests.at(m_IndexOfOpenChest).m_amount, chests.at(m_IndexOfOpenChest).m_Items, -1);
						chests.at(m_IndexOfOpenChest).m_Indestrucrtible = false;
						for (int i = 0;i < 50;i++)
						{
							if (chests.at(m_IndexOfOpenChest).m_Items[i] != i_Nothing)
							{
								chests.at(m_IndexOfOpenChest).m_Indestrucrtible = true;

							}
						}
						if (m_AimingAtSlot == 0)
						{
							m_AimingAtSlot = 1;
							m_UseSlot = 1;
						}
						else if (m_AimingAtSlot == -1)
						{
							m_AimingAtSlot = 0;
						}
					}
				}
				else
				{
					m_TimerSplitingItem = 1;
					m_TimerCrafting = 1;
					m_AddNextFrameDropItem = 0;
				}
			}
			else if (slotCoordinates[0] > -1 && slotCoordinates[0] < 10 && slotCoordinates[1] == 0)
			{
				m_AimingAtSlot = slotCoordinates[0] + 1;
				if(Input::LeftMouseHold)
				{
					m_HUDUseSlot = m_AimingAtSlot;
					m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
					SwapItemStats();
				}
			}
			else
			{
				m_TimerSplitingItem = 1;
				m_TimerCrafting = 1;
				m_AddNextFrameDropItem = 0;
			}

		}
		else
		{
			m_TimerSplitingItem = 1;
			m_TimerCrafting = 1;
			m_AddNextFrameDropItem = 0;
		}

	
		for (int i = 0; i < 10; i++)
		{
			if (Input::NumberPress[i])
			{
				if (m_UseSlot != 0 && m_IsInventoryOpen)
				{
					if (m_PlayerSlots[m_UseSlot] != i_Nothing)
					{
						if (!ItermGetToInventory(m_AmountInSlots[0], m_PlayerSlots[0]))
						{
							droppedItems.emplace_back(m_Transform[0], m_Transform[1], m_DirectionLook, m_PlayerSlots[0], m_AmountInSlots[0], false);
							m_PlayerSlots[0] = i_Nothing;
							m_AmountInSlots[0] = 0;
							SwapItemStats();
						}
					}
					else
					{
						m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
						m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];

					}
					m_PlayerSlots[0] = i_Nothing;
					m_AmountInSlots[0] = 0;
					m_UseSlot = 0;
				}
				m_HUDUseSlot = i + 1;
				m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
				m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
				SwapItemStats();
			}
		}
		if (Input::EscapePress)
		{
			if (m_IsInventoryOpen)
			{
				if (m_IndexOfOpenChest != -1)
				{
					chests.at(m_IndexOfOpenChest).m_Open = false;
					m_IndexOfOpenChest = -1;
				}
				m_IsInventoryOpen = false;
				if (m_UseSlot != 0)
				{
					if (m_PlayerSlots[m_UseSlot] != i_Nothing)
					{
						ItermGetToInventory(m_AmountInSlots[0], m_PlayerSlots[0]);
					}
					else
					{
						m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
						m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];

					}
					m_PlayerSlots[0] = i_Nothing;
					m_AmountInSlots[0] = 0;
					m_UseSlot = 0;
					m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
					SwapItemStats();
				}
			}
			else
			{
				m_IsInventoryOpen = true;
			}
		}
	}
	float verticesPlayer[4] = { m_Transform[0] - 0.8 ,m_Transform[1] + 1.3 ,m_Transform[0] + 0.8,m_Transform[1] - 1.5 };
	int x = roundf(Input::XMousePos + CameraCoordinates[0]);
	int y = roundf(Input::YMousePos + CameraCoordinates[1]);

	// cursor decider and using item 
	if (!m_AimingAtSlot)
	{
		float playerVertices[4] = { verticesPlayer[0], verticesPlayer[1], verticesPlayer[2], verticesPlayer[3] };


		int blockIndex = 0;
		int craftingStationIndex = -1;
		int seedlingIndex = -1;
		int chestIndex = -1;
		int wallIndex = -1;
		m_AimingAtChest = -1;
		m_AimingAtDoors = -1;
		int woodIndex = 0;
		bool inBlock = false;

		int rangeX = x - m_Transform[0];
		int rangeY = y - m_Transform[1];
		if (m_ArmsBehaviour != ArmUsing && m_PlayerSlots[0] != i_Nothing)
		{
			if (Input::TPress && m_UseSlot == 0)
			{
				droppedItems.emplace_back(m_Transform[0], m_Transform[1], m_DirectionLook, m_PlayerSlots[0], m_AmountInSlots[0], false);
				m_PlayerSlots[0] = 0;
				m_PlayerSlots[m_HUDUseSlot] = 0;
				m_AmountInSlots[0] = 0;
				m_AmountInSlots[m_HUDUseSlot] = 0;
				SwapItemStats();
			}
			else if (Input::RightMousePress && m_UseSlot != 0 && !m_AimingAtSlot)
			{
				droppedItems.emplace_back(m_Transform[0], m_Transform[1], m_DirectionLook, m_PlayerSlots[0], m_AmountInSlots[0], false);
				m_PlayerSlots[0] = 0;
				m_AmountInSlots[0] = 0;
				SwapItemStats();
			}
		}
		m_CursorOnPlaceableForStructure = false;
		m_CursorOnPlaceableSpot = false;
		m_CursorOnMinableBlock = false;
		m_CursorOnMinableWall = false;
		m_CursorOnMinableWood = false;	
		if (CHESTANDDOORSREACH >= sqrtf(rangeX * rangeX + rangeY * rangeY))
		{
			bool found = false;
			m_AimingAtChest = FindChest(chests, x, y, found);
			if (Input::RightMousePress && found)
			{
				m_IsInventoryOpen = true;

				if (m_IndexOfOpenChest == m_AimingAtChest)
				{
					chests.at(m_IndexOfOpenChest).m_Open = false;
					m_IndexOfOpenChest = -1;
				}
				else
				{
					
					if (m_IndexOfOpenChest != -1)
					{
						chests.at(m_IndexOfOpenChest).m_Open = false;
					}
					m_IndexOfOpenChest = m_AimingAtChest;
					chests.at(m_IndexOfOpenChest).m_Open = true;

				}
			}
			else
			{
				found = false;
				m_AimingAtDoors = FindDoors(doors, x, y, found);
				if (Input::RightMousePress && found)
				{
					doors.at(m_AimingAtDoors).DoorInteract(blocks, walls, seedlings, trees, craftStations, chests, doors, isThereSandOnX, m_Transform);
				}
			}
		}
		if (m_Range >= sqrtf(rangeX * rangeX + rangeY * rangeY))
		{

			
			if (m_Placeable)
			{

				playerVertices[0] = RoundFiveUp(playerVertices[0]);
				playerVertices[1] = RoundFiveDown(playerVertices[1]);
				playerVertices[2] = RoundFiveDown(playerVertices[2]);
				playerVertices[3] = RoundFiveUp(playerVertices[3]);

				if (!(x >= playerVertices[0] && x <= playerVertices[2] && y <= playerVertices[1] && y >= playerVertices[3]))
				{
					bool inWall;
					wallIndex = FindWall(walls, x, y, inWall);
					m_CursorOnPlaceableSpot = inWall;
					inBlock = isAnythingOnThisTransform(x, y, blocks, seedlings, trees, craftStations,doors, chests);

					if (!m_CursorOnPlaceableSpot)
					{
						m_CursorOnPlaceableSpot = inBlock;
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindBlock(blocks, x + 1, y, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindBlock(blocks, x - 1, y, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindBlock(blocks, x, y + 1, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindBlock(blocks, x, y - 1, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindWall(walls, x + 1, y, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindWall(walls, x, y - 1, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindWall(walls, x - 1, y, m_CursorOnPlaceableSpot);
					}
					if (!m_CursorOnPlaceableSpot)
					{
						FindWall(walls, x, y + 1, m_CursorOnPlaceableSpot);
					}
					if (inBlock && !(m_PlayerSlots[0] >= i_WallDirt && m_PlayerSlots[0] <= i_WallIce) || inWall && m_PlayerSlots[0] >= i_WallDirt && m_PlayerSlots[0] <= i_WallIce)
					{
						m_CursorOnPlaceableSpot = false;
					}
				}
			}
			else if (m_PickaxeStreanght)
			{

				blockIndex = FindBlock(blocks, x, y, m_CursorOnMinableBlock);

				if (m_CursorOnMinableBlock && (blocks.at(x).at(blockIndex).m_BlockBehavior == b_Indestructible || blocks.at(x).at(blockIndex).m_BlockBehavior == b_Door || blocks.at(x).at(blockIndex).m_Hardness > m_PickaxeStreanght))
				{
					m_CursorOnMinableBlock = false;
				}

				if (!m_CursorOnMinableBlock)
				{
					craftingStationIndex = FindCraftStation(craftStations, x, y, m_CursorOnMinableBlock);
				}
				if (!m_CursorOnMinableBlock)
				{
					chestIndex = FindChest(chests, x, y, m_CursorOnMinableBlock);
					if (m_CursorOnMinableBlock)
					{
						if (chests.at(chestIndex).m_Indestrucrtible)
						{
							m_CursorOnMinableBlock = false;
							chestIndex = -1;
						}
					}
				}
				if (!m_CursorOnMinableBlock)
				{
					seedlingIndex = FindSeedling(seedlings, x, y, m_CursorOnMinableBlock);
				}
				
			}
			else if (m_HammerStreanght)
			{
				wallIndex = FindWall(walls, x, y, m_CursorOnMinableWall);
				if (m_CursorOnMinableBlock && walls.at(x).at(wallIndex).m_Hardness > m_HammerStreanght)
				{
					m_CursorOnMinableBlock = false;
				}
			}
			else if (m_AxeStreanght)
			{
				woodIndex = FindWood(trees, x, y, m_CursorOnMinableWood);
				if (m_CursorOnMinableWood && trees.at(woodIndex).m_Hardness > m_AxeStreanght)
				{
					m_CursorOnMinableWood = false;
				}
			}
			else if (m_LargePlaceable)
			{
				int  vertices[4];
				bool floors = true;
				getStructureVertices(x, y, GetStructureID(m_PlayerSlots[0]), vertices);
				inBlock = isAnythinginArea(vertices, blocks, seedlings, trees, craftStations,doors, chests);
				if (!inBlock)
				{
					for (int i = vertices[0]; i <= vertices[2]; i++)
					{
						floors = false;
						for (int j = 0; j < blocks.at(i).size(); j++)
						{
							if (blocks.at(i).at(j).m_Y == vertices[3] - 1)
							{
								floors = true;
								break;
							}
							if (blocks.at(i).at(j).m_Y < vertices[3] - 1)
							{
								break;
							}
						}
						if (!floors)
						{
							break;
						}
					}
				}

				if (floors)
				{

					if (!inBlock)
					{
						m_CursorOnPlaceableForStructure = true;
					}
				}
			}
		}
		else if (m_WeaponType)
		{
			unsigned char arrows[ARROWSTYPES] = { i_BasicArrow, i_PierceArrow, i_BouncingArrow, i_FireArrow };
			unsigned char cannonBalls[CANNONBALLSTYPES] = { i_BasicCannonBall, i_PierceCannonBall, i_BouncingCannonBall, i_FireCannonBall, i_Sand };
			unsigned char bullets[BULLETSTYPES] = { i_BasicBullet, i_PierceBullet, i_BouncingBullet, i_FireBullet };
			switch (m_WeaponType)
			{
			case weaponBow:
			case weaponBow + weaponAutomatic:
				m_LocationAmmunition = FindOneOfItemsInInv(arrows, ARROWSTYPES);
				break;
			case weaponCanon:
			case weaponCanon + weaponAutomatic:
				m_LocationAmmunition = FindOneOfItemsInInv(cannonBalls, CANNONBALLSTYPES);
				break;
				break;
			case weaponGun:
			case weaponGun + weaponAutomatic:
				m_LocationAmmunition = FindOneOfItemsInInv(bullets, BULLETSTYPES);
				break;
			}

		}
		if (m_UseItemTimer > m_CooldownToUse)
		{
			if (m_WeaponType > weaponAutomatic && Input::LeftMouseHold || m_WeaponType < weaponAutomatic && m_WeaponType > weaponNot && Input::LeftMouseRelease)
			{
				if (m_LocationAmmunition != -1)
				{
					float velocity[2] = { Input::XMousePos - PLAYERHANDOFFSETX * m_DirectionLook - (m_Transform[0] - CameraCoordinates[0]), Input::YMousePos - PLAYERHANDOFFSETY - (m_Transform[1] - CameraCoordinates[1]) };
					NormalizeVector(velocity);
					switch (m_PlayerSlots[0])
					{
					case weaponMelee:
						m_ArmsBehaviour = ArmUsing;
						break;
					case i_WoodBow:

						projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY, velocity[0] * 25, velocity[1] * 25, m_Damage, blockDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);

						break;
					case i_Cannon:


						projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY, velocity[0] * 20, velocity[1] * 20, m_Damage, blockDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);
						break;
					case i_Pistol:


						projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY, velocity[0] * 30, velocity[1] * 30, m_Damage, m_BulletsDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);
						break;
					default:
						std::cout << "Error player.cpp Dont know this Weapon: " << m_PlayerSlots[0] << std::endl;
						break;
					}
					m_AmountInSlots[m_LocationAmmunition]--;
					if (m_AmountInSlots[m_LocationAmmunition] <= 0)
					{
						m_PlayerSlots[m_LocationAmmunition] = i_Nothing;
						m_LocationAmmunition = -1;
					}
					m_UseItemTimer = 0;
				}


			}
			else if (Input::LeftMouseHold && m_WeaponType == weaponNot)
			{



				switch (m_PlayerSlots[0])
				{
				case i_Nothing:
					break;
				default:
					if (!m_Consume)
					{
						m_ArmsBehaviour = ArmUsing;
					}
					break;
				}

				if (m_CursorOnMinableBlock)
				{
					if (blockIndex != -1)
					{
						bool damaged = false;
						int damageIndex;
						for (damageIndex = 0; damageIndex < damageblocks.size(); damageIndex++)
						{
							if (damageblocks.at(damageIndex).m_Transform[0] == x && damageblocks.at(damageIndex).m_Transform[1] == y)
							{
								damaged = true;
								break;
							}
						}
						if (damaged)
						{
							damageblocks.at(damageIndex).m_HP -= floorf((float)m_PickaxeStreanght / (float)blocks.at(x).at(blockIndex).m_Hardness);
							if (0 >= damageblocks.at(damageIndex).m_HP)
							{
								droppedItems.emplace_back(x, blocks.at(x).at(blockIndex).m_Y, 0, blocks.at(x).at(blockIndex).m_ItemDrop, 1, true);
								damageblocks.erase(damageblocks.begin() + damageIndex);
								DestroyBlock(blocks, walls, isThereSandOnX, x, y);
							}
						}
						else if (0 >= ((float)blocks.at(x).at(blockIndex).m_Hardness) - ((float)m_PickaxeStreanght / 3.0f))
						{
							droppedItems.emplace_back(x, blocks.at(x).at(blockIndex).m_Y, 0, blocks.at(x).at(blockIndex).m_ItemDrop, 1, true);
							DestroyBlock(blocks, walls, isThereSandOnX, x, y);
						}
						else
						{
							damageblocks.emplace_back(x, blocks.at(x).at(blockIndex).m_Y, ceilf(3.0f - ((float)m_PickaxeStreanght / (float)blocks.at(x).at(blockIndex).m_Hardness)));
						}
					}
					else if (craftingStationIndex != -1)
					{

						droppedItems.emplace_back(x, y, 0, GetItemIDByStructure(craftStations.at(craftingStationIndex).m_CraftStationtype), 1, true);

						craftStations.erase(craftStations.begin() + craftingStationIndex);
					}
					else if (chestIndex != -1)
					{
						if (m_IndexOfOpenChest != -1)
						{
							chests.at(m_IndexOfOpenChest).m_Open = false;
							m_IndexOfOpenChest = -1;
						}

						chests.at(chestIndex).DestroyChest(blocks);
						droppedItems.emplace_back(x, y, 0, i_Chest, 1, true);
						chests.erase(chests.begin() + chestIndex);
					}
					else if (seedlingIndex != -1)
					{
						droppedItems.emplace_back(x, y, 0, i_Sapling, 1, true);
						seedlings.erase(seedlingIndex + seedlings.begin());
					}

				}
				else if (m_CursorOnPlaceableSpot)
				{
					if (m_PlayerSlots[0] >= i_WallDirt && m_PlayerSlots[0] <= i_WallIce)
					{
						createWall(x, y, m_PlayerSlots[0], walls, blocks, texturesIDs);
					}
					else
					{
						CreateBlock(x, y, m_PlayerSlots[0], walls, blocks, isThereSandOnX, texturesIDs);
					}
					m_AmountInSlots[0]--;
					if (m_UseSlot == 0)
					{
						m_AmountInSlots[m_HUDUseSlot]--;
					}
					if (m_AmountInSlots[0] <= 0)
					{
						if (m_UseSlot == 0)
						{
							m_PlayerSlots[m_HUDUseSlot] = i_Nothing;
						}
					}
				}
				else if (m_CursorOnMinableWall)
				{
					bool damaged = false;
					int damageIndex;
					for (damageIndex = 0; damageIndex < damagedWalls.size(); damageIndex++)
					{
						if (damagedWalls.at(damageIndex).m_Transform[0] == x && damagedWalls.at(damageIndex).m_Transform[1] == y)
						{
							damaged = true;
							break;
						}
					}
					if (damaged)
					{
						damagedWalls.at(damageIndex).m_HP -= floorf((float)m_HammerStreanght / (float)walls.at(x).at(wallIndex).m_Hardness);
						if (0 >= damagedWalls.at(damageIndex).m_HP)
						{
							if (walls.at(x).at(wallIndex).m_ItemDrop != i_Nothing)
							{
								droppedItems.emplace_back(x, walls.at(x).at(wallIndex).m_Y, 0, walls.at(x).at(wallIndex).m_ItemDrop, 1, true);
							}
							damagedWalls.erase(damagedWalls.begin() + damageIndex);
							walls.at(x).erase(walls.at(x).begin() + wallIndex);
						}
					}
					else if (0 >= ((float)walls.at(x).at(wallIndex).m_Hardness) - ((float)m_HammerStreanght / 3.0f))
					{
						if (walls.at(x).at(wallIndex).m_ItemDrop != i_Nothing)
						{
							droppedItems.emplace_back(x, walls.at(x).at(wallIndex).m_Y, 0, walls.at(x).at(wallIndex).m_ItemDrop, 1, true);
						}
						walls.at(x).erase(walls.at(x).begin() + wallIndex);
					}
					else
					{

						damagedWalls.emplace_back(x, walls.at(x).at(wallIndex).m_Y, ceilf(3.0f - ((float)m_HammerStreanght / (float)walls.at(x).at(wallIndex).m_Hardness)));
					}

				}
				else if (m_CursorOnMinableWood)
				{
					bool damaged = false;
					int damageIndex;
					for (damageIndex = 0; damageIndex < damagedWoods.size(); damageIndex++)
					{
						if (damagedWoods.at(damageIndex).m_Transform[0] == x && damagedWoods.at(damageIndex).m_Transform[1] == y)
						{
							damaged = true;
							break;
						}
					}
					if (damaged)
					{
						damagedWoods.at(damageIndex).m_HP -= floorf((float)m_AxeStreanght / (float)trees.at(woodIndex).m_Hardness);
						if (0 >= damagedWoods.at(damageIndex).m_HP)
						{
							for (int i = 0; i < blocks.at(x).size(); i++)
							{
								if (blocks.at(x).at(i).m_Y == y - 1)
								{
									blocks.at(x).at(i).m_BlockBehavior = getBehaviorByTexture(blocks.at(x).at(i).m_te);
								}
							}
							while (woodIndex != -1)
							{
								int specialIndex = -1;
								int destroy[6] = { -1, -1, -1, -1, -1 , -1 };
								for (int i = 0; i < trees.size(); i++)
								{
									if (trees.at(woodIndex).m_Transform[0] == trees.at(i).m_Transform[0] && trees.at(woodIndex).m_Transform[1] + 1 == trees.at(i).m_Transform[1])
									{
										specialIndex = i;
									}
									else if (trees.at(woodIndex).m_Transform[1] == trees.at(i).m_Transform[1] && trees.at(i).m_Transform[0] <= trees.at(woodIndex).m_Transform[0] + 2 && trees.at(i).m_Transform[0] >= trees.at(woodIndex).m_Transform[0] - 2 && trees.at(i).m_Transform[0] != trees.at(woodIndex).m_Transform[0])
									{
										int j;
										for (j = 0; destroy[j] != -1; j++) {}
										destroy[j] = i;
									}
								}
								{
									int j;
									for (j = 0; destroy[j] != -1; j++) {}
									destroy[j] = woodIndex;
									for (int i = 0; i < 5; i++)
									{
										for (int j = 1; j < 5; j++)
										{
											if (destroy[j - 1] < destroy[j])
											{
												int holder = destroy[j - 1];
												destroy[j - 1] = destroy[j];
												destroy[j] = holder;
											}
										}
									}
								}
								for (int i = 0; i < 5; i++)
								{
									if (destroy[i] < specialIndex && destroy[i] != -1)
									{
										specialIndex--;
									}
								}
								woodIndex = specialIndex;
								for (int j = 0; destroy[j] != -1; j++)
								{
									if (trees.at(destroy[j]).m_ItemDrop != i_Nothing)
									{
										droppedItems.emplace_back(trees.at(destroy[j]).m_Transform[0], trees.at(destroy[j]).m_Transform[1], 0, trees.at(destroy[j]).m_ItemDrop, 1, true);

									}
									trees.erase(trees.begin() + destroy[j]);
								}
							}
							damagedWoods.erase(damagedWoods.begin() + damageIndex);

						}
					}
					else if (0 >= ((float)trees.at(woodIndex).m_Hardness) - ((float)m_AxeStreanght / 12.0f))
					{
						while (woodIndex != -1)
						{
							int specialIndex = -1;
							int destroy[5] = { -1, -1, -1, -1, -1 };
							for (int i = 0; i < trees.size(); i++)
							{
								if (trees.at(woodIndex).m_Transform[0] == trees.at(i).m_Transform[0] && trees.at(woodIndex).m_Transform[1] + 1 == trees.at(i).m_Transform[1])
								{
									specialIndex = i;
								}
								else if (trees.at(woodIndex).m_Transform[1] == trees.at(i).m_Transform[1] && trees.at(i).m_Transform[0] <= trees.at(woodIndex).m_Transform[0] + 2 && trees.at(i).m_Transform[0] >= trees.at(woodIndex).m_Transform[0] - 2 && trees.at(i).m_Transform[0] != trees.at(woodIndex).m_Transform[0])
								{
									int j;
									for (j = 0; destroy[j] != -1; j++) {}
									destroy[j] = i;
								}
							}
							{
								int j;
								for (j = 0; destroy[j] != -1; j++) {}
								destroy[j] = woodIndex;
								for (int i = 0; i < 5; i++)
								{
									for (int j = 1; j < 5; j++)
									{
										if (destroy[j - 1] < destroy[j])
										{
											int holder = destroy[j - 1];
											destroy[j - 1] = destroy[j];
											destroy[j] = holder;
										}
									}
								}
							}
							for (int i = 0; i < 5; i++)
							{
								if (destroy[i] < specialIndex && destroy[i] != -1)
								{
									specialIndex--;
								}
							}
							woodIndex = specialIndex;
							for (int j = 0; destroy[j] != -1; j++)
							{
								if (trees.at(destroy[j]).m_ItemDrop != i_Nothing)
								{
									droppedItems.emplace_back(trees.at(destroy[j]).m_Transform[0], trees.at(destroy[j]).m_Transform[1], 0, trees.at(destroy[j]).m_ItemDrop, 1, true);

								}
								trees.erase(trees.begin() + destroy[j]);
							}
						}

					}
					else
					{

						damagedWoods.emplace_back(x, trees.at(woodIndex).m_Transform[1], trees.at(woodIndex).m_Rotation, ceilf(12.0f - ((float)m_AxeStreanght / (float)trees.at(woodIndex).m_Hardness)));
					}

				}
				else if (m_CursorOnPlaceableForStructure)
				{
					switch (m_PlayerSlots[0])
					{
					case i_Sapling:
						seedlings.emplace_back(s_Sapling, x, y, structuresTextures, blocks);
						break;
					case i_CraftingTable:
					{
						CraftStation table;
						table.m_CraftStationtype = s_CraftingTable;
						table.m_Transform[0] = x;
						table.m_Transform[1] = y;
						table.m_LookAt = m_DirectionLook;
						craftStations.emplace_back(table);
						break;
					}
					case i_Forge:
					{
						CraftStation forge;
						forge.m_CraftStationtype = s_Forge;
						forge.m_Transform[0] = x;
						forge.m_Transform[1] = y;
						forge.m_LookAt = m_DirectionLook;
						craftStations.emplace_back(forge);
						break;
					}
					case i_Anvil:
					{
						CraftStation anvil;
						anvil.m_CraftStationtype = s_Anvil;
						anvil.m_Transform[0] = x;
						anvil.m_Transform[1] = y;
						anvil.m_LookAt = m_DirectionLook;
						craftStations.emplace_back(anvil);
						break;
					}
					case i_Chest:
					{
						chests.emplace_back(x, y,blocks);
						

						break;
					}
					}
					m_AmountInSlots[0]--;
					if (m_UseSlot == 0)
					{
						m_AmountInSlots[m_HUDUseSlot]--;
					}
					if (m_AmountInSlots[0] <= 0)
					{
						if (m_UseSlot == 0)
						{
							m_PlayerSlots[m_HUDUseSlot] = i_Nothing;
						}
					}
				}
				else if (m_Consume)
				{
					switch (m_PlayerSlots[0])
					{
					case i_WoodHelmet:
						if (m_UseSlot == 0)
						{
							SwapArmor(m_HUDUseSlot, armorHelmet);
						}
						else
						{
							SwapArmor(0, armorHelmet);
						}
						break;
					case i_WoodChestPlate:
						if (m_UseSlot == 0)
						{
							SwapArmor(m_HUDUseSlot, armorChestPlate);
						}
						else
						{
							SwapArmor(0, armorChestPlate);
						}
						break;
					case i_WoodPants:
						if (m_UseSlot == 0)
						{
							SwapArmor(m_HUDUseSlot, armorPants);
						}
						else
						{
							SwapArmor(0, armorPants);
						}
						break;
					case i_WoodShoes:
						if (m_UseSlot == 0)
						{
							SwapArmor(m_HUDUseSlot, armorShoes);
						}
						else
						{
							SwapArmor(0, armorShoes);
						}
						break;
					default:
						std::cout << "Error Player.cpp Unknow consumable " << m_PlayerSlots[0] << std::endl;
						break;
					}
					SwapItemStats();
				}
				m_UseItemTimer = 0;


			}
		}


		if (m_UseItemTimer < 5)
		{
			m_UseItemTimer += deltaTime;
		}
	}

	// moving player 
	m_CoyoteTimer += deltaTime;
	{
		
		if (m_ArmsBehaviour != ArmUsing)
		{
			if (Input::DHold)
			{
				m_DirectionLook = 1;
			}
			if (Input::AHold)
			{
				m_DirectionLook = -1;
			}
		}
		
		if (Input::DHold && m_Velocity[0] <= m_MaxMovementSpeed)
		{
			m_Velocity[0] += m_Acceleration * deltaTime;
		}
		else if (m_Velocity[0] > 0)
		{
			float velocity = m_Velocity[0] - m_Friction * deltaTime;
			if (velocity < 0)
			{
				m_Velocity[0] = 0;
			}
			else
			{
				m_Velocity[0] = velocity;
			}
		}
		if (Input::AHold && m_Velocity[0] >= -m_MaxMovementSpeed)
		{
			m_Velocity[0] += -m_Acceleration * deltaTime;
		}
		else if (m_Velocity[0] < 0)
		{
			float velocity = m_Velocity[0] + m_Friction * deltaTime;
			if (velocity > 0)
			{
				m_Velocity[0] = 0;
			}
			else
			{
				m_Velocity[0] = velocity;
			}
		}

		if (Input::SpacePress && m_CanJump)
		{
			m_Velocity[1] += m_JumpPower;
			m_CanJump = false;
			m_JumpTimer += deltaTime;
		}
		else if (Input::SpaceHold && m_JumpTimer > 0 && m_JumpTimer < 0.3f)
		{
			m_Velocity[1] -= (m_JumpPower / 2) * deltaTime;
			m_JumpTimer += deltaTime;
		}
		else
		{
			m_Velocity[1] += m_Gravity * deltaTime;
			if (-30 > m_Velocity[1])
			{
				m_Velocity[1] = -30;
			}
			m_JumpTimer = 0;
		}

		m_FloorHit = false;
		m_CeilHit = false;
		m_LeftWallHit = false;
		m_RightWallHit = false;
	
		m_FloorBehaviour = DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, oldVelocity, verticesPlayer, blocks, m_LeftWallHit, m_RightWallHit, m_FloorHit, m_CeilHit);
		if (AddVelocityToTransform(verticesPlayer, m_Transform, m_Velocity, oldVelocity, m_FloorHit, m_RightWallHit, m_LeftWallHit, m_CeilHit, deltaTime))
		{
			m_FloorBehaviour = b_BasicSolid;
		}
		
		if (m_FloorHit)
		{
			if (m_LastStandingY - m_Transform[1] > 20)
			{
				DamagePlayer(NULL, m_LastStandingY - m_Transform[1] - 20);
			}
			m_LastStandingY = m_Transform[1];
		}
		if ((m_LeftWallHit && Input::AHold  || m_RightWallHit && Input::DHold) && m_Velocity[1] < 0 && m_Effects[effectWallClimb])
		{
			m_FloorHit = true;
			m_Velocity[1] = 0;
			m_FloorBehaviour = b_Asphalt;
			m_LastStandingY = m_Transform[1];
		}

		switch (m_FloorBehaviour)
		{
		case(b_Air):
			m_Acceleration = 5.0f;
			m_Friction = 4;
			m_MaxMovementSpeed = 12 * m_SpeedMultiplier;
			break;
		case(b_Slippery):
			m_Acceleration = 12.0f;
			m_Friction = 8;
			m_MaxMovementSpeed = 15 * m_SpeedMultiplier;
			break;
		case(b_Asphalt):
			m_Acceleration = 30.0f;
			m_Friction = 70;
			m_MaxMovementSpeed = 25 * m_SpeedMultiplier;
			break;
		case(b_BasicSolid):
		case(b_Platform):
		case(b_Sand):
			m_Acceleration = 25.0f;
			m_Friction = 40;
			m_MaxMovementSpeed = 10 * m_SpeedMultiplier;
			break;
		}
		if (m_FloorHit)
		{
			m_CanJump = true;
			m_CoyoteTimer = 0.0f;
		}
		if (!m_FloorHit && m_CanJump && m_CoyoteTimer >= 0.1f)
		{
			m_CanJump = false;
			m_CoyoteTimer = 0.0f;
		}
	}

	//item receive 
	for (int i = 0; i < droppedItems.size(); i++)
	{
		if (droppedItems.at(i).EveryFrame(deltaTime, blocks, droppedItems, m_Transform, HavePlayerSpace(droppedItems.at(i).m_Item)))
		{
			unsigned short int itemSwapCheck;
			if (m_UseSlot == 0)
			{
				itemSwapCheck = m_PlayerSlots[m_HUDUseSlot];
			}
			if (ItermGetToInventory(droppedItems.at(i).m_Amount, droppedItems.at(i).m_Item))
			{
				if (m_UseSlot == 0 && itemSwapCheck != m_PlayerSlots[m_HUDUseSlot])
				{
					m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
					SwapItemStats();
				}
				droppedItems.erase(droppedItems.begin() + i);
			}
		}
	}

	//player anim
	{
		if (m_WeaponType > weaponNot)
		{
			if (Input::LeftMouseHold && !m_AimingAtSlot )
			{
				m_ArmsBehaviour = ArmUsing;
			}
			else if(m_ArmsBehaviour == ArmUsing)
			{
				m_ArmsBehaviour = ArmStanding;
			}
		}
		if (m_FloorHit)
		{
			m_WalkingTimer += deltaTime;

			if (m_Velocity[0] != 0)
			{
				if (m_ArmTimer > 0.1f && m_ArmsBehaviour == ArmStanding)
				{
					m_ArmsBehaviour = ArmRun;
					m_ArmTimer = 0;
				}
				else if (m_ArmsBehaviour == ArmStanding)
				{
					m_ArmTimer += deltaTime;
				}
				if (0.1f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 2;
					}
				}
				else if (SPEEDOFANIM / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 3;
					}
				}
				else if (SPEEDOFANIM * 2 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 1;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 3;
					}
				}
				else if (SPEEDOFANIM * 3 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 2;
					}
				}
				else if (SPEEDOFANIM * 4 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 2;
					}
				}
				else if (SPEEDOFANIM * 5 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 4;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 1;
					}
				}
				else if (SPEEDOFANIM * 6 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 2;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 1;
					}
				}
				else if (SPEEDOFANIM * 7 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 4;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 2;
					}
				}
				else if (SPEEDOFANIM * 8 / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
					if (m_ArmsBehaviour == ArmRun)
					{
						m_ArmPhase = 2;
					}
				}
				else
				{
					m_WalkingTimer = 0;
				}
			}
			else
			{
				if (m_ArmTimer > 0.5 && m_ArmsBehaviour == ArmRun)
				{
					m_ArmsBehaviour = ArmStanding;
					m_ArmPhase = 0;
					m_ArmTimer = 0;
				}
				else if(m_ArmsBehaviour == ArmRun)
				{
					m_ArmTimer += deltaTime;
				}
				m_WalkingPhase = 0;
			}
		}
		else
		{
			m_WalkingPhase = 1;
		}
		if (m_ArmsBehaviour == ArmUsing)
		{
			switch (m_PlayerSlots[0])
			{

			case i_WoodBow:
			case i_Cannon:
			case i_Pistol:
				m_ArmRotation = atan2f(Input::XMousePos - PLAYERHANDOFFSETX * m_DirectionLook-(m_Transform[0] - CameraCoordinates[0]), Input::YMousePos - PLAYERHANDOFFSETY - (m_Transform[1] - CameraCoordinates[1])) * 180.0 / PI;
				if (m_ArmRotation)
				{
					m_DirectionLook = m_ArmRotation / abs(m_ArmRotation);
				}
				m_ArmRotation = -abs(m_ArmRotation);
				break;
			default:
				m_ArmRotation -= 150 * deltaTime / m_CooldownToUse;
				if (!m_Placeable && !m_LargePlaceable)
				{
					float pointOfRotation[2] = { PLAYERHANDOFFSETX * m_DirectionLook + m_Transform[0], PLAYERHANDOFFSETY + m_Transform[1] };
					for (int i = 0; i < enemies.size(); i++)
					{
						if ((enemies.at(i).m_Transform[0] - m_Transform[0])*m_DirectionLook >= 0)
						{
							float distance[2] = { enemies.at(i).m_Transform[0] - pointOfRotation[0], enemies.at(i).m_Transform[1] - pointOfRotation[1] };
							float enemyVertices[4];

							GetEnemyVerticesByType(enemies.at(i).m_TypeOfEnemy, enemyVertices);
							float distanceVertices[4] = { distance[0] - enemyVertices[0], distance[1] - enemyVertices[1] , distance[0] - enemyVertices[2] , distance[1] - enemyVertices[2] };
							for (int j = 0; j < 2; j++)
							{
								float holder[2];
								holder[1] = distanceVertices[1 + 2 * j];
								for (int l = 0; l < 2; l++)
								{
									holder[0] = distanceVertices[2 * l];
									if (Pyt2D(holder) < Pyt2D(distance))
									{
										distance[0] = holder[0];
										distance[1] = holder[1];
									}
								}
							}

							if (Pyt2D(distance) <= 2.5f)
							{
								if (abs(atan2(distance[0], distance[1])) <= -m_ArmRotation)
								{
									bool wasEnemyHit = false;
									for (int j = 0; j < m_HitEnemies.size(); j++)
									{
										if (enemies.at(i).m_ID == m_HitEnemies.at(j))
										{
											wasEnemyHit = true;
										}
									}
									if (!wasEnemyHit)
									{
										if (enemies.at(i).DamageEnemy(m_Damage, m_Transform))
										{
											enemies.erase(enemies.begin() + i);
											i--;
										}
										else
										{
											m_HitEnemies.push_back(enemies.at(i).m_ID);
										}
									}
								}

							}

						}

					}
				}
				if (m_UseItemTimer > m_CooldownToUse)
				{
					if (m_AmountInSlots[0] == 0)
					{
						m_PlayerSlots[0] = i_Nothing;
						SwapItemStats();
					}
					m_ArmsBehaviour = ArmStanding;
					m_HitEnemies.clear();
					m_ArmRotation = 0;
				}
				break;
			}
		}
	}
		
			

	//health
	int playerVertices[4] = { RoundFiveUp(verticesPlayer[0]), RoundFiveDown(verticesPlayer[1]), RoundFiveDown(verticesPlayer[2]), RoundFiveUp(verticesPlayer[3]) };
	{
		bool IsPlayerInBlock = false; 
		for (int i = playerVertices[0] ; i < playerVertices[2]; i++)
		{
			for (int j = 0; j < blocks.at(i).size(); j++)
			{
				if (playerVertices[3] > blocks.at(i).at(j).m_Y)
				{
					break;
				}
				else if (playerVertices[1] > blocks.at(i).at(j).m_Y && blocks.at(i).at(j).m_BlockBehavior != b_Platform)
				{
					IsPlayerInBlock = true;
					break;
				}
			}
		}
		m_DamageTimer += deltaTime;
		if (m_DamageTimer > 0.5f)
		{
			m_DamageTimer = 0;
			if (IsPlayerInBlock)
			{
				DamagePlayer(NULL, 5);
			}
		}
		if (m_TimerSinceLastHit >= REGENCOLDDOWN)
		{
			m_TimerSinceLastHit = REGENCOLDDOWN;
			m_CurrentHealth += floorf(deltaTime * m_HPRegen + m_AddNextFrameHP);
			m_AddNextFrameHP = deltaTime * m_HPRegen + m_AddNextFrameHP - floorf(deltaTime * m_HPRegen + m_AddNextFrameHP);
			if (m_CurrentHealth > m_maxHealth)
			{
				m_CurrentHealth = m_maxHealth;
			}
		}
		else
		{
			m_TimerSinceLastHit += deltaTime;
		}
		
	}

}
void Player::DrawPlayer(float deltaTime
	, Shader& basicSh
	, Shader& HUDSh
	, Shader& fontSh
	, Shader& animSh
	, Shader& handSh
	, Shader& particlesSh
	, std::vector<Chest>& chests
	, float* transform
	, float* scale
	, float* rotation
	, unsigned int fontDD
	, unsigned int particlesDD
	, unsigned int numberTexture)
{
	{
		animSh.Bind();
		ChangeTransform(m_Transform[0], m_Transform[1], transform);
		animSh.SetUniformMat4(animTransform, transform);
		ChangeScale(m_DirectionLook, 1, scale);
		animSh.SetUniformMat4(animScale, scale);
		animSh.SetUniform1i(animLeangth, 5);
		animSh.SetUniform1i(animNumber, m_WalkingPhase);
		ErrorGL(glBindVertexArray(m_BottomAnimDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_BootsAnimTex));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_LegAnimTex));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (m_WalkingPhase == 0)
		{
			ChangeTransform(m_Transform[0], m_Transform[1] + 0.1f, transform);
			animSh.SetUniformMat4(animTransform, transform);
		}
		animSh.SetUniform1i(animNumber, 0);
		ErrorGL(glBindVertexArray(m_HeadDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HeadTex));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		if (m_ArmsBehaviour == ArmUsing)
		{
			animSh.SetUniform1i(animNumber, 4);
			ErrorGL(glBindVertexArray(m_BodyAnimDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_BodyAnimTex));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			animSh.SetUniform1i(animNumber, 0);

			handSh.Bind();
			handSh.SetUniformMat4(handTransform, transform);
			handSh.SetUniformMat4(handScale, scale);
			ChangeRotation(m_ArmRotation, rotation);
			handSh.SetUniformMat4(handRotation, rotation);
			
			switch (m_PlayerSlots[0])
			{

			case i_WoodBow:
				ErrorGL(glBindVertexArray(m_ItemsInHandDD[InHandBow]));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ItemsInHandTexture[InHandBow]));
				break;
			case i_Cannon:
				ErrorGL(glBindVertexArray(m_ItemsInHandDD[InHandCanon]));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ItemsInHandTexture[InHandCanon]));
				break;
			case i_Pistol:
				ErrorGL(glBindVertexArray(m_ItemsInHandDD[InHandPistol]));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ItemsInHandTexture[InHandPistol]));
				break;

			default:
				if (m_Placeable)
				{
					ErrorGL(glBindVertexArray(m_BlockInHandDD));
				}
				else
				{
					ErrorGL(glBindVertexArray(m_ItemInHandDD));
				}
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[0]]));
				break;
			}
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			ErrorGL(glBindVertexArray(m_HandDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HandTex));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		else
		{
			animSh.SetUniform1i(animNumber, m_ArmPhase);
			ErrorGL(glBindVertexArray(m_BodyAnimDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_BodyAnimTex));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}

	}
	particlesSh.Bind();
	ErrorGL(glBindVertexArray(particlesDD));
	if (m_IsBurning)
	{
		m_BurningTimer += deltaTime;
		if (m_BurningTimer < TIMEONFIRE)
		{
			if (m_OnFire.DrawParticles(particlesSh, deltaTime,true, m_Transform, transform))
			{
				m_BurnDamageNextTime++;
				if (m_BurnDamageNextTime >= 4)
				{
					m_BurnDamageNextTime = 0;
					DamagePlayer(NULL, 2);
				}
			}
		}
		else
		{
			m_BurningTimer = 0;
			m_BurnDamageNextTime = 0;
			m_IsBurning = false;
		}
	}
	else
	{
		m_OnFire.DrawParticles(particlesSh, deltaTime, false, m_Transform, transform);
	}
	
	HUDSh.Bind();
	ErrorGL(glBindVertexArray(m_HUDDD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
	ChangeTransform(m_InvOffset[0], m_InvOffset[1], transform);
	HUDSh.SetUniformMat4(HUDBasicLocation, transform);
	ChangeScale(1, 1, scale);
	HUDSh.SetUniformMat4(HUDScale, scale);
	HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);
	if (m_IsInventoryOpen)
	{

		for (int i = 0; i < 5; i++)
		{

			for (int j = 0; j < 10; j++)
			{
				if (i == 0 && j + 1 == m_HUDUseSlot && m_UseSlot == 0)
				{
					ChangeScale(1.2f, 1.2f, scale);
					HUDSh.SetUniformMat4(HUDScale, scale);
					ChangeTransform(j * m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(HUDTransform, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
					ChangeScale(1, 1, scale);
					HUDSh.SetUniformMat4(HUDScale, scale);
				}
				else
				{
					ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4(HUDTransform, transform);
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}

			}

			ChangeScale(0.8f, 0.8f, scale);
			HUDSh.SetUniformMat4(HUDScale, scale);


			for (int j = 0; j < 10; j++)
			{

				if (m_PlayerSlots[(i * 10) + (j + 1)] != i_Nothing)
				{
					if (m_PlayerSlots[(i * 10) + (j + 1)] >= i_WallDirt && m_PlayerSlots[(i * 10) + (j + 1)] <= i_WallIce)
					{
						HUDSh.SetUniform1i(HUDSize + HUDShadow, 1);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(HUDTransform, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

					}
					else
					{
						HUDSh.SetUniform1i(HUDSize + HUDShadow, 0);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(HUDTransform, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					}
				}
			}
			HUDSh.SetUniform1i(HUDSize + HUDShadow, 0);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
			ChangeScale(1, 1, scale);
			HUDSh.SetUniformMat4(HUDScale, scale);
		}


		if (m_IndexOfOpenChest != -1)
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ChestSlotTexture));
			for (int i = 6; i < 11; i++)
			{

				for (int j = 0; j < 10; j++)
				{
					ChangeTransform(j* m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4(HUDTransform, transform);
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

				}
				ChangeScale(0.8f, 0.8f, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);

				for (int j = 0; j < 10; j++)
				{

					if (chests.at(m_IndexOfOpenChest).m_Items[((i - 6) * 10) + j ] != i_Nothing)
					{
						if (chests.at(m_IndexOfOpenChest).m_Items[((i-6) * 10) + (j + 1)] >= i_WallDirt && chests.at(m_IndexOfOpenChest).m_Items[(i * 10) + j ] <= i_WallIce)
						{
							HUDSh.SetUniform1i(HUDSize + HUDShadow, 1);
							ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
							HUDSh.SetUniformMat4(HUDTransform, transform);
							ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[chests.at(m_IndexOfOpenChest).m_Items[((i - 6) * 10) + j ]]));
							ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

						}
						else
						{
							HUDSh.SetUniform1i(HUDSize + HUDShadow, 0);
							ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
							HUDSh.SetUniformMat4(HUDTransform, transform);
							ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[chests.at(m_IndexOfOpenChest).m_Items[((i - 6) * 10) + j ]]));
							ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
						}
					}
				}
				HUDSh.SetUniform1i(HUDSize + HUDShadow, 0);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ChestSlotTexture));
				ChangeScale(1, 1, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);

			}
		}


		for (int i = 0; i < m_NumberOfVisibleRecipes; i++)
		{
			if (!(i - m_RecipeY > 3 || i - m_RecipeY < -3))
			{

				float x = 10 * m_SlotGap + 2 * (m_SlotGap - 2 * m_HalfOfSlotLeanght);
				float y = m_RecipeY * m_SlotGap - m_SlotGap * i - 2 * m_SlotGap;
				HUDSh.SetUniform1f(HUDSize + HUDCraftingY, i - m_RecipeY);


				ChangeTransform(x, y, transform);
				HUDSh.SetUniformMat4(HUDTransform, transform);
				if (m_UsingIndexRecipe == i)
				{
					ChangeScale(1.2f, 1.2f, scale);
					HUDSh.SetUniformMat4(HUDScale, scale);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					ChangeScale(1, 1, scale);
					HUDSh.SetUniformMat4(HUDScale, scale);
					for (int j = 0; j < m_VisibleRecipes[i].m_Ingredients.size(); j++)
					{
						ChangeScale(0.8f, 0.8f, scale);
						HUDSh.SetUniformMat4(HUDScale, scale);
						ChangeTransform(x + (j + 1) * m_SlotGap, y, transform);
						HUDSh.SetUniformMat4(HUDTransform, transform);

						if (m_VisibleRecipes[i].m_Ingredients.at(j).m_CraftingState == ReadyToCraft)
						{
							ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));

						}
						else if (m_VisibleRecipes[i].m_Ingredients.at(j).m_CraftingState == missingToCraft)
						{
							ErrorGL(glBindTexture(GL_TEXTURE_2D, m_MissingSlotTexture));
						}
						else
						{
							ErrorGL(glBindTexture(GL_TEXTURE_2D, m_NothingSlotTexture));
						}

						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

						ChangeScale(0.6f, 0.6f, scale);
						HUDSh.SetUniformMat4(HUDScale, scale);


						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_VisibleRecipes[i].m_Ingredients.at(j).m_Item]));

						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));



					}
					ChangeScale(1, 1, scale);
					HUDSh.SetUniformMat4(HUDScale, scale);
					ChangeTransform(x, y, transform);
					HUDSh.SetUniformMat4(HUDTransform, transform);

				}
				if (m_VisibleRecipes[i].m_CraftingState == ReadyToCraft)
				{
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));

				}
				else
				{
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_MissingSlotTexture));
				}


				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));


				ChangeScale(0.8f, 0.8f, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);


				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_VisibleRecipes[i].m_ItemOutput]));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

				ChangeScale(1, 1, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);

			}
		}
		HUDSh.SetUniform1f(HUDSize + HUDCraftingY, 0);

		ChangeTransform(9 * m_SlotGap, -5 * m_SlotGap, transform);
		HUDSh.SetUniformMat4(HUDTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_TrashCanSlotTexture));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (m_PlayerSlots[51] != i_Nothing)
		{
			ChangeScale(0.8f, 0.8f, scale);
			HUDSh.SetUniformMat4(HUDScale, scale);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[51]]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		fontSh.Bind();
		ErrorGL(glBindVertexArray(fontDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));

		for (int i = 0; i < m_NumberOfVisibleRecipes; i++)
		{
			if (!(i - m_RecipeY > 3 || i - m_RecipeY < -3))
			{
				float x = 10 * m_SlotGap + 2 * (m_SlotGap - 2 * m_HalfOfSlotLeanght) + m_InvOffset[0];
				float y = m_RecipeY * m_SlotGap - m_SlotGap * i - 2 * m_SlotGap + m_InvOffset[1] - m_HalfOfSlotLeanght;
				fontSh.SetUniform1f(HUDSize + HUDCraftingY, i - m_RecipeY);
				if (m_UsingIndexRecipe == i)
				{


					for (int j = 0; j < m_VisibleRecipes[i].m_Ingredients.size(); j++)
					{

						float right = x + m_HalfOfSlotLeanght * 0.8f + (j + 1) * m_SlotGap;
						float left = x - m_HalfOfSlotLeanght * 0.8f + (j + 1) * m_SlotGap;
						drawNumber((y + m_HalfOfSlotLeanght) - m_HalfOfSlotLeanght * 0.8f, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_VisibleRecipes[i].m_Ingredients.at(j).m_Amount, scale, transform, fontSh);

					}

				}
				float right = x + m_HalfOfSlotLeanght;
				float left = x - m_HalfOfSlotLeanght;
				drawNumber(y, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_VisibleRecipes[i].m_AmountOutput, scale, transform, fontSh);


			}
		}
		fontSh.SetUniform1f(HUDSize + HUDCraftingY, 0);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				float right = (m_InvOffset[0] + m_HalfOfSlotLeanght + j * m_SlotGap);
				float left = (m_InvOffset[0] - m_HalfOfSlotLeanght + j * m_SlotGap);
				drawNumber(m_InvOffset[1] - m_HalfOfSlotLeanght - i * m_SlotGap, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_AmountInSlots[(i * 10) + j + 1], scale, transform, fontSh);
			}
		}
		if (m_IndexOfOpenChest != -1)
		{
			for (int i = 6; i < 11; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					float right = (m_InvOffset[0] + m_HalfOfSlotLeanght + j * m_SlotGap);
					float left = (m_InvOffset[0] - m_HalfOfSlotLeanght + j * m_SlotGap);
					drawNumber(m_InvOffset[1] - m_HalfOfSlotLeanght - i * m_SlotGap, right - (right - left) * 0.1f, left + (right - left) * 0.1f, chests.at(m_IndexOfOpenChest).m_amount[((i-6) * 10) + j], scale, transform, fontSh);
				}
			}
		}
			float right = (m_InvOffset[0] + m_HalfOfSlotLeanght + 9 * m_SlotGap);
			float left = (m_InvOffset[0] - m_HalfOfSlotLeanght + 9 * m_SlotGap);
			drawNumber(m_InvOffset[1] - m_HalfOfSlotLeanght - 5 * m_SlotGap, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_AmountInSlots[51], scale, transform, fontSh);
		

		HUDSh.Bind();
		ErrorGL(glBindVertexArray(m_HUDDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
		ChangeTransform(m_HPOffset[0], m_HPOffset[1], transform);
		HUDSh.SetUniformMat4(HUDBasicLocation, transform);
		ChangeScale(1, 1, scale);
		HUDSh.SetUniformMat4(HUDScale, scale);
		HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);

		for (int i = -3; i > -7; i--)
		{
			if (!m_PlayerSlots[52 - i - 3])
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ArmorSlotsTex[-i - 3]));
			}
			else
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
			}
			ChangeTransform(0, m_SlotGap * i, transform);
			HUDSh.SetUniformMat4(HUDTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));

		for (int i = -7; i > -11; i--)
		{
			ChangeTransform(0, m_SlotGap * i, transform);
			HUDSh.SetUniformMat4(HUDTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ArmorClassTex));
		ChangeTransform(0, m_SlotGap * -11, transform);
		HUDSh.SetUniformMat4(HUDTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		
		

		ChangeScale(0.8f, 0.8f, scale);
		HUDSh.SetUniformMat4(HUDScale, scale);

		for (int i = -3; i > -11; i--)
		{
			if (m_PlayerSlots[52 - i - 3])
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[52 - i - 3]]));
				ChangeTransform(0, m_SlotGap * i, transform);
				HUDSh.SetUniformMat4(HUDTransform, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			}
		}
		fontSh.Bind();
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
		ErrorGL(glBindVertexArray(fontDD));
		right = m_HPOffset[0] + m_HalfOfSlotLeanght;
		left = m_HPOffset[0] - m_HalfOfSlotLeanght;
		drawTwoNumbersWithZero(m_HPOffset[1] - m_HalfOfSlotLeanght/2.0f - 11 * m_SlotGap, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_ArmorClass, scale, transform, fontSh);

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (i + 1 == m_HUDUseSlot)
			{
				ChangeScale(1.2f, 1.2f, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4( HUDTransform, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
				ChangeScale(1, 1, scale);
				HUDSh.SetUniformMat4(HUDScale, scale);
			}
			else
			{
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(HUDTransform, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

			}

		}
		ChangeScale(0.8f, 0.8f, scale);
		HUDSh.SetUniformMat4(HUDScale, scale);
		for (int i = 0; i < 10; i++)
		{
			if (m_PlayerSlots[i + 1] != i_Nothing)
			{
				if (m_PlayerSlots[i + 1] >= i_WallDirt && m_PlayerSlots[i + 1] <= i_WallIce)
				{
					HUDSh.SetUniform1i( HUDSize + ShadowLocation, 1);
					ChangeTransform(i* m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4( HUDTransform, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[i + 1]]));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}
				else
				{
					HUDSh.SetUniform1i( HUDSize + ShadowLocation, 0);
					ChangeTransform(i * m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(HUDTransform, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[i + 1]]));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

				}
				HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);

			}
		}
		fontSh.Bind();
		ErrorGL(glBindVertexArray(fontDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
		for (int j = 0; j < 10; j++)
		{
			float right = (m_InvOffset[0] + m_HalfOfSlotLeanght + j * m_SlotGap);
			float left = (m_InvOffset[0] - m_HalfOfSlotLeanght + j * m_SlotGap);
			drawNumber(m_InvOffset[1] - m_HalfOfSlotLeanght, right - (right - left) * 0.1f, left + (right - left) * 0.1f, m_AmountInSlots[j + 1], scale, transform, fontSh);
		}
	}

	{
		HUDSh.Bind();
		ErrorGL(glBindVertexArray(m_HUDDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HPTexture[4]));
		ChangeTransform(m_HPOffset[0], m_HPOffset[1], transform);
		HUDSh.SetUniformMat4(HUDBasicLocation, transform);
		ChangeScale(1, 1, scale);
		HUDSh.SetUniformMat4(HUDScale, scale);
		HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);


		int DrawHP = m_CurrentHealth;
		int DrawMaxHP = m_maxHealth;
		DrawHP -= 25;
		DrawMaxHP -= 25;
		int i = 0;
		int j = 0;
		while (DrawHP > 0)
		{
			ChangeTransform(m_SlotGap * i, m_SlotGap * j, transform);
			HUDSh.SetUniformMat4(HUDTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			DrawHP -= 25;
			DrawMaxHP -= 25;
			i--;
			if (i <= -8)
			{
				i = 0;
				j--;
			}
		}
		DrawHP += 25;
		if ((int)floorf(DrawHP / 5.0f) != 5)
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HPTexture[(int)floorf(DrawHP / 5.0f)]));
		}
		else
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HPTexture[4]));
		}
		ChangeTransform(m_SlotGap * i, m_SlotGap * j, transform);
		HUDSh.SetUniformMat4(HUDTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HPTexture[0]));
		while (DrawMaxHP > 0)
		{
			i--;
			if (i <= -8)
			{
				i = 0;
				j--;
			}
			ChangeTransform(m_SlotGap * i, m_SlotGap * j, transform);
			HUDSh.SetUniformMat4(HUDTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			DrawMaxHP -= 25;

		}
	}
}
