#include"DroppedItems.h"
#include"math/matrix.h"
#include"math/VectorOperation.h"
#include"Collision.h"


#define PickUpTime 4
#define FlyDistance 6.0f


DroppedItem::DroppedItem(float x, float y, int looksOnWhatCoordinates, unsigned short int item, unsigned short int amount, bool canPlayerPickUpIt)
	:m_Transform{ x,y }, m_Velocity{looksOnWhatCoordinates * FlyDistance, 4},m_Item(item),m_Amount(amount), m_Timer(0), m_BlockY(0), m_BlockIndex(-1),m_GoToPlayer(false)
{
	if (canPlayerPickUpIt)
	{
		m_Timer = 4;
	}
}
bool DroppedItem::EveryFrame(float deltaTime, std::vector<std::vector<Block>>& blocks, float* playerTransform, bool doesPlayerHaveSpace)
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

	if (m_Timer >= PickUpTime)
	{
		if (doesPlayerHaveSpace && distanceToPlayer < 5)
		{
			if (distanceToPlayer < 1.5f)
			{
				return true;
			}
			m_GoToPlayer = true;
		}
		else
		{
			m_GoToPlayer = false;
			m_Timer = PickUpTime;
		}
	}
	if (m_GoToPlayer)
	{
		m_Velocity[0] += playerTransform[0] - m_Transform[0];
		m_Velocity[1] += playerTransform[1] - m_Transform[1];
		NormalizeVector(m_Velocity);
		m_Velocity[0] *= 25 * (m_Timer - PickUpTime) / distanceToPlayer;
		m_Velocity[1] *= 25 * (m_Timer - PickUpTime) / distanceToPlayer;
	}
	else if (m_Velocity[0] == 0 && m_Velocity[0] <= 0)
	{
		ItemHitBox( *(this), blocks, itemVertices, deltaTime);
	}
	else
	{
		bool floorHit = false;
		unsigned char behavior =  DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, itemVertices, blocks,hit, hit, floorHit, hit);
		
		int goingWay = PlusMinusOneOrZero(m_Velocity[0]);

		switch (behavior)
		{
		case(b_Slippery):
			m_Velocity[0] -= deltaTime * FlyDistance * 1.5f * goingWay;
			break;

		case(b_Asphalt):
			m_Velocity[0] -= deltaTime * FlyDistance * 3 * goingWay;
			break;

		case(b_Air):
			m_Velocity[0] -= deltaTime * FlyDistance * goingWay;
			break;

		default:
			m_Velocity[0] -= deltaTime * FlyDistance * 2 * goingWay;
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
void DroppedItem::DrawItem(unsigned int* textureIDs, Shader& basicSh, unsigned int transformLocation, float* transform)
{
	
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicSh.SetUniformMat4(transformLocation, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, textureIDs[m_Item]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}

void ItemHitBox(DroppedItem& dropItem, std::vector<std::vector<Block>>& hitbox, float* itemVertices, float deltaTime)
{
	int x = roundf(dropItem.m_Transform[0]);
	int hitboxVertices[2] = { roundf(itemVertices[3]) , roundf(dropItem.m_Velocity[1] * deltaTime + itemVertices[3]) };
	if (dropItem.m_BlockIndex != -1 && hitbox.at(x).at(dropItem.m_BlockIndex).m_Transform[0] == dropItem.m_BlockY)
	{
		return;
	}
	float closestVertice;
	bool hit = false;
	for (int i = 0; i < hitbox.at(x).size(); i++)
	{
		float blockTop = hitbox.at(x).at(i).m_Transform[1] + 0.5f;

		float y = hitbox.at(x).at(i).m_Transform[1];

		if (y <= hitboxVertices[0] && y >= hitboxVertices[1])
		{
			if (hit)
			{
				if (y + 0.5f > closestVertice)
				{
					dropItem.m_BlockY = hitbox.at(x).at(i).m_Transform[1];
					dropItem.m_BlockIndex = i;
					closestVertice = y + 0.5f;
				}
			}
			else
			{
				dropItem.m_BlockY = hitbox.at(x).at(i).m_Transform[1];
				dropItem.m_BlockIndex = i;
				closestVertice = y + 0.5f;
				hit = true;
			}
		}
	}
	if (hit)
	{
		dropItem.m_Velocity[1] = 0;
		dropItem.m_Transform[1] += closestVertice - itemVertices[3];

	}
}