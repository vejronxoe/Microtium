#include"Player.h"


#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"glfw/Input.h"
#include"glfw/window.h"
#include"math/matrix.h"
#include"NumberRender.h"

void Player::CreateAllItemsTexture(unsigned int* texturesIDs)
{
	m_AllItemTextures[i_CooperPickaxe] = CreateTextureRGBA("res/textures/cooperPickaxe.png");
	m_AllItemTextures[i_CooperAxe] = CreateTextureRGBA("res/textures/cooperAxe.png");
	m_AllItemTextures[i_CooperHammer] = CreateTextureRGBA("res/textures/cooperHammer.png");
	m_AllItemTextures[i_CooperSword] = CreateTextureRGBA("res/textures/cooperSword.png");
	m_AllItemTextures[i_Dirt] = texturesIDs[t_Dirt];
	m_AllItemTextures[i_Ice] = texturesIDs[t_Ice];
	m_AllItemTextures[i_Asphalt] = texturesIDs[t_Asphalt];
	m_AllItemTextures[i_Platform] = texturesIDs[t_Platform];
}

Player::Player(unsigned int eob, unsigned int HUDTransformLocatin, unsigned int HUDScaleLocatin, float& yLocationOfFirstSlot, float& xLocationOfFirstSlot, unsigned int* texturesIDs)
	:m_PlayerDrawData(0), m_Tex(0), m_FloorHit(false), m_CeilHit(false), m_WallHit(false), m_CoyoteTimer(0), m_JumpTimer(0), m_CanJump(false), m_JumpPower(20), m_Gravity(-60.0f), m_Acceleration(25.0f), m_Friction(30), m_MaxMovementSpeed(10), m_Velocity{ 0,0 }, m_Transform{ 128, 0 }
{	
	m_HUDTransformLocation = HUDTransformLocatin;
	m_HUDScaleLocation = HUDScaleLocatin;
	m_InventoryDrawData = 0;
	m_SlotTexture = 0;
	m_UseSlotTexture = 0;
	m_SlotGap = 0;
	m_IsInventoryOpen = 0;
	m_HUDUseSlot = 0;
	m_UseSlot = 0;
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
	m_AmountInSlots[1] = 1;
	m_AmountInSlots[2] = 1;
	m_AmountInSlots[3] = 1;
	m_AmountInSlots[4] = 10;
	m_UseItemTimer = 0;
	m_CooldownToUse = 0;
	m_PickaxeStreanght = 0;
	m_AxeSteanght = 0;
	m_HammerStreanght = 0;
	m_Range = 0;
	m_Damage = 0;
	m_Placeable = 0;
	m_AimingAtSlot = 0;


	m_Tex = CreateTextureRGBA("res/textures/player0.png");
	CreateAllItemsTexture(texturesIDs);
	
	m_PlayerDrawData = CreateDrawData(eob, 1.5f, -1.5f, 1.0f, -1.0f);


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

	CreateScale(1,1,m_Scale);

	m_SlotTexture = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_UseSlotTexture = CreateTextureRGBA("res/textures/useInventorySlot.png");
	m_TrashCanSlotTexture = CreateTextureRGBA("res/textures/trash.png");

}
void Player::SwapItemStats()
{
	m_CooldownToUse = 0.5f;
	m_PickaxeStreanght = 0;
	m_AxeSteanght = 0;
	m_HammerStreanght = 0;
	m_Range = 4;
	m_Damage = 0;
	m_Placeable = true;
	switch (m_PlayerSlots[0])
	{
		case(i_Nothing):
			m_CooldownToUse = 0;
			m_Range = 0;
			m_Placeable = false;
		break;
		case(i_CooperSword):
			m_Damage = 2;
			m_Placeable = false;
		break;
		
		case(i_CooperAxe):
			m_AxeSteanght = 1;
			m_Damage = 1;
			m_Placeable = false;
		break;
		

		case(i_CooperPickaxe):
			m_PickaxeStreanght = 1;
			m_Damage = 1;
			m_Placeable = false;
		break;
		
		case(i_CooperHammer):
			m_HammerStreanght = 1;
			m_Damage = 1;
			m_Placeable = false;
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
void Player::ItermGetToInventory(unsigned short int amount, unsigned short int item)
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

			if (!isItDone && freeSlot)
			{
				m_AmountInSlots[freeSlot] = amount;
				m_PlayerSlots[freeSlot] = item;
				isItDone = true;
			}

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
	if (!isItDone)
	{
		//drop Item
	}
}
void Player::MovementEveryFrame(float deltaTime, std::vector<std::vector<Block>>& blocks)
{
	m_CoyoteTimer += deltaTime;
	if (Input::DHold)
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
	if (Input::AHold)
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

	if (abs(m_Velocity[0]) > m_MaxMovementSpeed)
		m_Velocity[0] = m_MaxMovementSpeed * (m_Velocity[0] / abs(m_Velocity[0]));
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
	
	float hitboxvertices[4] = { -1.0f + m_Transform[0],1.5f + m_Transform[1],1.0f + m_Transform[0],-1.5f + m_Transform[1] };

	unsigned char moveBehavior = DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, hitboxvertices, blocks, m_WallHit, m_WallHit, m_FloorHit, m_CeilHit);
	switch (moveBehavior)
	{
	case(b_Air):
		m_Acceleration = 5.0f;
		m_Friction = 5;
		m_MaxMovementSpeed = 12;
		break;
	case(b_Slippery):
		m_Acceleration = 12.0f;
		m_Friction = 7;
		m_MaxMovementSpeed = 14;
		break;
	case(b_Asphalt):
		m_Acceleration = 20.0f;
		m_Friction = 60;
		m_MaxMovementSpeed = 20;
		break;
	case(b_BasicSolid):
		m_Acceleration = 25.0f;
		m_Friction = 30;
		m_MaxMovementSpeed = 10;
		break;
	}
	AddVelocityToTransform(hitboxvertices, m_Transform, m_Velocity, m_FloorHit,deltaTime);
	if (m_FloorHit)
	{
		m_CanJump = true;
		m_CoyoteTimer = 0.0f;
	}
	if (!m_FloorHit && m_CanJump && m_CoyoteTimer >= 0.25f)
	{
		m_CanJump = false;
		m_CoyoteTimer = 0.0f;
	}
}
void Player::IventoryEveryFrame()
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
			if (m_AimingAtSlot == 51)
			{
				if (m_UseSlot == 0)
				{
					m_PlayerSlots[0] = i_Nothing;
					m_AmountInSlots[0] = 0;
				}
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
			else
			{

				if (m_UseSlot == 0)
				{
					m_PlayerSlots[0] = i_Nothing;
					m_AmountInSlots[0] = 0;
				}
				m_UseSlot = m_AimingAtSlot;
				if (m_PlayerSlots[0] == i_Nothing)
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
}
void Player::EveryFrame(float deltaTime, std::vector<std::vector<Block>>& blocks, std::vector<DamagedBlock>& damageblocks, float* CameraCoordinates, unsigned int* texturesIDs)
{


	IventoryEveryFrame();

	int x = roundf(Input::XMousePos + CameraCoordinates[0]);
	int y = roundf(Input::YMousePos + CameraCoordinates[1]);

	int blockIndex = 0;

	int rangeX = Input::XMousePos + CameraCoordinates[0] - m_Transform[0];
	int rangeY = Input::YMousePos + CameraCoordinates[1] - m_Transform[1];

	m_CursorOnPlaceableSpot = false;
	m_CursorOnMinableBlock = false;
	if (m_Range >= sqrtf(rangeX * rangeX + rangeY * rangeY))
	{
		
		if (m_Placeable)
		{
			float verticesPlayer[4] = { m_Transform[0] - 1 ,m_Transform[1] + 1.5f,m_Transform[0] + 1,m_Transform[1] - 1.5f };
			verticesPlayer[0] = roundf(verticesPlayer[0]);
			if ((verticesPlayer[3] - std::floorf(verticesPlayer[3])) == 0.5f)
			{
				verticesPlayer[3] = std::ceilf(verticesPlayer[3]);
			}
			else
			{
				verticesPlayer[3] = roundf(verticesPlayer[3]);
			}
			if ((verticesPlayer[1] - std::floorf(verticesPlayer[1])) == 0.5f)
			{
				verticesPlayer[1] = std::floorf(verticesPlayer[1]);
			}
			else
			{
				verticesPlayer[1] = roundf(verticesPlayer[1]);
			}
			if ((verticesPlayer[2] - std::floorf(verticesPlayer[2])) == 0.5f)
			{
				verticesPlayer[2] = std::floorf(verticesPlayer[2]);
			}
			else
			{
				verticesPlayer[2] = roundf(verticesPlayer[2]);
			}

			bool inBlock = false;
			bool youCanbuild = false;
			if (!(x >= verticesPlayer[0] && x <= verticesPlayer[2] && y <= verticesPlayer[1] && y >= verticesPlayer[3]))
			{
				for (blockIndex = 0; blockIndex < blocks.at(x).size(); blockIndex++)
				{

					if (y == blocks.at(x).at(blockIndex).m_Transform[1])
					{
						inBlock = true;
						break;
					}
					else if (y + 1 == blocks.at(x).at(blockIndex).m_Transform[1] || y - 1 == blocks.at(x).at(blockIndex).m_Transform[1])
					{
						youCanbuild = true;
					}
				}
				if (!inBlock && youCanbuild)
				{
					m_CursorOnPlaceableSpot = true;
				}
				else if (!inBlock)
				{


					for (blockIndex = 0; blockIndex < blocks.at(x - 1).size(); blockIndex++)
					{
						if (y == blocks.at(x - 1).at(blockIndex).m_Transform[1])
						{
							youCanbuild = true;
							break;
						}
					}
					if (youCanbuild)
					{
						m_CursorOnPlaceableSpot = true;
					}
					else
					{
						for (blockIndex = 0; blockIndex < blocks.at(x + 1).size(); blockIndex++)
						{
							if (y == blocks.at(x + 1).at(blockIndex).m_Transform[1])
							{
								m_CursorOnPlaceableSpot = true;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			for (blockIndex = 0; blockIndex < blocks.at(x).size(); blockIndex++)
			{
				if (y == blocks.at(x).at(blockIndex).m_Transform[1])
				{
					if (m_PickaxeStreanght >= blocks.at(x).at(blockIndex).m_Hardness)
					{

						m_CursorOnMinableBlock = true;
					}
					break;
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
					if (damageblocks.at(damageIndex).m_Transform[0] == x && damageblocks.at(damageIndex).m_Transform[1] == blocks.at(x).at(blockIndex).m_Transform[1])
					{
						damaged = true;
						break;
					}
				}
				if (damaged)
				{
					damageblocks.erase(damageblocks.begin() + damageIndex);
					blocks.at(x).erase(blocks.at(x).begin() + blockIndex);
				}
				else if (0 >= ((float)blocks.at(x).at(blockIndex).m_Hardness) - ((float)m_PickaxeStreanght / 2.0f))
				{
					blocks.at(x).erase(blocks.at(x).begin() + blockIndex);
				}
				else
				{
					damageblocks.emplace_back(x, blocks.at(x).at(blockIndex).m_Transform[1]);
				}

			}
			else if (m_CursorOnPlaceableSpot)
			{
				switch (m_PlayerSlots[0])
				{
				case i_Dirt:
					blocks.at(x).emplace_back(texturesIDs[t_Dirt], x, y, b_BasicSolid, 1);
					break;
				case i_Platform:
					blocks.at(x).emplace_back(texturesIDs[t_Platform], x, y, b_Platform, 1);
					break;
				case i_Asphalt:
					blocks.at(x).emplace_back(texturesIDs[t_Asphalt], x, y, b_Asphalt, 1);
					break;
				case i_Ice:
					blocks.at(x).emplace_back(texturesIDs[t_Ice], x, y, b_Slippery, 1);
					break;
				}
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
			m_UseItemTimer = 0;

		}
	}
	if (m_UseItemTimer < 5)
	{
		m_UseItemTimer += deltaTime;
	}


	MovementEveryFrame(deltaTime, blocks);
}
void Player::DrawPlayer(Shader& basicSh, Shader& HUDSh, Shader& fontSh, unsigned int transformLocation, float* transform, int fontDrawData, int numberLocation, int fontTransformLocation, int fontscaleLocation, int numberTexture)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicSh.SetUniformMat4(transformLocation, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex));
	ErrorGL(glBindVertexArray(m_PlayerDrawData));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	HUDSh.Bind();
	ErrorGL(glBindVertexArray(m_InventoryDrawData));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
	ChangeScale(1, 1, m_Scale);
	HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);

	if (m_IsInventoryOpen)
	{

		for (int i = 0; i < 5; i++)
		{

			for (int j = 0; j < 10; j++)
			{
				if (i == 0 && j + 1 == m_HUDUseSlot && m_UseSlot == 0)
				{
					ChangeScale(1.2f, 1.2f, m_Scale);
					HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
					ChangeTransform(j * m_SlotGap, 0, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
					ChangeScale(1, 1, m_Scale);
					HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
				}
				else
				{
					ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}

			}
			ChangeScale(0.8f, 0.8f, m_Scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);

			for (int j = 0; j < 10; j++)
			{
				
				if (m_PlayerSlots[(i * 10) + (j + 1)] != i_Nothing)
				{
					ChangeTransform(j * m_SlotGap, -i * m_SlotGap, transform);
					HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[(i * 10) + (j + 1)]]));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}
			}
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
			ChangeScale(1, 1, m_Scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
		}
	
		ChangeTransform(9 * m_SlotGap, -5 * m_SlotGap, transform);
		HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_TrashCanSlotTexture));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (m_PlayerSlots[51] != i_Nothing)
		{
			ChangeScale(0.8f, 0.8f, m_Scale);
			HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
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
				drawNumber(Window::height - m_SlotVertices[3] - i * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[(i * 10) + j + 1], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, m_Scale, transform, fontSh);
			}
		}
		
		float right = (m_SlotVertices[2] + 9 * m_SlotGap);
		float left = (m_SlotVertices[0] + 9 * m_SlotGap);
		drawNumber(Window::height - m_SlotVertices[3] - 5 * m_SlotGap, left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[51], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, m_Scale, transform, fontSh);

	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (i + 1 == m_HUDUseSlot)
			{
				ChangeScale(1.2f, 1.2f, m_Scale);
				HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_UseSlotTexture));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexture));
				ChangeScale(1, 1, m_Scale);
				HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
			}
			else
			{
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

			}

		}
		ChangeScale(0.8f, 0.8f, m_Scale);
		HUDSh.SetUniformMat4(m_HUDScaleLocation, m_Scale);
		for (int i = 0; i < 10; i++)
		{
			if (m_PlayerSlots[i + 1] != i_Nothing)
			{
				ChangeTransform(i * m_SlotGap, 0, transform);
				HUDSh.SetUniformMat4(m_HUDTransformLocation, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, m_AllItemTextures[m_PlayerSlots[i + 1]]));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			}
		}
		fontSh.Bind();
		ErrorGL(glBindVertexArray(fontDrawData));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
		for (int j = 0; j < 10; j++)
		{
			float right = (m_SlotVertices[2] + j * m_SlotGap);
			float left = (m_SlotVertices[0] + j * m_SlotGap);
			drawNumber(Window::height - m_SlotVertices[3], left + (right - left) * 0.1f, right - (right - left) * 0.1f, m_AmountInSlots[j + 1], fontDrawData, numberLocation, fontTransformLocation, fontscaleLocation, m_Scale, transform, fontSh);
		}
	}
}
