#include"Collision.h"

#include<iostream>
#include<math.h>

#include"glfw/input.h"
#include"glfw/Window.h"

bool FindClosestBlockInArea(std::vector<std::vector<Block>>& blocks
	, float* objVertices
	, int* vertices
	, int index
	, int& behavior
	,int& closest)
{
	bool count = false;
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < blocks.at(j).size(); i++)
		{
			if (blocks.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (blocks.at(j).at(i).m_Y <= vertices[1])
			{
				if (blocks.at(j).at(i).m_BlockBehavior != b_Platform || blocks.at(j).at(i).m_BlockBehavior == b_Platform && (objVertices[3] > blocks.at(j).at(i).m_Y + 0.5f || objVertices[3] == blocks.at(j).at(i).m_Y + 0.5f && !Input::SHold))
				{
					int chooser[4] = { j, blocks.at(j).at(i).m_Y, j, blocks.at(j).at(i).m_Y };
					if (count)
					{
						if (abs(closest - vertices[index]) > abs(chooser[index] - vertices[index]))
						{
							closest = chooser[index];
							if (behavior < blocks.at(j).at(i).m_BlockBehavior)
							{
								behavior = blocks.at(j).at(i).m_BlockBehavior;
							}
						}
						else if (abs(closest - vertices[index]) == abs(chooser[index] - vertices[index]))
						{
							if (behavior < blocks.at(j).at(i).m_BlockBehavior)
							{
								behavior = blocks.at(j).at(i).m_BlockBehavior;
							}
						}
					}
					else
					{
						closest = chooser[index];
						behavior = blocks.at(j).at(i).m_BlockBehavior;
						count = true;
					}
				}
			}

		}
	}
	return count;
}

int RoundFiveUp(float x)
{

	float decider = x - floorf(x);
	if (decider == 0.5f)
	{
		return ceilf(x);

	}
	else
	{
		return roundf(x);
	}

}

int RoundFiveDown(float x)
{
	float decider = x - floor(x);
	if (decider == 0.5f)
	{
		return floorf(x);
		
	}
	else
	{
		return roundf(x);
	}
}

void memoryDefender(int* vertices
	, int lenghtOfArray)
{
	for (int i = 0; i < lenghtOfArray; i++)
	{
		if (i % 2 == 0)
		{
			if (vertices[i] > Blocks::xMax)
			{
				vertices[i] = Blocks::xMax;
			}
			else if (vertices[i] < Blocks::xMin)
			{
				vertices[i] = Blocks::xMin;
			}
		}
		else
		{
			if (vertices[i] > Blocks::yMax)
			{
				vertices[i] = Blocks::yMax;
			}
			else if (vertices[i] < Blocks::yMin)
			{
				vertices[i] = Blocks::yMin;
			}
		}
	}
}
unsigned int OneDirectionCheck(int* vertices
	, float* objectVertices4f
	, std::vector<std::vector<Block>>& hitbox
	, int blockIndex
	, int playerIndex
	, float& transform
	, float& velocity
	, float& oldVelocity
	, bool& sideHit)
{
	int closest;
	int behavior = b_Air;
	sideHit = FindClosestBlockInArea(hitbox, objectVertices4f, vertices, blockIndex, behavior, closest);
	if (sideHit)
	{
		float couner[4] = { -0.5, 0.5, 0.5, -0.5 };
		velocity = 0;
		oldVelocity = 0;
		transform += closest + couner[blockIndex] - objectVertices4f[playerIndex];
	
	}
	return behavior;
}
unsigned int TwoDirectionCheck(std::vector< std::vector<Block>>& hitbox
	, int* verticesX
	, int* verticesY
	, int* verticesEdge
	, float* objectVertices4f
	, int XHitboxIndex
	, int YHitboxIndex
	, int XplayerIndex
	, int YplayerIndex
	, float* transform
	, float* velocity
	, float* oldVelocity
	, bool& floorHit
	, bool& wallHit)
{
	int closest[4];
	bool edgehit[2];
	int behavior[2];
	wallHit = FindClosestBlockInArea(hitbox, objectVertices4f, verticesX, XHitboxIndex, behavior[0], closest[0]);
	floorHit = FindClosestBlockInArea(hitbox, objectVertices4f, verticesY, YHitboxIndex, behavior[0], closest[1]);
	edgehit[0] = FindClosestBlockInArea(hitbox, objectVertices4f, verticesEdge, XHitboxIndex, behavior[1], closest[2]);
	edgehit[1] = FindClosestBlockInArea(hitbox, objectVertices4f, verticesEdge, YHitboxIndex, behavior[1], closest[3]);
	float couner[4] = { -0.5, 0.5, 0.5, -0.5 };

	if (!wallHit && !floorHit && (edgehit[0] || edgehit[1]))
	{
		if (abs(velocity[0]) < abs(velocity[1]) && edgehit[1] || edgehit[1] && !edgehit[0])
		{
			oldVelocity[1] = 0;
			velocity[1] = 0;
			transform[1] += closest[2] + couner[XHitboxIndex] - objectVertices4f[XplayerIndex];
			wallHit = true;
		}
		else
		{
			oldVelocity[0] = 0;
			velocity[0] = 0;
			transform[0] += closest[3] + couner[YHitboxIndex] - objectVertices4f[YplayerIndex];
			floorHit = true;
			return behavior[1];
		}
	}
	else if(wallHit && floorHit && edgehit[1] && edgehit[0] && abs(closest[0] - objectVertices4f[XplayerIndex]) > abs(closest[2] - objectVertices4f[XplayerIndex]) && abs(closest[1] - objectVertices4f[YplayerIndex]) > abs(closest[3] - objectVertices4f[YplayerIndex]))
	{
		if ( edgehit[0])
		{
			oldVelocity[0] = 0;
			velocity[0] = 0;
			transform[0] += closest[3] + couner[YHitboxIndex] - objectVertices4f[YplayerIndex];
			floorHit = true;
			return behavior[1];
		}
		else
		{
			oldVelocity[1] = 0;
			velocity[1] = 0;
			transform[1] += closest[2] + couner[XHitboxIndex] - objectVertices4f[XplayerIndex];
			wallHit = true;
		}
		
	}
	else
	{
		
		if (wallHit)
		{
			oldVelocity[0] = 0;
			velocity[0] = 0;
			transform[0] += closest[0] + couner[XHitboxIndex] - objectVertices4f[XplayerIndex];
		}
		if (floorHit)
		{
			oldVelocity[1] = 0;
			velocity[1] = 0;
			transform[1] += closest[1] + couner[YHitboxIndex] - objectVertices4f[YplayerIndex];
		}
	}
	return behavior[0];
}
bool AddVelocityToTransform(float* objectVertices4f
	, float* transform
	, float* velocity
	, float* oldVelocity
	, bool& floorHit
	, bool& rightHit
	, bool& leftHit
	, bool& ceilHit
	, float deltaTime)
{
	objectVertices4f[0] -= transform[0]; 
	objectVertices4f[2] -= transform[0];
	transform[0] += oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;
	objectVertices4f[0] += transform[0]; 
	objectVertices4f[2] += transform[0]; 

	objectVertices4f[1] -= transform[1];
	objectVertices4f[3] -= transform[1];
	transform[1] += oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;
	objectVertices4f[1] += transform[1];
	objectVertices4f[3] += transform[1];


	if (objectVertices4f[2] >= Blocks::xMax - SAFEDISTANCE && velocity[0] > 0)
	{
		transform[0] -= objectVertices4f[2] - Blocks::xMax + SAFEDISTANCE;
		velocity[0] = 0;
		oldVelocity[0] = 0;
		rightHit = true;
	}
	else if (objectVertices4f[0] <= Blocks::xMin + SAFEDISTANCE && velocity[0] < 0)
	{
		transform[0] -= objectVertices4f[0]- Blocks::xMin - SAFEDISTANCE;
		velocity[0] = 0;
		oldVelocity[0] = 0;
		leftHit = true;
	}
	if (objectVertices4f[1] >= Blocks::yMax - SAFEDISTANCE && velocity[1] > 0)
	{
		transform[1] -= objectVertices4f[1] - Blocks::yMax + SAFEDISTANCE;
		velocity[1] = 0;
		oldVelocity[1] = 0;
		ceilHit = true;
	}
	else if (objectVertices4f[3] <= Blocks::yMin + SAFEDISTANCE && velocity[1] < 0)
	{
		transform[1] -= objectVertices4f[3] - Blocks::yMin - SAFEDISTANCE;
		velocity[1] = 0;
		oldVelocity[1] = 0;
		floorHit = true;
		return true;

	}
	return false;
}

unsigned char DynamicSquereHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* oldVelocity
	, float* objectVertices4f
	, std::vector<std::vector<Block>>& hitbox
	, bool& leftWallHit
	, bool& rightWallHit
	, bool& floorHit
	, bool& ceilHit)

{

	if (velocity[0] == 0 && velocity[1] == 0)
		return b_Air;
	float move[2] = { velocity[0] * deltaTime, velocity[1] * deltaTime };
	int wallVertices[4];
	int floorVertices[4];
	int edgeVertices[4];
	wallVertices[1] = RoundFiveDown(objectVertices4f[1]);
	wallVertices[3] = RoundFiveUp(objectVertices4f[3]);

	if (move[0] > 0)
	{

		wallVertices[0] = RoundFiveUp(objectVertices4f[2]);
		wallVertices[2] = RoundFiveDown(objectVertices4f[2] + move[0]);

		edgeVertices[0] = RoundFiveUp(objectVertices4f[2]);
		edgeVertices[2] = RoundFiveDown(objectVertices4f[2] + move[0]);
	}
	else
	{
		wallVertices[0] = RoundFiveUp(objectVertices4f[0] + move[0]);
		wallVertices[2] = RoundFiveDown(objectVertices4f[0]);

		edgeVertices[0] = RoundFiveUp(objectVertices4f[0]);
		edgeVertices[2] = RoundFiveDown(objectVertices4f[0] + move[0]);
	}

	floorVertices[0] = RoundFiveUp(objectVertices4f[0]);
	floorVertices[2] = RoundFiveDown(objectVertices4f[2]);
	if (move[1] > 0)
	{
		floorVertices[1] = RoundFiveDown(objectVertices4f[1] + move[1]);
		floorVertices[3] = RoundFiveUp(objectVertices4f[1]);
		edgeVertices[1] = RoundFiveDown(objectVertices4f[1] + move[1]);
		edgeVertices[3] = RoundFiveUp(objectVertices4f[1]);
	}
	else
	{
		floorVertices[1] = RoundFiveDown(objectVertices4f[3]);
		floorVertices[3] = RoundFiveUp(objectVertices4f[3] + move[1]);
		edgeVertices[1] = RoundFiveDown(objectVertices4f[3]);
		edgeVertices[3] = RoundFiveUp(objectVertices4f[3] + move[1]);
	}
	memoryDefender(wallVertices, 4);
	memoryDefender(floorVertices, 4);
	memoryDefender(edgeVertices, 4);
	unsigned int behavior = b_Air;
	if (velocity[0] > 0 && velocity[1] > 0)
	{
		TwoDirectionCheck(hitbox, wallVertices, floorVertices, edgeVertices, objectVertices4f, 0, 3, 2, 1, transform, velocity, oldVelocity, ceilHit, rightWallHit);
	}
	else if (velocity[0] < 0 && velocity[1] < 0)
	{
		behavior = TwoDirectionCheck(hitbox, wallVertices, floorVertices, edgeVertices, objectVertices4f, 2, 1, 0, 3, transform, velocity, oldVelocity, floorHit, leftWallHit);
		return behavior;
	}
	else if (velocity[0] < 0 && velocity[1] > 0)
	{
		TwoDirectionCheck(hitbox, wallVertices, floorVertices, edgeVertices, objectVertices4f, 2, 3, 0, 1, transform, velocity, oldVelocity, ceilHit, leftWallHit);
	}
	else if (velocity[0] > 0 && velocity[1] < 0)
	{
		behavior = TwoDirectionCheck(hitbox, wallVertices, floorVertices, edgeVertices, objectVertices4f, 0, 1, 2, 3, transform, velocity, oldVelocity, floorHit, rightWallHit);
	

		return behavior;

	}
	else if (velocity[1] > 0)
	{

		OneDirectionCheck(floorVertices, objectVertices4f, hitbox, 3, 1, transform[1], velocity[1], oldVelocity[1], ceilHit);

	}
	else if (velocity[1] < 0)
	{
		behavior = OneDirectionCheck(floorVertices, objectVertices4f, hitbox, 1, 3, transform[1], velocity[1], oldVelocity[1], floorHit);
	}
	else if (velocity[0] > 0)
	{
		OneDirectionCheck(wallVertices, objectVertices4f, hitbox, 0, 2, transform[0], velocity[0], oldVelocity[0], leftWallHit);
	}
	else if (velocity[0] < 0)
	{

		OneDirectionCheck(wallVertices, objectVertices4f, hitbox, 2, 0, transform[0], velocity[0], oldVelocity[0], rightWallHit);

	}

	return behavior;
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
