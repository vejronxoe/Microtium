#include"Player.h"


#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"glfw/Input.h"
#include"glfw/window.h"
#include"math/matrix.h"
#include"NumberRender.h"
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
void Player::CreateAllItemsTexture(unsigned int* texturesIDs)
{
	m_AllItemTextures[i_CooperPickaxe] = CreateTextureRGBA("res/textures/cooperPickaxe.png");
	m_AllItemTextures[i_CooperAxe] = CreateTextureRGBA("res/textures/cooperAxe.png");
	m_AllItemTextures[i_CooperHammer] = CreateTextureRGBA("res/textures/cooperHammer.png");
	m_AllItemTextures[i_CooperSword] = CreateTextureRGBA("res/textures/cooperSword.png");
	m_AllItemTextures[i_ForestPlank] = texturesIDs[t_ForestPlank];
	m_AllItemTextures[i_Dirt] = texturesIDs[t_Dirt];
	m_AllItemTextures[i_Ice] = texturesIDs[t_Ice];         
	m_AllItemTextures[i_Asphalt] = texturesIDs[t_Asphalt];
	m_AllItemTextures[i_Platform] = texturesIDs[t_Platform];
	m_AllItemTextures[i_WallDirt] = texturesIDs[t_Dirt];
	m_AllItemTextures[i_WallIce] = texturesIDs[t_Ice];
	m_AllItemTextures[i_Sand] = texturesIDs[t_Sand];
	m_AllItemTextures[i_Sapling] = CreateTextureRGBA("res/textures/saplingInv.png");
}									   

Player::Player(unsigned int eob
	, unsigned int HUDTransformLocatin
	, unsigned int HUDScaleLocatin
	, float& yLocationOfFirstSlot
	, float& xLocationOfFirstSlot
	, unsigned int* texturesIDs)
	:m_FloorHit(false), m_CeilHit(false), m_WallHit(false), m_CoyoteTimer(0), m_JumpTimer(0), m_CanJump(false), m_JumpPower(20), m_Gravity(-60.0f), m_Acceleration(25.0f), m_Friction(30), m_MaxMovementSpeed(10), m_Velocity{ 0,0 }, m_Transform{ 128, 0 }
{	
	m_WalkingTimer = 0;
	m_WalkingPhase = 0;
	m_HUDTransformLocation = HUDTransformLocatin;
	m_HUDScaleLocation = HUDScaleLocatin;
	m_InventoryDrawData = 0;
	m_SlotTexture = 0;
	m_UseSlotTexture = 0;
	m_SlotGap = 0;
	m_IsInventoryOpen = 0;
	m_HUDUseSlot = 1;
	m_UseSlot = 0;
	m_DirectionLook = -1;
	for (int i = 0; i < 52; i++)
	{
		m_PlayerSlots[i] = i_Nothing;
	}
	for (int i = 0; i < 52; i++)
	{
		m_AmountInSlots[i] = 0;
	}
	m_PlayerSlots[1] = i_CooperSword;
	m_PlayerSlots[2] = i_CooperPickaxe;
	m_PlayerSlots[3] = i_CooperAxe;
	m_PlayerSlots[4] = i_Dirt;
	m_PlayerSlots[5] = i_CooperHammer;
	m_PlayerSlots[6] = i_Sand;
	m_PlayerSlots[6] = i_Sand;
	m_PlayerSlots[7] = i_Sapling;
	m_AmountInSlots[1] = 1;
	m_AmountInSlots[2] = 1;
	m_AmountInSlots[3] = 1;
	m_AmountInSlots[4] = 9999;
	m_AmountInSlots[5] = 1;
	m_AmountInSlots[6] = 20;
	m_AmountInSlots[7] = 20;
	m_UseItemTimer = 0;
	m_CooldownToUse = 0;
	m_PickaxeStreanght = 0;
	m_AxeStreanght = 0;
	m_HammerStreanght = 0;
	m_Range = 0;
	m_Damage = 0;
	m_Placeable = 0;
	m_AimingAtSlot = 0;
	m_LargePlaceable = false;
	

	m_WalkingTex = CreateTextureRGBA("res/textures/walkAnimationDefault.png");
	CreateAllItemsTexture(texturesIDs);
	
	m_LegsDD = CreateDrawData(eob, -0.2, -1.5f, 1.0f, -1.0f, 1, 0, 1.0f / 5.0f, 0);


	unsigned int inventoryVertexBuffer;
	float inventoryVertices[20];
	float left = 0.02f * Window::width;
	float top = Window::height - 0.01f * Window::height;
	float workSpace = (Window::width / 2.0f) - left;
	float canNotDivide = ((int)std::floor(workSpace) % (18 + Window::scaleOfHUD)) - std::floor(workSpace) + workSpace;
	float right = ((workSpace - canNotDivide) / (18.0f + Window::scaleOfHUD)) + left;
	float down = top - (right - left);
	m_SlotGap = right - left + 0.005f * Window::height;
	m_SlotVertices[0] = left; m_SlotVertices[1] = 0.01f *Window::height;
	m_SlotVertices[2] = right; m_SlotVertices[3] = m_SlotVertices[1] + (right - left);
	yLocationOfFirstSlot = (top + down) / 2.0f;
	xLocationOfFirstSlot = (left + right) / 2.0f;
	down -= yLocationOfFirstSlot;
	top -= yLocationOfFirstSlot;
	left -= xLocationOfFirstSlot;
	right -= xLocationOfFirstSlot;
	m_InventoryDrawData = CreateDrawData(eob, top, down, right, left);


	m_SlotTexture = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_UseSlotTexture = CreateTextureRGBA("res/textures/useInventorySlot.png");
	m_TrashCanSlotTexture = CreateTextureRGBA("res/textures/trash.png");

}

void Player::SwapItemStats()
{
	m_CooldownToUse = 0.4;
	m_PickaxeStreanght = 0;
	m_AxeStreanght = 0;
	m_HammerStreanght = 0;
	m_Range = 4;
	m_Damage = 0;
	m_Placeable = false;
	m_LargePlaceable = false;
	switch (m_PlayerSlots[0])
	{
		case(i_Nothing):
			m_CooldownToUse = 0;
			m_Range = 0;
		break;
		case(i_CooperSword):
			m_Damage = 2;
		break;
		
		case(i_CooperAxe):
			m_AxeStreanght = 35;
			m_Damage = 1;
		break;
		

		case(i_CooperPickaxe):
			m_PickaxeStreanght = 35;
			m_Damage = 1;
		break;
		
		case(i_CooperHammer):
			m_HammerStreanght = 35;
			m_Damage = 1;
		break;
		case(i_Sapling):
			m_CooldownToUse = 0.1f;
			m_LargePlaceable = true;
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
	case(i_CooperPickaxe):
		isItStackble = false;
		break;

	case(i_CooperAxe):
		isItStackble = false;
		break;

	case(i_CooperHammer):
		isItStackble = false;
		break;

	case(i_CooperSword):
		isItStackble = false;
		break;
	}
	return isItStackble;
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
	, unsigned int* texturesIDs
	, unsigned int* structuresTextures
	, std::vector<tree>& trees
	, std::vector<seedling>& seedlings
	, std::vector<DroppedItem>& droppedItems)
{

	{
		m_AimingAtSlot = 0;
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
		if (m_SlotGap * 9 + m_SlotVertices[0] < Input::XRawMousePos && m_SlotGap * 9 + m_SlotVertices[2] > Input::XRawMousePos && m_SlotGap * 5 + m_SlotVertices[1] < Input::YRawMousePos && m_SlotGap * 5 + m_SlotVertices[3] > Input::YRawMousePos)
		{
			m_AimingAtSlot = 51;
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

	float verticesPlayer[4] = { m_Transform[0] - 0.8 ,m_Transform[1] + 1.3 ,m_Transform[0] + 0.8,m_Transform[1] - 1.5 };
	{
		float playerVertices[4] = { verticesPlayer[0], verticesPlayer[1], verticesPlayer[2], verticesPlayer[3] };

		int x = roundf(Input::XMousePos + CameraCoordinates[0]);
		int y = roundf(Input::YMousePos + CameraCoordinates[1]);

		int blockIndex = 0;
		int wallIndex = -1;
		int woodIndex = 0;
		bool inBlock = false;

		int rangeX = x - m_Transform[0];
		int rangeY = y - m_Transform[1];

		if (Input::TPress && m_PlayerSlots[0] != i_Nothing && m_UseSlot == 0)
		{
			droppedItems.emplace_back(m_Transform[0], m_Transform[1], m_DirectionLook, m_PlayerSlots[0], m_AmountInSlots[0], false);
			m_PlayerSlots[0] = 0;
			m_PlayerSlots[m_HUDUseSlot] = 0;
			m_AmountInSlots[0] = 0;
			m_AmountInSlots[m_HUDUseSlot] = 0;
			SwapItemStats();
		}
		else if (Input::RightMousePress && m_PlayerSlots[0] != i_Nothing && m_UseSlot != 0 && !m_AimingAtSlot)
		{
			droppedItems.emplace_back(m_Transform[0], m_Transform[1], m_DirectionLook, m_PlayerSlots[0], m_AmountInSlots[0], false);
			m_PlayerSlots[0] = 0;
			m_AmountInSlots[0] = 0;
			SwapItemStats();
		}
		m_CursorOnPlaceableForStructure = false;
		m_CursorOnPlaceableSpot = false;
		m_CursorOnMinableBlock = false;
		m_CursorOnMinableWall = false;
		m_CursorOnMinableWood = false;
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
							for (int j = 0; j < blocks.at(i).size(); i++)
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
					if (!inBlock )
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



		if (Input::LeftMouseHold)
		{
			if (m_UseItemTimer > m_CooldownToUse)
			{
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
		if (m_UseItemTimer < 5)
		{
			m_UseItemTimer += deltaTime;
		}
	}
	m_CoyoteTimer += deltaTime;
	{
		if (Input::DHold)
		{
			m_DirectionLook = 1;
		}
		if (Input::AHold)
		{
			m_DirectionLook = -1;
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
		else if (Input::SpaceHold && m_JumpTimer > 0 && m_JumpTimer < 0.25f)
		{
			m_Velocity[1] -= (m_JumpPower / 2) * deltaTime;
			m_JumpTimer += deltaTime;
		}
		else
		{
			m_Velocity[1] += m_Gravity * deltaTime;
			if (30 < m_Gravity)
			{
				m_Gravity = 30;
			}
			m_JumpTimer = 0;
		}

		m_FloorHit = false;
		m_CeilHit = false;
		m_WallHit = false;


		unsigned char moveBehavior = DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, verticesPlayer, blocks, m_WallHit, m_WallHit, m_FloorHit, m_CeilHit);
		if (AddVelocityToTransform(verticesPlayer, m_Transform, m_Velocity, m_FloorHit, deltaTime))
		{
			moveBehavior = b_BasicSolid;
		}
		if (m_FloorHit)
		{
			m_WalkingTimer += deltaTime;

			if (0 < m_Velocity[0])
			{
				if (0.1f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else if (1.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
				}
				else if (2.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 1;
				}
				else if (3.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
				}
				else if (4.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else if (5.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 4;
				}
				else if (6.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 2;
				}
				else if (7.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else
				{
					m_WalkingTimer = 0;
				}
			}
			else if (0 > m_Velocity[0])
			{
				if (0.1f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else if (1.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
				}
				else if (2.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 1;
				}
				else if (3.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 3;
				}
				else if (4.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else if (5.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 4;
				}
				else if (6.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 2;
				}
				else if (7.0f / abs(m_Velocity[0]) >= m_WalkingTimer)
				{
					m_WalkingPhase = 0;
				}
				else
				{
					m_WalkingTimer = 0;
				}
			}
			else
			{
				m_WalkingPhase = 0;
			}
		}
		else
		{
			m_WalkingPhase = 1;
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
}

void Player::DrawPlayer(Shader& basicSh
	, Shader& HUDSh
	, Shader& fontSh
	, Shader& animSh
	, unsigned int shadowLocation
	, unsigned int transformLocation
	, float* transform
	, float* scale
	, unsigned int fontDrawData
	, unsigned int numberLocation
	, unsigned int fontTransformLocation
	, unsigned int fontscaleLocation
	, unsigned int numberTexture
	, unsigned int animTransformLocation
	, unsigned int animScaleLocation
	, unsigned int animNumberLocation
	, unsigned int animLeangthLocation)
{
	animSh.Bind();
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	animSh.SetUniformMat4(animTransformLocation, transform);
	ChangeScale( m_DirectionLook, 1, scale);
	animSh.SetUniformMat4(animScaleLocation, scale);
	animSh.SetUniform1i(animLeangthLocation, 5);
	animSh.SetUniform1i(animNumberLocation, m_WalkingPhase);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_WalkingTex));
	ErrorGL(glBindVertexArray(m_LegsDD));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	HUDSh.Bind();
	ErrorGL(glBindVertexArray(m_InventoryDrawData));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
	ChangeScale(1, 1, scale);
	HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
	HUDSh.SetUniform1i(shadowLocation, 0);

	if (m_IsInventoryOpen)
	{

		for (int i = 0; i < 5; i++)
		{

			for (int j = 0; j < 10; j++)
			{
				if (i == 0 && j + 1 == m_HUDUseSlot && m_UseSlot == 0)
				{
					ChangeScale(1.2f, 1.2f, scale);
					HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
					ChangeTransform(j * m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
					ChangeScale(1, 1, scale);
					HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
				}
				else
				{
					ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}

			}
			ChangeScale(0.8f, 0.8f, scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);

			for (int j = 0; j < 10; j++)
			{
				
				if (m_PlayerSlots[(i * 10) + (j + 1)] != i_Nothing)
				{
					if (m_PlayerSlots[(i * 10) + (j + 1)] >= i_WallDirt && m_PlayerSlots[(i * 10) + (j + 1)] <= i_WallIce)
					{
						HUDSh.SetUniform1i(shadowLocation, 1);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

					}
					else
					{
						HUDSh.SetUniform1i(shadowLocation, 0);
						ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
						HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
						ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
						ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					}
				}
			}
			HUDSh.SetUniform1i(shadowLocation, 0);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
			ChangeScale(1, 1, scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
		}
	
		ChangeTransform(9 * m_SlotGap, -5 * m_SlotGap, transform);
		HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_TrashCanSlotTexture));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (m_PlayerSlots[51] != i_Nothing)
		{
			ChangeScale(0.8f, 0.8f, scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[51]]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		fontSh.Bind();
		ErrorGL(glBindVertexArray(fontDrawData));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
		
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				float right = (m_SlotVertices[2] + j * m_SlotGap);
				float left = (m_SlotVertices[0] + j * m_SlotGap);
				drawNumber(Window::height - m_SlotVertices[3] - i * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[(i * 10) + j + 1], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, scale, transform, fontSh);
			}
		}
		
		float right = (m_SlotVertices[2] + 9 * m_SlotGap);
		float left = (m_SlotVertices[0] + 9 * m_SlotGap);
		drawNumber(Window::height - m_SlotVertices[3] - 5 * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[51], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, scale, transform, fontSh);

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (i + 1 == m_HUDUseSlot)
			{
				ChangeScale(1.2f, 1.2f, scale);
				HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
				ChangeScale(1, 1, scale);
				HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
			}
			else
			{
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

			}

		}
		ChangeScale(0.8f, 0.8f, scale);
		HUDSh.SetUniformMat4(m_HUDScaleLocation, scale);
		for (int i = 0; i < 10; i++)
		{
			if (m_PlayerSlots[i + 1] != i_Nothing)
			{
				if (m_PlayerSlots[i + 1] >= i_WallDirt && m_PlayerSlots[i + 1] <= i_WallIce)
				{
					HUDSh.SetUniform1i(shadowLocation, 1);
					ChangeTransform(i * m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[i + 1]]));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}
				else
				{
					HUDSh.SetUniform1i(shadowLocation, 0);
					ChangeTransform(i* m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[i + 1]]));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

				}
				HUDSh.SetUniform1i(shadowLocation, 0);

			}
		}
		fontSh.Bind();
		ErrorGL(glBindVertexArray(fontDrawData));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
		for (int j = 0; j < 10; j++)
		{
			float right = (m_SlotVertices[2] + j * m_SlotGap);
			float left = (m_SlotVertices[0] + j * m_SlotGap);
			drawNumber(Window::height - m_SlotVertices[3], left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[j + 1], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, scale, transform, fontSh);
		}
	}

}
