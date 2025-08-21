#include"Collision.h"

#include<iostream>
#include<math.h>

#include"glfw/input.h"

void GetCorners(int x , int y , float* hitboxsCorners )
{
	hitboxsCorners[0] = x - 0.5f;
	hitboxsCorners[1] = y + 0.5f; 
	hitboxsCorners[2] = x + 0.5f; 
	hitboxsCorners[3] = y - 0.5f;
}

void OneDirectionCheck(float* vertices, float* objectVertices4f, std::vector<Block>& hitbox, const int blockIndex, const int playerIndex, float& transform, float& velocity, float& closestVertice, bool& sideHit)
{
	for (int i = 0; i < 4; i++)
	{
			vertices[i] = std::roundf(vertices[i]);
	}
	for (int i = 0; i < hitbox.size(); i++)
	{ 
		if (hitbox.at(i).m_CollisionActive && hitbox.at(i).m_BlockBehavior != platform || hitbox.at(i).m_BlockBehavior == platform && objectVertices4f[3] >= hitbox.at(i).m_Transform[1] + 0.5f && !Input::SHold)
		{
			float x = hitbox.at(i).m_Transform[0];
			float y = hitbox.at(i).m_Transform[1];

			if (x >= vertices[0] && y <= vertices[1] && x <= vertices[2] && y >= vertices[3])
			{
				float corners[4];
				GetCorners(hitbox.at(i).m_Transform[0], hitbox.at(i).m_Transform[1], corners);
				if (sideHit)
				{
					if (abs(closestVertice - objectVertices4f[playerIndex]) > abs(corners[blockIndex] - objectVertices4f[playerIndex]))
					{
						closestVertice = corners[blockIndex];
					}
				}
				else
				{
					closestVertice = corners[blockIndex];
					sideHit = true;
				}
			}
		}
	}
	if (sideHit)
	{
		velocity = 0;
		transform += closestVertice - objectVertices4f[playerIndex];
	}
}
void TwoDirectionCheck(float* vertices, float* objectVertices4f, std::vector<Block>& hitbox, const int XHitboxIndex, const int YHitboxIndex, const int XplayerIndex, const int YplayerIndex, float* transform, float* velocity, float* closestVertice, bool& floorHit, bool& wallHit)
{
	bool edgehit = false;
	float edgeVertice[2];
	for (int i = 0; i < 12; i++)
	{
		vertices[i] = std::roundf(vertices[i]);
	}
	for (int i = 0; i < hitbox.size(); i++)
	{
		if (hitbox.at(i).m_CollisionActive && hitbox.at(i).m_BlockBehavior != platform || hitbox.at(i).m_BlockBehavior == platform && objectVertices4f[3] >= hitbox.at(i).m_Transform[1] + 0.5f && !Input::SHold)
		{
			bool xHit = false;
			bool yHit = false;
			float x = hitbox.at(i).m_Transform[0];
			float y = hitbox.at(i).m_Transform[1];

			if (x >= vertices[0] && y <= vertices[1] && x <= vertices[2] && y >= vertices[3])
			{
				float corners[4];
				GetCorners(hitbox.at(i).m_Transform[0], hitbox.at(i).m_Transform[1], corners);
				if (wallHit)
				{
					if (abs(closestVertice[0] - objectVertices4f[XplayerIndex]) > abs(corners[XHitboxIndex] - objectVertices4f[XplayerIndex]))
					{
						closestVertice[0] = corners[XHitboxIndex];
					}
				}
				else
				{
					closestVertice[0] = corners[XHitboxIndex];
					wallHit = true;
				}
				xHit = true;
			}


			if (x >= vertices[4] && y <= vertices[5] && x <= vertices[6] && y >= vertices[7])
			{
				float corners[4];
				GetCorners(hitbox.at(i).m_Transform[0], hitbox.at(i).m_Transform[1], corners);
				if (floorHit)
				{
					if (abs(closestVertice[1] - objectVertices4f[YplayerIndex]) > abs(corners[YHitboxIndex] - objectVertices4f[YplayerIndex]))
					{
						closestVertice[1] = corners[YHitboxIndex];
					}
				}
				else
				{
					closestVertice[1] = corners[YHitboxIndex];
					floorHit = true;
				}
				yHit = true;
			}
			if (!yHit && !xHit)
			{
				if (x >= vertices[8] && y <= vertices[9] && x <= vertices[10] && y >= vertices[11])
				{
					float corners[4];
					GetCorners(hitbox.at(i).m_Transform[0], hitbox.at(i).m_Transform[1], corners);
					if (edgehit)
					{

						if (abs(edgeVertice[0] - objectVertices4f[XplayerIndex]) > abs(closestVertice[XHitboxIndex] - objectVertices4f[XplayerIndex]))
						{
							edgeVertice[0] = corners[XHitboxIndex];
						}
						if (abs(edgeVertice[1] - objectVertices4f[YplayerIndex]) > abs(closestVertice[YHitboxIndex] - objectVertices4f[YplayerIndex]))
						{
							edgeVertice[1] = corners[YHitboxIndex];
						}
					}
					else
					{
						edgeVertice[1] = corners[YHitboxIndex];
						edgeVertice[0] = corners[XHitboxIndex];
						edgehit = true;
					}
				}
			}
		}
	}
	if ((edgehit && !floorHit && !wallHit) || (edgehit && wallHit && floorHit && abs(edgeVertice[1] - objectVertices4f[YplayerIndex]) < abs(closestVertice[YHitboxIndex] - objectVertices4f[YplayerIndex])))
	{
		if (abs(velocity[0]) < abs(velocity[1]))
		{
			velocity[0] = 0;
			transform[0] += edgeVertice[0] - objectVertices4f[XplayerIndex];
			wallHit = true;
		}
		else
		{

			velocity[1] = 0;
			transform[1] += edgeVertice[1] - objectVertices4f[YplayerIndex];
		}
	}
	else
	{
		if (wallHit)
		{
			velocity[0] = 0;
			transform[0] += closestVertice[0] - objectVertices4f[XplayerIndex];
		}
		if (floorHit)
		{
			velocity[1] = 0;
			transform[1] += closestVertice[1] - objectVertices4f[YplayerIndex];
		}
	}
}

void DynamicSquereHitbox(float deltaTime, float* transform, float* velocity, float* objectVertices4f, std::vector<Block>& hitbox, bool& leftWallHit, bool& rightWallHit, bool& floorHit, bool& ceilHit)
{
	if (velocity[0] == 0 && velocity[1] == 0)
		return;
	float closestVertices[2];
	float vertices[12];
	if (velocity[0] > 0 && velocity[1] > 0)
	{
		vertices[0] = objectVertices4f[2]; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[1];
		vertices[8] = objectVertices4f[2]; vertices[9] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[10] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[11] = objectVertices4f[1];
		TwoDirectionCheck(vertices, objectVertices4f, hitbox, 0, 3, 2, 1, transform, velocity, closestVertices, ceilHit, rightWallHit);
	}
	else if (velocity[0] < 0 && velocity[1] < 0)
	{

		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[3];
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[3] + velocity[1] * deltaTime;
		vertices[8] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[9] = objectVertices4f[3];
		vertices[10] = objectVertices4f[0]; vertices[11] = objectVertices4f[3] + velocity[1] * deltaTime;
		TwoDirectionCheck(vertices, objectVertices4f, hitbox, 2, 1, 0, 3, transform, velocity, closestVertices, floorHit, leftWallHit);
	}
	else if (velocity[0] < 0 && velocity[1] > 0)
	{
		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[1];
		vertices[8] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[9] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[10] = objectVertices4f[0]; vertices[11] = objectVertices4f[1];
		TwoDirectionCheck(vertices, objectVertices4f, hitbox, 2, 3, 0, 1, transform, velocity, closestVertices, ceilHit, leftWallHit);
	}
	else if (velocity[0] > 0 && velocity[1] < 0)
	{
		vertices[0] = objectVertices4f[2]; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[3];
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[3] + velocity[1] * deltaTime;
		vertices[8] = objectVertices4f[2]; vertices[9] = objectVertices4f[3];
		vertices[10] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[11] = objectVertices4f[3] + velocity[1] * deltaTime;
		TwoDirectionCheck(vertices, objectVertices4f, hitbox, 0, 1, 2, 3, transform, velocity, closestVertices, floorHit, rightWallHit);

	}
	else if (velocity[1] > 0)
	{
		vertices[0] = objectVertices4f[0] + 0.1f; vertices[1] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[2] = objectVertices4f[2] - 0.1f; vertices[3] = objectVertices4f[1];
		OneDirectionCheck(vertices, objectVertices4f, hitbox, 3, 1, transform[1], velocity[1], closestVertices[0], ceilHit);
	}
	else if (velocity[1] < 0)
	{
		vertices[0] = objectVertices4f[0] + 0.1f; vertices[1] = objectVertices4f[3];
		vertices[2] = objectVertices4f[2] - 0.1f; vertices[3] = objectVertices4f[3] + velocity[1] * deltaTime;
		OneDirectionCheck(vertices, objectVertices4f, hitbox, 1, 3, transform[1], velocity[1], closestVertices[0], floorHit);
	}
	else if (velocity[0] > 0)
	{
		vertices[0] = objectVertices4f[2]; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[3] = objectVertices4f[3] + 0.1f;
		OneDirectionCheck(vertices, objectVertices4f, hitbox, 0, 2, transform[0], velocity[0], closestVertices[0], leftWallHit);
	}
	else if(velocity[0] < 0)
	{
		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		OneDirectionCheck(vertices, objectVertices4f, hitbox, 2, 0, transform[0], velocity[0], closestVertices[0], rightWallHit);
	}
	
}
