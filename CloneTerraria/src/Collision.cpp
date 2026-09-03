#include"Collision.h"

#include<iostream>
#include<math.h>

#include"glfw/input.h"
#include"math/VectorOperation.h"
#include"glfw/Window.h"
#include"ItemList.h"


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
			if (vertices[i] >= Blocks::xMax)
			{
				vertices[i] = Blocks::xMax-1;
			}
			else if (vertices[i] < Blocks::xMin)
			{
				vertices[i] = Blocks::xMin;
			}
		}
		else
		{
			if (vertices[i] >= Blocks::yMax)
			{
				vertices[i] = Blocks::yMax-1;
			}
			else if (vertices[i] < Blocks::yMin)
			{
				vertices[i] = Blocks::yMin;
			}
		}
	}
}


bool AddVelocityToTransform(float* objectVertices4f
	, float* transform
	, float* velocity
	, bool* hit
	, float deltaTime)
{
	objectVertices4f[0] -= transform[0]; 
	objectVertices4f[2] -= transform[0];
	transform[0] += velocity[0]* deltaTime;
	objectVertices4f[0] += transform[0]; 
	objectVertices4f[2] += transform[0]; 

	objectVertices4f[1] -= transform[1];
	objectVertices4f[3] -= transform[1];
	transform[1] += velocity[1] * deltaTime;
	objectVertices4f[1] += transform[1];
	objectVertices4f[3] += transform[1];


	if (objectVertices4f[2] > Blocks::xMax - SAFEDISTANCE )
	{
		transform[0] -= objectVertices4f[2] - Blocks::xMax + SAFEDISTANCE;
		velocity[0] = 0;
		hit[2] = true;
	}
	else if (objectVertices4f[0] < Blocks::xMin + SAFEDISTANCE)
	{
		transform[0] -= objectVertices4f[0]- Blocks::xMin - SAFEDISTANCE;
		velocity[0] = 0;
		hit[0] = true;
	}
	if (objectVertices4f[1] > Blocks::yMax - SAFEDISTANCE)
	{
		transform[1] -= objectVertices4f[1] - Blocks::yMax + SAFEDISTANCE;
		velocity[1] = 0;
		hit[1] = true;
	}
	else if (objectVertices4f[3] < Blocks::yMin + SAFEDISTANCE)
	{
		transform[1] -= objectVertices4f[3] - Blocks::yMin - SAFEDISTANCE;
		velocity[1] = 0;
		hit[3] = true;
		return true;

	}
	return false;
}

bool RayVsRect(float* rectPos
	, float* rectSides
	, float* rayPos
	, float* rayDir
	, float* contactPoint
	, float* normal
	, float& hitTime)
{
	float invDir[2] = { 1.0f/rayDir[0],  1.0f/rayDir[1]};
	float x[2] = {(rectPos[0] + rectSides[0] - rayPos[0]) * invDir[0] ,(rectPos[0]+ rectSides[2] - rayPos[0])* invDir[0]};
	float y[2] = {(rectPos[1] + rectSides[3] - rayPos[1]) * invDir[1] ,(rectPos[1]+ rectSides[1] - rayPos[1])* invDir[1]};
	if (std::isnan(y[1]) || std::isnan(x[1]) || std::isnan(y[0]) || std::isnan(x[0])) return false;

	if(x[1] < x[0])std::swap(x[0],x[1]);
	if(y[1] < y[0])std::swap(y[0],y[1]);
	if(x[0] > y[1] || y[0] > x[1])return false;
	hitTime = std::max(x[0],y[0]);
	float hitTimeFar = std::min(y[1],x[1]);
	
	if(hitTimeFar < 0) return false;


	contactPoint[0] = rayPos[0] + rayDir[0] * hitTime; 
	contactPoint[1] = rayPos[1] + rayDir[1] * hitTime;

	if(x[0] > y[0])
	{
		if(invDir[0] > 0)
		{
			normal[0] = -1;
		}
		else if(invDir[0] < 0)
		{
			normal[0] = 1;
		}
		normal[1] = 0;
	}
	else if((x[0] < y[0]))
	{
		if(invDir[1] > 0)
		{
			normal[1] = -1;
		}
		else if(invDir[1] < 0)
		{
			normal[1] = 1;
		}
		normal[0] = 0;
	}
	return true;
}
bool rectDymVsBlock(float deltatime
	, float* rectpos
	, float* rectSides
	, float* rectVelocity
	, int* blockPos 
	, float* contactNormal
	, float* contactPoint
	, float& hitTime)
{
	float blockSides[4] = {-0.5f-rectSides[2],0.5f-rectSides[3],0.5f-rectSides[0],-0.5f-rectSides[1]};
 	float blockPos2f[2] = {(float)blockPos[0],(float)blockPos[1]};
	float velocity[2] = {rectVelocity[0] * deltatime,rectVelocity[1] * deltatime};
	
	if(RayVsRect(blockPos2f,blockSides,rectpos,velocity, contactPoint,contactNormal,hitTime)) 
	return (hitTime >= 0.0f && hitTime < 1.0f);
	return false;
}
struct contact
{
	int pos[2] ={};
	float time = 0;
	contact(int* bPos,float hitTime)
	{
		time = hitTime;
		pos[0] = bPos[0]; 
		pos[1] = bPos[1]; 
	}
};
unsigned char DynamicHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* relVertices
	, bool platformControl
	, bool platformIgnore
	, std::vector<std::vector<Block>>& blocks
	, bool* hit)
{
	unsigned char behavior = b_Air;
	if(velocity[0] == 0 && velocity[1] == 0) return b_Air;
	int workSpace[4] = {};
	if(velocity[1] > 0)
	{
		workSpace[1] = RoundFiveDown(transform[1]+relVertices[1]+velocity[1]*deltaTime)+2;
		workSpace[3] = RoundFiveUp(transform[1]+relVertices[3]) - 1;
	}
	else
	{
		workSpace[1] = round(transform[1]+relVertices[1])+2;
		workSpace[3] = round(transform[1]+relVertices[3]+velocity[1]*deltaTime) - 1;
	}
	if(velocity[1] > 0)
	{
		workSpace[2] = round(transform[0]+relVertices[2]+velocity[0]*deltaTime)+2;
		workSpace[0] = round(transform[0]+relVertices[0]) - 1;
	}
	else
	{
		workSpace[2] = round(transform[0]+relVertices[2])+2;
		workSpace[0] = round(transform[0]+relVertices[0]+velocity[0]*deltaTime) - 1;
	}
	memoryDefender(workSpace,4);
	workSpace[1] -= Blocks::yMin;
	workSpace[3] -= Blocks::yMin;
	float rectpos[2] = {transform[0],transform[1] - Blocks::yMin}; 
	int blockPos[2] = {};
	std::vector<contact> contacts;
	for(blockPos[0] = workSpace[0]; blockPos[0] < workSpace[2];blockPos[0]++)
	{
		for(blockPos[1] = workSpace[3]; blockPos[1] < workSpace[1];blockPos[1]++)
		{
			if (blocks.at(blockPos[0]).at(blockPos[1]).m_Behavior == b_Air)
			{
				continue;
			}
			float contactPoint[2] ={};
			float contactNormal[2] = {};
			float hitTime = 0;
			if(rectDymVsBlock(deltaTime,rectpos,relVertices,velocity,blockPos,contactNormal,contactPoint,hitTime))
			{
				if(contactNormal[1] == 1 && (blocks.at(blockPos[0]).at(blockPos[1]).m_Behavior > behavior))
					behavior = blocks.at(blockPos[0]).at(blockPos[1]).m_Behavior;
				int indexToPlace = 0;
				for (indexToPlace = 0; indexToPlace < contacts.size(); indexToPlace++)
				{
					if (contacts.at(indexToPlace).time > hitTime)
					{
						break;
					}
				}
				contacts.emplace(contacts.begin()+ indexToPlace,blockPos,hitTime);
			}
		
		}
	}
	
	for(int i = 0; i < contacts.size(); i++)
	{
		float contactPoint[2] ={};
		float contactNormal[2] = {};
		float hitTime = 0;
	
		if(rectDymVsBlock(deltaTime,rectpos,relVertices,velocity,contacts.at(i).pos,contactNormal,contactPoint,hitTime))
		{
			//transform[0] = contactPoint[0];
			//transform[1] = contactPoint[1] + Blocks::yMin;
			//rectpos[0] = contactPoint[0];
			//rectpos[1] = contactPoint[1];
			//velocity[0] += contactNormal[0] * std::abs(velocity[0]);
			//velocity[1] += contactNormal[1] * std::abs(velocity[1]);
			
			velocity[0] += contactNormal[0] * std::abs(velocity[0]) * (1-hitTime);
			velocity[1] += contactNormal[1] * std::abs(velocity[1]) * (1-hitTime);
		
			if(contactNormal[0] == 1)hit[0] = true;
			if(contactNormal[1] == -1)hit[1] = true;
			if(contactNormal[0] == -1)hit[2] = true;
			if(contactNormal[1] == 1)hit[3] = true;
				
		}
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



unsigned char CharacterHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* relVertices
	, bool platformControl
	, bool platformIgnore
	, std::vector<std::vector<Block>>& hitbox
	, bool* hit)
{
	float objectVertices4f[4] = {transform[0] + relVertices[0],transform[1] + relVertices[1], transform[0] + relVertices[2],transform[1] + relVertices[3]};
	float  vel[2] = { velocity[0] , velocity[1] };
	unsigned char behavior = DynamicHitbox(deltaTime, transform, velocity,relVertices, platformControl, platformIgnore, hitbox, hit);
	bool autoJump = false;
	if ((hit[0] || hit[1])&& abs(vel[0]) > 9)
	{
		if (hit[3])
		{
			autoJump = true;
		}
		else if (objectVertices4f[3] - floorf(objectVertices4f[3]) == 0.5f)
		{
			for (int i = RoundFiveDown( objectVertices4f[0]); i < RoundFiveDown( objectVertices4f[2]); i++)
			{

				if (hitbox.at(i).at(RoundFiveDown(objectVertices4f[3] - Blocks::yMin)).m_Behavior != b_Air)
				{
					autoJump = true;
				}
				if (hitbox.at(i).at(RoundFiveDown(objectVertices4f[1])+1 - Blocks::yMin).m_Behavior != b_Air)
				{
					autoJump = false;
					break;
				}
			}
		}

	}
	if (autoJump)
	{
		int wallX = 0;
		if (hit)
		{
			wallX = RoundFiveUp( - 1 + objectVertices4f[0]);
		}
		else
		{
			wallX = RoundFiveDown( 1 + objectVertices4f[2]);
		}
		for (int i = RoundFiveUp(objectVertices4f[3]) +1; i < RoundFiveDown(objectVertices4f[1])+1; i++)
		{
			if (hitbox.at(wallX).at(i-Blocks::yMin).m_Behavior != b_Air)
			{
				autoJump = false;
				break;
			}
		}
		if (autoJump)
		{
			velocity[0] = vel[0];
			velocity[1] = 0;
			transform[1]++;
		}
	}
	return behavior;
}
