#include"Player.h"

#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"glfw/Input.h"
#include"glfw/window.h"
#include"math/matrix.h"
#include"math/VectorOperation.h"
#include"NumberRender.h"

#define SPEEDOFANIM 0.8f
#define ARROWSTYPES 4
#define CANNONBALLSTYPES 5
#define BULLETSTYPES 4
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
unsigned char AmmunicionToProjectileType(unsigned char ammo)
{
	switch (ammo)
	{
	case i_BasicCannonBall:
		return p_BasicCannonBall;
	case i_BleedCannonBall:
		return p_BleedCannonBall;
	case i_BouncingCannonBall:
		return p_BouncingCannonBall;
	case i_FireCannonBall:
		return p_FireCannonBall;
	case i_BasicArrow:
		return p_BasicArrow;
	case i_BleedArrow:
		return p_BleedArrow;
	case i_BouncingArrow:
		return p_BouncingArrow;
	case i_FireArrow:
		return p_FireArrow;
	case i_BasicBullet:
		return p_BasicBullet;
	case i_BleedBullet:
		return p_BleedBullet;
	case i_BouncingBullet:
		return p_BouncingBullet;
	case i_FireBullet:
		return p_FireBullet;
	default:
		std::cout << "Error player.cpp Dont know this Ammo: " << ammo << std::endl;
		return -1;
	}
}
unsigned int getBehaviorByTexture(unsigned int texture)
{
	switch (texture)
	{
	
		case t_Ice:
			return b_Slippery;
			break;

		case t_Asphalt:
			return b_Asphalt;
			break;

		case t_Platform:
			return b_Platform;
			break;

		default:
			return b_BasicSolid;
			break;
	}
}
void getStructureVertices(int x
	,int y
	, unsigned int ID
	, float* vertices)
{
	switch (ID)
	{
	case s_Sapling:
		vertices[0] = x;
		vertices[1] = y + 1;
		vertices[2] = x;
		vertices[3] = y;
		break;
	}
}
Player::Player(unsigned int eob
	, unsigned int* texturesIDs)
{	

	m_FloorHit = false;
	m_CeilHit = false;
	m_WallHit = false;
	m_CoyoteTimer = 0;
	m_JumpTimer = 0;
	m_CanJump = false;
	m_DirectionLook = -1;
	m_JumpPower = 12;
	m_Gravity = -40;
	m_Acceleration = 0;
	m_Friction = 0;
	m_MaxMovementSpeed = 0;
	m_Transform[0] = 150;
	m_Transform[1] = 10;
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
	m_AddNextFrame = 0;
	m_UseSlot = 0;
	m_HUDUseSlot = 1;
	m_AimingAtSlot = -1;
	for (int i = 0; i < 52; i++)
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
	m_CurrentHealth = 55;
	m_maxHealth = 100;
	m_PlayerSlots[0] = i_Cannon;
	m_AmountInSlots[0] = 1;
	m_PlayerSlots[1] = i_Cannon;
	m_AmountInSlots[1] = 1;
	m_PlayerSlots[2] = i_WoodBow;
	m_AmountInSlots[2] = 1;
	m_PlayerSlots[3] = i_Pistol;
	m_AmountInSlots[3] = 1;
	
	m_PlayerSlots[4] = i_BasicArrow;
	m_PlayerSlots[5] = i_BleedArrow;
	m_PlayerSlots[6] = i_BouncingArrow;
	m_PlayerSlots[7] = i_FireArrow;
	m_PlayerSlots[8] = i_BasicCannonBall;
	m_PlayerSlots[9] = i_BleedCannonBall;
	m_PlayerSlots[10] = i_BouncingCannonBall;
	m_PlayerSlots[11] = i_FireCannonBall;
	m_PlayerSlots[12] = i_BasicBullet;
	m_PlayerSlots[13] = i_BleedBullet;
	m_PlayerSlots[14] = i_BouncingBullet;
	m_PlayerSlots[15] = i_FireBullet;

	m_AmountInSlots[4]  = 20;
	m_AmountInSlots[5]  = 20;
	m_AmountInSlots[6]  = 20;
	m_AmountInSlots[7]  = 20;
	m_AmountInSlots[8]  = 20;
	m_AmountInSlots[9]  = 20;
	m_AmountInSlots[10] = 20;
	m_AmountInSlots[11] = 20;
	m_AmountInSlots[12] = 20;
	m_AmountInSlots[13] = 20;
	m_AmountInSlots[14] = 20;
	m_AmountInSlots[15] = 20;


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
	m_AllItemTextures[i_BleedArrow] = CreateTextureRGBA("res/textures/bleedArrow.png");
	m_AllItemTextures[i_BouncingArrow] = CreateTextureRGBA("res/textures/bouncingArrow.png");
	m_AllItemTextures[i_FireArrow] = CreateTextureRGBA("res/textures/fireArrow.png");
	m_AllItemTextures[i_Cannon] = CreateTextureRGBA("res/textures/canonInv.png");
	m_AllItemTextures[i_BasicCannonBall] = CreateTextureRGBA("res/textures/basicCanonBall.png");
	m_AllItemTextures[i_BleedCannonBall] = CreateTextureRGBA("res/textures/scrapCannonBall.png");
	m_AllItemTextures[i_BouncingCannonBall] = CreateTextureRGBA("res/textures/bouncingCannonBall.png");
	m_AllItemTextures[i_FireCannonBall] = CreateTextureRGBA("res/textures/fireCannonBall.png");
	m_AllItemTextures[i_Pistol] = CreateTextureRGBA("res/textures/pistolInv.png");
	m_AllItemTextures[i_BasicBullet] = CreateTextureRGBA("res/textures/basicBullet.png");
	m_AllItemTextures[i_BleedBullet] = CreateTextureRGBA("res/textures/BleedBullet.png");
	m_AllItemTextures[i_BouncingBullet] = CreateTextureRGBA("res/textures/bouncingBullet.png");
	m_AllItemTextures[i_FireBullet] = CreateTextureRGBA("res/textures/fireBullet.png");

	m_BulletsDD  = CreateDrawData(eob,0.3f,-0.3f,0.2f,-0.2f);
	m_ItemsInHandDD[InHandBow] = CreateDrawData(eob, 2, 1, 1, -1);
	m_ItemsInHandTexture[InHandBow] = CreateTextureRGBA("res/textures/bowInHand.png");
	m_ItemsInHandDD[InHandCanon] = CreateDrawData(eob, 3, 1, 0.5f, -0.5f);
	m_ItemsInHandTexture[InHandCanon] = CreateTextureRGBA("res/textures/canonHand.png");
	m_ItemsInHandDD[InHandPistol] = CreateDrawData(eob, 2.5f, 1, 0.5f, -0.7f);
	m_ItemsInHandTexture[InHandPistol] = CreateTextureRGBA("res/textures/pistolHand.png");


	m_ItemInHandDD = CreateDrawData(eob, 2.5f, 1, 0, -1.5f);
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
	m_SlotVertices[0] = left; m_SlotVertices[1] = 0.01f * Window::height;
	m_SlotVertices[2] = right; m_SlotVertices[3] = m_SlotVertices[1] + (right - left);
	
	m_InvOffset[0] = (left + right) / 2.0f;
	m_HPOffset[0] = Window::width - (left + right) / 2.0f;
	m_InvOffset[1] = (top + down) / 2.0f;
	m_HPOffset[1] = m_InvOffset[1];
	down -= m_InvOffset[1];
	top -= m_InvOffset[1];
	left -= m_InvOffset[0];
	right -= m_InvOffset[0];
	m_HUDDD = CreateDrawData(eob, top, down, right, left);
	m_HPTexture[0] = CreateTextureRGBA("res/textures/0To5HP.png");
	m_HPTexture[1] = CreateTextureRGBA("res/textures/5To10HP.png");
	m_HPTexture[2] = CreateTextureRGBA("res/textures/10To15HP.png");
	m_HPTexture[3] = CreateTextureRGBA("res/textures/15To20HP.png");
	m_HPTexture[4] = CreateTextureRGBA("res/textures/20To25HP.png");


	m_SlotTexture = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_UseSlotTexture = CreateTextureRGBA("res/textures/useInventorySlot.png");
	m_TrashCanSlotTexture = CreateTextureRGBA("res/textures/trash.png");
	SwapItemStats();
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
	m_Placeable = false;
	m_LargePlaceable = false;
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
		case(i_Sapling):
			m_CooldownToUse = 0.1f;
			m_LargePlaceable = true;
			break;
		case i_WoodBow:
			m_Range = 0;
			m_WeaponType = weaponBow;
			m_CooldownToUse = 0.8;
			break;
		case i_Cannon:
			m_Range = 0;
			m_WeaponType = weaponCanon;
			m_CooldownToUse = 1.2;
			break;
		case i_Pistol:
			m_Range = 0;
			m_WeaponType = weaponGun;
			m_CooldownToUse = 0.4;
			break;
		case i_BleedArrow:
		case i_BasicArrow:
		case i_BouncingArrow:
		case i_FireArrow:
		case i_BasicCannonBall:
		case i_BleedCannonBall:
		case i_BouncingCannonBall:
		case i_FireCannonBall:
		case i_BasicBullet:
		case i_BleedBullet:
		case i_BouncingBullet:
		case i_FireBullet:
		case(i_Nothing):
			m_CooldownToUse = 0;
			m_Range = 0;
			break;
		default:
			m_CooldownToUse = 0.1f;
			m_Placeable = true;
			break;
	}


}
bool Player::IsItStackble(unsigned short int item)
{
	bool isItStackble = true;
	switch (item)
	{
	case i_CopperHammer:
	case i_CopperPickaxe:
	case i_CopperAxe:
	case i_CopperSword:
	case i_WoodBow:
		isItStackble = false;
		break;
	}
	return isItStackble;
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
char Player::FindOneOfItemsInInv(unsigned char* items, int sizeOfArray)
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
	, std::vector<bool>& isThereSandOnX
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
	, std::vector<Projectile>& projectiles)
{
	
	{
		if (m_ArmsBehaviour != ArmUsing)
		{
			m_AimingAtSlot = 0;
			if (m_IsInventoryOpen)
			{
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						if ((m_PlayerSlots[0] != i_Nothing && m_UseSlot != 0) || m_PlayerSlots[i * 10 + j + 1] != i_Nothing)
						{
							if (m_SlotGap * j + m_SlotVertices[0] < Input::XRawMousePos && m_SlotGap * j + m_SlotVertices[2] > Input::XRawMousePos && m_SlotGap * i + m_SlotVertices[1] < Input::YRawMousePos && m_SlotGap * i + m_SlotVertices[3] > Input::YRawMousePos)
							{
								m_AimingAtSlot = i * 10 + j + 1;
							}
						}
					}
				}
				if ((m_PlayerSlots[0] != i_Nothing && m_UseSlot != 0) || m_PlayerSlots[51] != i_Nothing)
				{
					if (m_SlotGap * 9 + m_SlotVertices[0] < Input::XRawMousePos && m_SlotGap * 9 + m_SlotVertices[2] > Input::XRawMousePos && m_SlotGap * 5 + m_SlotVertices[1] < Input::YRawMousePos && m_SlotGap * 5 + m_SlotVertices[3] > Input::YRawMousePos)
					{
						m_AimingAtSlot = 51;
					}
				}
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					if ((m_PlayerSlots[0] != i_Nothing && m_UseSlot != 0) || m_PlayerSlots[i] != i_Nothing)
					{
						if (m_SlotGap * i + m_SlotVertices[0] < Input::XRawMousePos && m_SlotGap * i + m_SlotVertices[2] > Input::XRawMousePos && + m_SlotVertices[1] < Input::YRawMousePos && + m_SlotVertices[3] > Input::YRawMousePos)
						{
							m_AimingAtSlot = i + 1;
						}
					}
				}
			}
			if (Input::LeftMousePress && m_AimingAtSlot)
			{
				if (m_IsInventoryOpen)
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
							m_PlayerSlots[0] = m_PlayerSlots[m_UseSlot];
							m_AmountInSlots[0] = m_AmountInSlots[m_UseSlot];
							m_PlayerSlots[m_UseSlot] = i_Nothing;
							m_AmountInSlots[m_UseSlot] = 0;
						}
						else
						{
							m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
							m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];
							m_UseSlot = 0;
							m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
							m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
						}
						SwapItemStats();
					}
					else if (Input::CtrlHold)
					{
						if (m_PlayerSlots[m_AimingAtSlot] != i_Nothing)
						{
							m_PlayerSlots[51] = m_PlayerSlots[m_AimingAtSlot];
							m_AmountInSlots[51] = m_AmountInSlots[m_AimingAtSlot];
							m_PlayerSlots[m_AimingAtSlot] = 0;
							m_AmountInSlots[m_AimingAtSlot] = 0;
							if (m_UseSlot == 0)
							{
								m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
								m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
								SwapItemStats();
							}
						}
					}
					else
					{
						m_UseSlot = m_AimingAtSlot;
						if (m_PlayerSlots[m_UseSlot] == m_PlayerSlots[0] && IsItStackble(m_PlayerSlots[0]) && m_AmountInSlots[0] != 9999 && m_AmountInSlots[m_UseSlot] != 9999)
						{
							if (m_AmountInSlots[0] + m_AmountInSlots[m_UseSlot] <= 9999)
							{
								m_AmountInSlots[m_UseSlot] += m_AmountInSlots[0];
								m_UseSlot = 0;
								m_PlayerSlots[0] = 0;
								m_AmountInSlots[0] = 0;
							}
							else
							{
								short int holdForAmountInSlot = m_AmountInSlots[0] + m_AmountInSlots[m_UseSlot] - 9999;
								m_AmountInSlots[m_UseSlot] += m_AmountInSlots[0] - holdForAmountInSlot;
								m_AmountInSlots[0] = holdForAmountInSlot;
							}
						}
						else if (m_PlayerSlots[0] == i_Nothing)
						{
							m_PlayerSlots[0] = m_PlayerSlots[m_UseSlot];
							m_AmountInSlots[0] = m_AmountInSlots[m_UseSlot];
							m_PlayerSlots[m_UseSlot] = i_Nothing;
							m_AmountInSlots[m_UseSlot] = 0;
						}
						else if (m_PlayerSlots[m_UseSlot] == i_Nothing)
						{
							m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
							m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];
							m_UseSlot = 0;
							m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
							m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
						}
						else
						{
							unsigned short int holdForPlyerSlot = m_PlayerSlots[m_UseSlot];
							unsigned short int holdForAmountInSlot = m_AmountInSlots[m_UseSlot];
							m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
							m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];
							m_PlayerSlots[0] = holdForPlyerSlot;
							m_AmountInSlots[0] = holdForAmountInSlot;
						}
						SwapItemStats();
					}
				}
				else
				{
					m_HUDUseSlot = m_AimingAtSlot;
					m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
					SwapItemStats();
				}

			}


			if (Input::MouseWheel && !m_IsInventoryOpen)
			{
				m_HUDUseSlot += Input::MouseWheel;
				if (10 < m_HUDUseSlot)
				{
					m_HUDUseSlot = 1;
				}
				else if (1 > m_HUDUseSlot)
				{
					m_HUDUseSlot = 10;
				}
				m_PlayerSlots[0] = m_PlayerSlots[m_HUDUseSlot];
				m_AmountInSlots[0] = m_AmountInSlots[m_HUDUseSlot];
				SwapItemStats();
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
			if (Input::RightMouseHold && m_AimingAtSlot && m_IsInventoryOpen && m_PlayerSlots[m_AimingAtSlot] && (m_PlayerSlots[0] == m_PlayerSlots[m_AimingAtSlot] || m_UseSlot == 0))
			{
				if (m_TimerSplitingItem == 1)
				{
					if (m_UseSlot == 0)
					{
						m_AmountInSlots[0] = 0;
					}
					m_AmountInSlots[m_AimingAtSlot]--;
					m_AmountInSlots[0]++;
					m_PlayerSlots[0] = m_PlayerSlots[m_AimingAtSlot];
					if (m_AmountInSlots[m_AimingAtSlot] <= 0)
					{
						m_PlayerSlots[m_AimingAtSlot] == i_Nothing;
					}
					m_UseSlot = m_AimingAtSlot;
					SwapItemStats();
				}
				else if (m_AmountInSlots[m_AimingAtSlot] - pow(m_TimerSplitingItem, 2) * deltaTime - m_AddNextFrame < 0)
				{
					m_AmountInSlots[0] += m_AmountInSlots[m_AimingAtSlot];
					m_AmountInSlots[m_AimingAtSlot] = 0;
					m_AddNextFrame = 0;
					m_PlayerSlots[m_AimingAtSlot] = i_Nothing;
				}
				else
				{
					m_AmountInSlots[0] += floorf(pow(m_TimerSplitingItem, 2) * deltaTime + m_AddNextFrame);
					m_AmountInSlots[m_AimingAtSlot] -= floorf(pow(m_TimerSplitingItem, 2) * deltaTime + m_AddNextFrame);
					m_AddNextFrame = pow(m_TimerSplitingItem, 2) * deltaTime + m_AddNextFrame - floorf(pow(m_TimerSplitingItem, 2) * deltaTime + m_AddNextFrame);
				}
				m_TimerSplitingItem += deltaTime;
			}
			else
			{
				m_TimerSplitingItem = 1;
				m_AddNextFrame = 0;
			}
		}
		if (Input::EscapePress)
		{
			if (m_IsInventoryOpen)
			{
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
	{
		float playerVertices[4] = { verticesPlayer[0], verticesPlayer[1], verticesPlayer[2], verticesPlayer[3] };

		
		int blockIndex = 0;
		int wallIndex = -1;
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
		if (m_AimingAtSlot == 0  )
		{
			
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
						FindWood(trees, x, y, inBlock);

						if (!inBlock)
						{
							inBlock = IsThereSeedling(seedlings, x, y);
						}
						if (!inBlock)
						{
							FindBlock(blocks, x, y, inBlock);
						}

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
					if (m_CursorOnMinableBlock && (blocks.at(x).at(blockIndex).m_BlockBehavior == b_Indestructible || blocks.at(x).at(blockIndex).m_Hardness > m_PickaxeStreanght))
					{
						m_CursorOnMinableBlock = false;
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
					switch (m_PlayerSlots[0])
					{
					case i_Sapling:
						vertices[0] = x;
						vertices[1] = y + 1;
						vertices[2] = x;
						vertices[3] = y;
						break;
					}
					if (vertices[0] <= Blocks::xMin)
					{
						inBlock = true;
					}
					else if (vertices[1] >= Blocks::yMax)
					{
						inBlock = true;
					}
					else if (vertices[2] >= Blocks::xMax)
					{
						inBlock = true;

					}
					else if (vertices[3] <= Blocks::yMin)
					{
						inBlock = true;
					}

					if (!inBlock)
					{
						inBlock = blockInArea(blocks, vertices);
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
					}
					if (floors)
					{
						if (!inBlock)
						{
							inBlock = WoodInArea(trees, vertices);
						}
						if (!inBlock)
						{
							inBlock = SeedlingInArea(seedlings, vertices);
						}
						if (!inBlock)
						{
							m_CursorOnPlaceableForStructure = true;
						}
					}
				}
			}
			else if (m_WeaponType)
			{
				unsigned char arrows[ARROWSTYPES] = { i_BasicArrow, i_BleedArrow, i_BouncingArrow, i_FireArrow };
				unsigned char cannonBalls[CANNONBALLSTYPES] = { i_BasicCannonBall, i_BleedCannonBall, i_BouncingCannonBall, i_FireCannonBall };
				unsigned char bullets[BULLETSTYPES] = {i_BasicBullet, i_BleedBullet, i_BouncingBullet, i_FireBullet};
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
						float velocity[2] = {Input::XMousePos - PLAYERHANDOFFSETX * m_DirectionLook, Input::YMousePos - PLAYERHANDOFFSETY };
						NormalizeVector(velocity);
						switch (m_PlayerSlots[0])
						{
						case weaponMelee:
							m_ArmsBehaviour = ArmUsing;
							break;
						case i_WoodBow:
					
							projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY, velocity[0] * 22, velocity[1] * 22, blockDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);

							break;
						case i_Cannon:

				
							projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY, velocity[0] * 15, velocity[1] * 15, blockDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);
							break;
						case i_Pistol:
						
							
							projectiles.emplace_back(AmmunicionToProjectileType(m_PlayerSlots[m_LocationAmmunition]), m_Transform[0] + PLAYERHANDOFFSETX * m_DirectionLook, m_Transform[1] + PLAYERHANDOFFSETY,velocity[0] * 30, velocity[1] * 30 , m_BulletsDD, m_AllItemTextures[m_PlayerSlots[m_LocationAmmunition]]);
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
						m_ArmsBehaviour = ArmUsing;
						break;
					}

					if (m_CursorOnMinableBlock)
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
					else if (m_CursorOnPlaceableSpot)
					{
						if (m_PlayerSlots[0] >= i_WallDirt && m_PlayerSlots[0] <= i_WallIce)
						{
							createWall(x, y, m_PlayerSlots[0], walls, blocks, texturesIDs);
							if (m_UseSlot)
							{
								m_AmountInSlots[0]--;
								if (!m_AmountInSlots[0])
								{
									m_UseSlot = 0;
									m_PlayerSlots[0] = i_Nothing;
									SwapItemStats();
								}
							}
							else
							{
								m_AmountInSlots[m_HUDUseSlot]--;
								if (m_AmountInSlots[m_HUDUseSlot] <= 0)
								{
									m_AmountInSlots[0] = 0;
									m_PlayerSlots[m_HUDUseSlot] = i_Nothing;
									m_PlayerSlots[0] = i_Nothing;
									SwapItemStats();
								}
							}
						}

						else
						{
							CreateBlock(x, y, m_PlayerSlots[0], walls, blocks, isThereSandOnX, texturesIDs);
							if (m_UseSlot)
							{
								m_AmountInSlots[0]--;
								if (!m_AmountInSlots[0])
								{
									m_UseSlot = 0;
									m_PlayerSlots[0] = i_Nothing;
									SwapItemStats();
								}
							}
							else
							{
								m_AmountInSlots[m_HUDUseSlot]--;
								if (m_AmountInSlots[m_HUDUseSlot] <= 0)
								{
									m_AmountInSlots[0] = 0;
									m_PlayerSlots[m_HUDUseSlot] = i_Nothing;
									m_PlayerSlots[0] = i_Nothing;
									SwapItemStats();
								}
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
						}
						m_AmountInSlots[0]--;
						if (m_UseSlot == 0)
						{
							m_AmountInSlots[m_HUDUseSlot]--;
						}
						if (m_AmountInSlots[0] <= 0)
						{
							m_PlayerSlots[0] = i_Nothing;
							if (m_UseSlot == 0)
							{
								m_PlayerSlots[m_HUDUseSlot] = i_Nothing;
							}
							SwapItemStats();
						}
					}
					m_UseItemTimer = 0;


				}
			}
		}
		if (m_UseItemTimer < 5)
		{
			m_UseItemTimer += deltaTime;
		}
	}
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
		m_WallHit = false;


		unsigned char moveBehavior = DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, verticesPlayer, blocks, m_WallHit, m_WallHit, m_FloorHit, m_CeilHit);
		if (AddVelocityToTransform(verticesPlayer, m_Transform, m_Velocity, m_FloorHit, m_WallHit, m_WallHit, m_CeilHit, deltaTime))
		{
			moveBehavior = b_BasicSolid;
		}


		switch (moveBehavior)
		{
		case(b_Air):
			m_Acceleration = 5.0f;
			m_Friction = 4;
			m_MaxMovementSpeed = 12;
			break;
		case(b_Slippery):
			m_Acceleration = 12.0f;
			m_Friction = 8;
			m_MaxMovementSpeed = 15;
			break;
		case(b_Asphalt):
			m_Acceleration = 30.0f;
			m_Friction = 70;
			m_MaxMovementSpeed = 25;
			break;
		case(b_BasicSolid):
		case(b_Platform):
		case(b_Sand):
			m_Acceleration = 25.0f;
			m_Friction = 40;
			m_MaxMovementSpeed = 10;
			break;
		}
		if (m_FloorHit)
		{
			m_CanJump = true;
			m_CoyoteTimer = 0.0f;
		}
		if (!m_FloorHit && m_CanJump && m_CoyoteTimer >= 0.125f)
		{
			m_CanJump = false;
			m_CoyoteTimer = 0.0f;
		}
	}
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
	{
		if (m_WeaponType > weaponNot)
		{
			if (Input::LeftMouseHold)
			{
				m_ArmsBehaviour = ArmUsing;
			}
			else
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
				m_ArmRotation = atan2f(Input::XMousePos - PLAYERHANDOFFSETX * m_DirectionLook, Input::YMousePos - PLAYERHANDOFFSETY) * 180.0 / PI;
				if (m_ArmRotation)
				{
					m_DirectionLook = m_ArmRotation / abs(m_ArmRotation);
				}
				m_ArmRotation = -abs(m_ArmRotation);
				break;
			default:
				m_ArmRotation -= 150 * deltaTime / m_CooldownToUse;
				if (m_UseItemTimer > m_CooldownToUse)
				{
					if (!Input::LeftMousePress)
					{
						m_ArmsBehaviour = ArmStanding;
					}
					m_ArmRotation = 0;
				}
				break;
			}
		}
	}
	
}
void Player::DrawPlayer(Shader& basicSh
	, Shader& HUDSh
	, Shader& fontSh
	, Shader& animSh
	, Shader& handSh
	, float* transform
	, float* scale
	, float* rotation
	, unsigned int fontDD
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
			ErrorGL(glBindVertexArray(m_HandDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_HandTex));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
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
		}
		else
		{
			animSh.SetUniform1i(animNumber, m_ArmPhase);
			ErrorGL(glBindVertexArray(m_BodyAnimDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_BodyAnimTex));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}

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
					HUDSh.SetUniformMat4( HUDTransform, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
					ChangeScale(1, 1, scale);
					HUDSh.SetUniformMat4( HUDScale, scale);
				}
				else
				{
					ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4( HUDTransform, transform);
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
						HUDSh.SetUniform1i(HUDSize + ShadowLocation, 1);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(HUDTransform, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

					}
					else
					{
						HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(HUDTransform, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					}
				}
			}
			HUDSh.SetUniform1i( HUDSize + ShadowLocation, 0);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
			ChangeScale(1, 1, scale);
			HUDSh.SetUniformMat4(HUDScale, scale);
		}
	
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
		
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				float right = (m_SlotVertices[2] + j * m_SlotGap);
				float left = (m_SlotVertices[0] + j * m_SlotGap);
				drawNumber(Window::height - m_SlotVertices[3] - i * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[(i * 10) + j + 1], fontDD, scale, transform, fontSh);
			}
		}
		
		float right = (m_SlotVertices[2] + 9 * m_SlotGap);
		float left = (m_SlotVertices[0] + 9 * m_SlotGap);
		drawNumber(Window::height - m_SlotVertices[3] - 5 * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[51], fontDD, scale, transform, fontSh);


		HUDSh.Bind();
		ErrorGL(glBindVertexArray(m_HUDDD));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
		ChangeTransform(m_HPOffset[0], m_HPOffset[1], transform);
		HUDSh.SetUniformMat4(HUDBasicLocation, transform);
		ChangeScale(1, 1, scale);
		HUDSh.SetUniformMat4(HUDScale, scale);
		HUDSh.SetUniform1i(HUDSize + ShadowLocation, 0);
		
		for (int i = -3; i > -11; i--)
		{
			ChangeTransform(0, m_SlotGap * i, transform);
			HUDSh.SetUniformMat4(HUDTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		
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
			float right = (m_SlotVertices[2] + j * m_SlotGap);
			float left = (m_SlotVertices[0] + j * m_SlotGap);
			drawNumber(Window::height - m_SlotVertices[3], left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[j + 1], fontDD, scale, transform, fontSh);
		}
	}

	
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
	ChangeTransform(m_SlotGap* i, m_SlotGap* j, transform);
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
		ChangeTransform(m_SlotGap* i, m_SlotGap* j, transform);
		HUDSh.SetUniformMat4(HUDTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		DrawMaxHP -= 25;
		
	}
}
