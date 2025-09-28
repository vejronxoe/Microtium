#include"Player.h"


#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"glfw/Input.h"
#include"glfw/window.h"
#include"math/matrix.h"
#include"NumberRender.h"

void Player::CreateAllItemsTexture()
{
	m_AllItemTextures[CooperPickaxe] = CreateTextureRGBA("res/textures/cooperPickaxe.png");
	m_AllItemTextures[CooperAxe] = CreateTextureRGBA("res/textures/cooperAxe.png");
	m_AllItemTextures[CooperHammer] = CreateTextureRGBA("res/textures/cooperHammer.png");
	m_AllItemTextures[CooperSword] = CreateTextureRGBA("res/textures/cooperSword.png");
}

Player::Player(unsigned int eob, unsigned int HUDTransformLocatin, unsigned int HUDScaleLocatin, float& yLocationOfFirstSlot, float& xLocationOfFirstSlot)
	:m_PlayerDrawData(0), m_Tex(0), m_FloorHit(false), m_CeilHit(false), m_WallHit(false), m_CoyoteTimer(0), m_JumpTimer(0), m_CanJump(false), m_JumpPower(20), m_Gravity(-60.0f), m_Acceleration(25.0f), m_Friction(30), m_MaxMovementSpeed(10), m_Velocity{ 0,0 }, m_Transform{ 0, 0 }
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
		m_PlayerSlots[i] = Nothing;
	}
	for (int i = 0; i < 52; i++)
	{
		m_AmountInSlots[i] = 0;
	}
	m_PlayerSlots[1] = CooperSword;
	m_PlayerSlots[2] = CooperPickaxe;
	m_PlayerSlots[3] = CooperAxe;
	m_AmountInSlots[1] = 1;
	m_AmountInSlots[2] = 1;
	m_AmountInSlots[3] = 1;
	m_CooldownToUse = 0;
	m_TypeOfItem = 0;
	m_PickaxeStreanght = 0;
	m_AxeSteanght = 0;
	m_HammerStreanght = 0;
	m_Range = 0;
	m_Damage = 0;
	m_Placeable = 0;
	m_AimingAtSlot = 0;
	

	m_Tex = CreateTextureRGBA("res/textures/player0.png");
	CreateAllItemsTexture();
	
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
	m_CooldownToUse = 1;
	m_TypeOfItem = Blocks;
	m_PickaxeStreanght = 0;
	m_AxeSteanght = 0;
	m_HammerStreanght = 0;
	m_Range = 5;
	m_Damage = 0;
	m_Placeable = true;
	switch (m_PlayerSlots[0])
	{


	case(CooperPickaxe):
		m_PickaxeStreanght = 1;
			m_Damage = 1;
		break;
	}


}
bool Player::IsItStackble(unsigned short int item)
{
	bool isItStackble = true;
	switch (item)
	{
	case(CooperPickaxe):
		isItStackble = false;
		break;

	case(CooperAxe):
		isItStackble = false;
		break;

	case(CooperHammer):
		isItStackble = false;
		break;

	case(CooperSword):
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
			if (m_PlayerSlots[i] == Nothing && freeSlot == 0)
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
			if (m_PlayerSlots[i] == Nothing)
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
void Player::MovementEveryFrame(float deltaTime, std::vector<Block>& blocks)
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

	if (Input::SpacePress && m_CanJump && !m_CeilHit)
	{
		m_Velocity[1] += m_JumpPower;
		m_CanJump = false;
		m_JumpTimer += deltaTime;
	}
	else if (Input::SpaceHold && m_JumpTimer > 0 && m_JumpTimer < 0.25f && !m_CeilHit)
	{
		m_Velocity[1] += (-m_JumpPower / 2) * deltaTime;
		m_JumpTimer += deltaTime;
	}
	else
	{
		m_Velocity[1] += m_Gravity * deltaTime;
		m_JumpTimer = 0;
	}

	m_FloorHit = false;
	m_CeilHit = false;
	m_WallHit = false;
	float hitboxvertices[4] = { -1.0f + m_Transform[0],1.5f + m_Transform[1],1.0f + m_Transform[0],-1.5f + m_Transform[1] };

	unsigned char moveBehavior = DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, hitboxvertices, blocks, m_WallHit, m_WallHit, m_FloorHit, m_CeilHit);
	switch (moveBehavior)
	{
	case(air):
		m_Acceleration = 5.0f;
		m_Friction = 5;
		m_MaxMovementSpeed = 12;
		break;
	case(slippery):
		m_Acceleration = 12.0f;
		m_Friction = 7;
		m_MaxMovementSpeed = 14;
		break;
	case(asphalt):
		m_Acceleration = 20.0f;
		m_Friction = 60;
		m_MaxMovementSpeed = 20;
		break;
	case(basicSolid):
		m_Acceleration = 25.0f;
		m_Friction = 30;
		m_MaxMovementSpeed = 10;
		break;
	}
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

	m_Transform[0] += m_Velocity[0] * deltaTime;
	m_Transform[1] += m_Velocity[1] * deltaTime;
}
void Player::EveryFrame(float deltaTime, std::vector<Block>& blocks)
{
	m_AimingAtSlot = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if ((m_PlayerSlots[0] != Nothing && m_UseSlot != 0) || m_PlayerSlots[i * 10 + j + 1] != Nothing)
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
					m_PlayerSlots[0] = Nothing;
					m_AmountInSlots[0] = 0;
				}
				m_UseSlot = 51;
				if (m_PlayerSlots[0] == Nothing)
				{
					m_PlayerSlots[0] = m_PlayerSlots[m_UseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_UseSlot];
					m_PlayerSlots[m_UseSlot] = Nothing;
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
					m_PlayerSlots[0] = Nothing;
					m_AmountInSlots[0] = 0;
				}
				m_UseSlot = m_AimingAtSlot;
				if (m_PlayerSlots[0] == Nothing)
				{
					m_PlayerSlots[0] = m_PlayerSlots[m_UseSlot];
					m_AmountInSlots[0] = m_AmountInSlots[m_UseSlot];
					m_PlayerSlots[m_UseSlot] = Nothing;
					m_AmountInSlots[m_UseSlot] = 0;
				}
				else if (m_PlayerSlots[m_UseSlot] == Nothing)
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
	if(Input::LeftMousePress)
	{
 		int xM = std::round(Input::XMousePos + m_Transform[0]);
		int yM = std::round(Input::YMousePos + m_Transform[1]);
		for (int i = 0; i < blocks.size(); i++)
		{
			if (blocks.at(i).m_Transform[0] == xM  && blocks.at(i).m_Transform[1] == yM)
			{
				blocks.erase(blocks.begin() + i);
			}
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
				if (m_PlayerSlots[m_UseSlot] != Nothing)
				{
					ItermGetToInventory(m_AmountInSlots[0], m_PlayerSlots[0]);
				}
				else
				{
					m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
					m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];
					
				}
				m_PlayerSlots[0] = Nothing;
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
				if (m_PlayerSlots[m_UseSlot] != Nothing)
				{
					ItermGetToInventory(m_AmountInSlots[0], m_PlayerSlots[0]);
				}
				else
				{
					m_PlayerSlots[m_UseSlot] = m_PlayerSlots[0];
					m_AmountInSlots[m_UseSlot] = m_AmountInSlots[0];

				}
				m_PlayerSlots[0] = Nothing;
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
				
				if (m_PlayerSlots[(i * 10) + (j + 1)] != Nothing)
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
		if (m_PlayerSlots[51] != Nothing)
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
			if (m_PlayerSlots[i + 1] != Nothing)
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
