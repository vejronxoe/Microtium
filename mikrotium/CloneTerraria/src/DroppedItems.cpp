#include"DroppedItems.h"
#include"math/matrix.h"
#include"math/VectorOperation.h"
#include"Collision.h"


#define PICKUPTIME 4
#define FLYDISTANCE 6.0f


DroppedItem::DroppedItem(float x
	, float y
	, int looksOnWhatCoordinates
	, unsigned short int item
	, unsigned short int amount
	, bool canPlayerPickUpIt)
	:m_Transform{ x,y }, m_Velocity{looksOnWhatCoordinates * FLYDISTANCE, 4},m_Item(item),m_Amount(amount), m_Timer(0), m_BlockY(0), m_BlockIndex(-1),m_GoToPlayer(false)
{
	if (canPlayerPickUpIt)
	{
		m_Timer = 4;
	}
}
bool DroppedItem::EveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, std::vector<DroppedItem>& dropItems
	, float* playerTransform
	, bool doesPlayerHaveSpace)
{
	float itemVertices[4] = { m_Transform[0] - 0.45f ,m_Transform[1] + 0.45f,m_Transform[0] + 0.45f,m_Transform[1] - 0.45f };

	bool hit = false;
	m_Timer += deltaTime;
	m_Velocity[1] -= deltaTime * 10;
	if (m_Velocity[1] <= -5)
	{
		m_Velocity[1] = -5;
	}
	float distanceToPlayer = Pyt2D(playerTransform[0] - m_Transform[0], playerTransform[1] - m_Transform[1]);

	for (int i = 0; i < dropItems.size(); i++)
	{
		if (2 > Pyt2D(m_Transform[0] - dropItems.at(i).m_Transform[0], dropItems.at(i).m_Transform[1] - m_Transform[1]) && dropItems.at(i).m_Item == m_Item && &(dropItems.at(i)) != this)
		{
			m_Amount += dropItems.at(i).m_Amount;
			dropItems.erase(dropItems.begin() + i);
		}
	}

	if (m_Timer >= PICKUPTIME)
	{
		if (doesPlayerHaveSpace && distanceToPlayer < 5)
		{
			if (distanceToPlayer < 2)
			{
				return true;
			}
			m_GoToPlayer = true;
		}
		else
		{
			m_GoToPlayer = false;
			m_Timer = PICKUPTIME;
		}
	}
	if (m_GoToPlayer)
	{
		m_Velocity[0] = playerTransform[0] - m_Transform[0];
		m_Velocity[1] = playerTransform[1] - m_Transform[1];
		if (Pyt2D(m_Velocity) > 1)
		{
			NormalizeVector(m_Velocity);
		}
		m_Velocity[0] *= 40 * (m_Timer - PICKUPTIME);
		m_Velocity[1] *= 40 * (m_Timer - PICKUPTIME);
	}
	else
	{
	
		bool floorHit = false;
		unsigned char behavior =  DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, itemVertices, blocks,hit, hit, floorHit, hit);
		
		int goingWay = PlusMinusOneOrZero(m_Velocity[0]);

		switch (behavior)
		{
		case(b_Slippery):
			m_Velocity[0] -= deltaTime * FLYDISTANCE * 1.5f * goingWay;
			break;

		case(b_Asphalt):
			m_Velocity[0] -= deltaTime * FLYDISTANCE * 3 * goingWay;
			break;

		case(b_Air):
			m_Velocity[0] -= deltaTime * FLYDISTANCE * goingWay;
			break;

		default:
			m_Velocity[0] -= deltaTime * FLYDISTANCE * 2 * goingWay;
			break;
		}
		if (0 >= m_Velocity[0] * goingWay)
		{
			m_Velocity[0] = 0;
		}

	}
	AddVelocityToTransform(itemVertices, m_Transform, m_Velocity, hit, deltaTime);
	return false;
}
void DroppedItem::DrawItem(unsigned int* textureIDs
	, Shader& basicSh
	, unsigned int transformLocation
	, float* transform)
{
	
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicSh.SetUniformMat4(transformLocation, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, textureIDs[m_Item]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}

