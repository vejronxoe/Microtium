#include"Collision.h"

#include<iostream>
#include<math.h>

#include"glfw/input.h"
#include"glfw/Window.h"

void GetCorners(int x , int y , float* hitboxsCorners )
{
	hitboxsCorners[0] = x - 0.5f;
	hitboxsCorners[1] = y + 0.5f; 
	hitboxsCorners[2] = x + 0.5f; 
	hitboxsCorners[3] = y - 0.5f;
}
void memoryDefender(float* verticesOfPredicts, int lenghtOfArray)
{
	for (int i = 0; i < lenghtOfArray; i++)
	{
		if (i % 2 == 0)
		{
			if (verticesOfPredicts[i] > Blocks::xMax)
			{
				verticesOfPredicts[i] = Blocks::xMax;
			}
			else if (verticesOfPredicts[i] < Blocks::xMin)
			{
				verticesOfPredicts[i] = Blocks::xMin;
			}
		}
		else
		{
			if (verticesOfPredicts[i] > Blocks::yMax)
			{
				verticesOfPredicts[i] = Blocks::yMax;
			}
			else if (verticesOfPredicts[i] < Blocks::yMin)
			{
				verticesOfPredicts[i] = Blocks::yMin;
			}
		}
	}
}
unsigned int OneDirectionCheck(float* vertices, float* objectVertices4f, std::vector<std::vector<Block>>& hitbox, const int blockIndex, const int playerIndex, float& transform, float& velocity, float& closestVertice, bool& sideHit)
{
	unsigned int behavior = b_Air;
	for (int i = 0; i < 4; i++)
	{
			vertices[i] = std::roundf(vertices[i]);
	}
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < hitbox.at(j).size(); i++)
		{
			float blockTop = hitbox.at(j).at(i).m_Transform[1] + 0.5f;
			if (hitbox.at(j).at(i).m_BlockBehavior != b_Platform || hitbox.at(j).at(i).m_BlockBehavior == b_Platform && (objectVertices4f[3] > blockTop || objectVertices4f[3] == blockTop && !Input::SHold))
			{
				float x = hitbox.at(j).at(i).m_Transform[0];
				float y = hitbox.at(j).at(i).m_Transform[1];

				if (y <= vertices[1] && y >= vertices[3])
				{
					float corners[4];
					GetCorners(hitbox.at(j).at(i).m_Transform[0], hitbox.at(j).at(i).m_Transform[1], corners);
					if (sideHit)
					{
						if (abs(closestVertice - objectVertices4f[playerIndex]) > abs(corners[blockIndex] - objectVertices4f[playerIndex]))
						{
							closestVertice = corners[blockIndex];
							behavior = hitbox.at(j).at(i).m_BlockBehavior;
						}
					}
					else
					{
						closestVertice = corners[blockIndex];
						sideHit = true;
						behavior = hitbox.at(j).at(i).m_BlockBehavior;
					}
				}
			}

		}
	}
	if (sideHit)
	{
		velocity = 0;
		transform += closestVertice - objectVertices4f[playerIndex];
		if (behavior == b_Platform)
			return b_BasicSolid;
		return behavior;
	}
	return b_Air;
}
unsigned int TwoDirectionCheck(float* vertices, float* objectVertices4f, std::vector< std::vector<Block>>& hitbox, const int XHitboxIndex, const int YHitboxIndex, const int XplayerIndex, const int YplayerIndex, float* transform, float* velocity, float* closestVertice, bool& floorHit, bool& wallHit)
{
	bool edgehit = false;
	float edgeVertice[2];
	unsigned int behavior = b_Air;
	unsigned int edgeBehavior = b_Air;
	vertices[0] = std::roundf(vertices[0]);
	int maxXNeeded = vertices[0];
	int minXNeeded = vertices[0];
	for (int i = 1; i < 6; i++)
	{
			vertices[i * 2] = std::roundf(vertices[i * 2]);
			if (vertices[i * 2] > maxXNeeded)
			{
				maxXNeeded = vertices[i * 2];
			}
			if (vertices[i * 2] < minXNeeded)
			{
				minXNeeded = vertices[i * 2];
			}
	}
	for (int i = 0; i < 6; i++)
	{
		vertices[i * 2 + 1] = std::roundf(vertices[i * 2 + 1]);
	}
	for (int j = minXNeeded; j <= maxXNeeded; j++)
	{
		for (int i = 0; i < hitbox.at(j).size(); i++)
		{
			float blockTop = hitbox.at(j).at(i).m_Transform[1] + 0.5f;
			if ( hitbox.at(j).at(i).m_BlockBehavior != b_Platform || hitbox.at(j).at(i).m_BlockBehavior == b_Platform && (objectVertices4f[3] > blockTop || objectVertices4f[3] == blockTop && !Input::SHold))
			{
				bool xHit = false;
				bool yHit = false;
				float x = hitbox.at(j).at(i).m_Transform[0];
				float y = hitbox.at(j).at(i).m_Transform[1];

				if (x >= vertices[0] && y <= vertices[1] && x <= vertices[2] && y >= vertices[3])
				{
					float corners[4];
					GetCorners(hitbox.at(j).at(i).m_Transform[0], hitbox.at(j).at(i).m_Transform[1], corners);
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
					GetCorners(hitbox.at(j).at(i).m_Transform[0], hitbox.at(j).at(i).m_Transform[1], corners);
					if (floorHit)
					{
						if (abs(closestVertice[1] - objectVertices4f[YplayerIndex]) > abs(corners[YHitboxIndex] - objectVertices4f[YplayerIndex]))
						{
							closestVertice[1] = corners[YHitboxIndex];
							behavior = hitbox.at(j).at(i).m_BlockBehavior;
						}
					}
					else
					{
						closestVertice[1] = corners[YHitboxIndex];
						floorHit = true;
						behavior = hitbox.at(j).at(i).m_BlockBehavior;
					}
					yHit = true;
				}
				if (!yHit && !xHit)
				{
					if (x >= vertices[8] && y <= vertices[9] && x <= vertices[10] && y >= vertices[11])
					{
						float corners[4];
						GetCorners(hitbox.at(j).at(i).m_Transform[0], hitbox.at(j).at(i).m_Transform[1], corners);
						if (edgehit)
						{

							if (abs(edgeVertice[0] - objectVertices4f[XplayerIndex]) > abs(closestVertice[XHitboxIndex] - objectVertices4f[XplayerIndex]))
							{
								edgeVertice[0] = corners[XHitboxIndex];
							}
							if (abs(edgeVertice[1] - objectVertices4f[YplayerIndex]) > abs(closestVertice[YHitboxIndex] - objectVertices4f[YplayerIndex]))
							{
								edgeVertice[1] = corners[YHitboxIndex];
								edgeBehavior = hitbox.at(j).at(i).m_BlockBehavior;
							}
						}
						else
						{
							edgeVertice[1] = corners[YHitboxIndex];
							edgeVertice[0] = corners[XHitboxIndex];
							edgeBehavior = hitbox.at(j).at(i).m_BlockBehavior;
							edgehit = true;
						}
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
			if (edgeBehavior == b_Platform)
				return b_BasicSolid;
			return edgeBehavior;
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
			if (behavior == b_Platform)
				return b_BasicSolid;
			return behavior;
		}
	}
	return b_Air;
}
bool AddVelocityToTransform(float* objectVertices4f, float* transform, float* velocity, bool& floorHit, float deltaTime)
{
	objectVertices4f[0] -= transform[0]; objectVertices4f[1] -= transform[1];
	objectVertices4f[2] -= transform[0]; objectVertices4f[3] -= transform[1];
	transform[0] += velocity[0] * deltaTime;
	transform[1] += velocity[1] * deltaTime;
	objectVertices4f[0] += transform[0]; objectVertices4f[1] += transform[1];
	objectVertices4f[2] += transform[0]; objectVertices4f[3] += transform[1];


	if (objectVertices4f[2] >= Blocks::xMax - SAFEDISTANCE)
	{
		transform[0] -= objectVertices4f[2] - Blocks::xMax + SAFEDISTANCE;
		velocity[0] = 0;
	}
	else if (objectVertices4f[0] <= Blocks::xMin + SAFEDISTANCE)
	{
		transform[0] -= objectVertices4f[0]- Blocks::xMin - SAFEDISTANCE;
		velocity[0] = 0;

	}
	if (objectVertices4f[1] >= Blocks::yMax - SAFEDISTANCE)
	{
		transform[1] -= objectVertices4f[1] - Blocks::yMax + SAFEDISTANCE;
		velocity[1] = 0;

	}
	else if (objectVertices4f[3] <= Blocks::yMin + SAFEDISTANCE)
	{
		transform[1] -= objectVertices4f[3] - Blocks::yMin - SAFEDISTANCE;
		velocity[1] = 0;
		floorHit = true;
		return true;

	}
	return false;
}

unsigned char DynamicSquereHitbox(float deltaTime, float* transform, float* velocity, float* objectVertices4f, std::vector<std::vector<Block>>& hitbox, bool& leftWallHit, bool& rightWallHit, bool& floorHit, bool& ceilHit)
{
	if (velocity[0] == 0 && velocity[1] == 0)
		return b_Air;
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
		memoryDefender(vertices, 12);
		unsigned int behavior = TwoDirectionCheck(vertices, objectVertices4f, hitbox, 0, 3, 2, 1, transform, velocity, closestVertices, ceilHit, rightWallHit);
	}
	else if (velocity[0] < 0 && velocity[1] < 0)
	{

		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[3];
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[3] + velocity[1] * deltaTime;
		vertices[8] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[9] = objectVertices4f[3];
		vertices[10] = objectVertices4f[0]; vertices[11] = objectVertices4f[3] + velocity[1] * deltaTime;
		memoryDefender(vertices, 12);
		unsigned int behavior = TwoDirectionCheck(vertices, objectVertices4f, hitbox, 2, 1, 0, 3, transform, velocity, closestVertices, floorHit, leftWallHit);
		if(b_Indestructible == behavior)
		{ 
			return b_BasicSolid;
		}
		return behavior;
	}
	else if (velocity[0] < 0 && velocity[1] > 0)
	{
		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[1];
		vertices[8] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[9] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[10] = objectVertices4f[0]; vertices[11] = objectVertices4f[1];
		memoryDefender(vertices, 12);
		unsigned int behavior = TwoDirectionCheck(vertices, objectVertices4f, hitbox, 2, 3, 0, 1, transform, velocity, closestVertices, ceilHit, leftWallHit);
	}
	else if (velocity[0] > 0 && velocity[1] < 0)
	{
		vertices[0] = objectVertices4f[2]; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[3] = objectVertices4f[3] + 0.1f;
		vertices[4] = objectVertices4f[0] + 0.1f; vertices[5] = objectVertices4f[3];
		vertices[6] = objectVertices4f[2] - 0.1f; vertices[7] = objectVertices4f[3] + velocity[1] * deltaTime;
		vertices[8] = objectVertices4f[2]; vertices[9] = objectVertices4f[3];
		vertices[10] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[11] = objectVertices4f[3] + velocity[1] * deltaTime;
		memoryDefender(vertices, 12);
		unsigned int behavior = TwoDirectionCheck(vertices, objectVertices4f, hitbox, 0, 1, 2, 3, transform, velocity, closestVertices, floorHit, rightWallHit);
		if (b_Indestructible == behavior)
		{
			return b_BasicSolid;
		}
		return behavior;

	}
	else if (velocity[1] > 0)
	{
		vertices[0] = objectVertices4f[0] + 0.1f; vertices[1] = objectVertices4f[1] + velocity[1] * deltaTime;
		vertices[2] = objectVertices4f[2] - 0.1f; vertices[3] = objectVertices4f[1];
		unsigned int behavior = OneDirectionCheck(vertices, objectVertices4f, hitbox, 3, 1, transform[1], velocity[1], closestVertices[0], ceilHit);
		memoryDefender(vertices, 4);
	}
	else if (velocity[1] < 0)
	{
		vertices[0] = objectVertices4f[0] + 0.1f; vertices[1] = objectVertices4f[3];
		vertices[2] = objectVertices4f[2] - 0.1f; vertices[3] = objectVertices4f[3] + velocity[1] * deltaTime;
		unsigned int behavior = OneDirectionCheck(vertices, objectVertices4f, hitbox, 1, 3, transform[1], velocity[1], closestVertices[0], floorHit);
		memoryDefender(vertices, 4);
		return behavior;
	}
	else if (velocity[0] > 0)
	{
		vertices[0] = objectVertices4f[2]; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[2] + velocity[0] * deltaTime; vertices[3] = objectVertices4f[3] + 0.1f;
		unsigned int behavior = OneDirectionCheck(vertices, objectVertices4f, hitbox, 0, 2, transform[0], velocity[0], closestVertices[0], leftWallHit);
		memoryDefender(vertices, 4);
	}
	else if(velocity[0] < 0)
	{
		vertices[0] = objectVertices4f[0] + velocity[0] * deltaTime; vertices[1] = objectVertices4f[1] - 0.1f;
		vertices[2] = objectVertices4f[0]; vertices[3] = objectVertices4f[3] + 0.1f;
		unsigned int behavior = OneDirectionCheck(vertices, objectVertices4f, hitbox, 2, 0, transform[0], velocity[0], closestVertices[0], rightWallHit);
		memoryDefender(vertices, 4);
	}
	return b_Air;
}
float CameraHitboxX(float x)
{
	float topX = x + Window::halfWidthOfGameTransform;
	float downX = x - Window::halfWidthOfGameTransform;
	if (topX > Blocks::xMax - SAFEDISTANCE)
	{
		x -= topX - Blocks::xMax + SAFEDISTANCE;
	}
	else if (downX < Blocks::xMin + SAFEDISTANCE)
	{
		x -= downX - Blocks::xMin - SAFEDISTANCE;
	}
	return x;
}
float CameraHitboxY(float y)
{
	float topY = y + Window::halfHeightOfGameTransform;
	float downY = y - Window::halfHeightOfGameTransform;
	if (topY > Blocks::yMax - SAFEDISTANCE)
	{
		y -= topY - Blocks::yMax + SAFEDISTANCE;
	}
	else if (downY < Blocks::yMin + SAFEDISTANCE)
	{
		y -= downY - Blocks::yMin - SAFEDISTANCE;
	}
	return y;
}
