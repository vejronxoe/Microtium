#include"flora.h"

#include "Opengl/ErrorSystem.h"
#include "math/matrix.h"
#include "math/VectorOperation.h"
#include "glfw/Window.h"
#include "ItemList.h"
#define TIMETOGROW 2 

bool FindSeedling(std::vector<seedling>& seedlings
	, int x
	, int y
	, int& index)
{
	for (int i = 0; i < seedlings.size(); i++)
	{
		if (seedlings.at(i).m_Transform[0] == x && (seedlings.at(i).m_Transform[1] == y || seedlings.at(i).m_Transform[1] + 1 == y))
		{
			index = i;
			return true;
		}
	}
	return false;
}
bool FindSeedling(std::vector<seedling>& seedlings
	, int* vertices)
{
	for (int i = 0; i < seedlings.size(); i++)
	{
		if (vertices[0] <= seedlings.at(i).m_Transform[0] && vertices[2] >= seedlings.at(i).m_Transform[0])
		{
			if ( vertices[3] <= seedlings.at(i).m_Transform[1] + 1 && vertices[1] >= seedlings.at(i).m_Transform[1])
			{
				return true;
			}
		}
	}
	return false;
}


void DrawCrowns(Shader& sh
	, std::vector<Crown>& crowns
	, unsigned int* DDs
	, unsigned int* textures
	, float* cameraCoordinate
	, float* transform
	, float* rotation)
{
	for (int i = 0; i < crowns.size(); i++)
	{
		Crown crown = crowns.at(i);
		if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform + 5) >= crown.m_Transform[0] && floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform - 5) <= crown.m_Transform[0])
		{
			if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform - 5) <= crown.m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform + 5) >= crown.m_Transform[1])
			{
				if (crown.m_Rotation != 0)
				{
					ChangeRotation(crown.m_Rotation, rotation);
					sh.SetUniformMat4(CrownRotation, rotation);
				}
				ErrorGL(glBindVertexArray(DDs[crown.m_Type]));
				ChangeTransform(crown.m_Transform[0], crown.m_Transform[1], transform);
				sh.SetUniformMat4(CrownTransform, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[crown.m_Type]));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				if (crown.m_Rotation != 0)
				{
					ChangeRotation(0, rotation);
					sh.SetUniformMat4(CrownRotation, rotation);
				}
			}
		}
	}
}
Crown::Crown(int16_t x
	, int16_t y
	, int8_t rotation
	, uint8_t type)
{
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Rotation = rotation;
	m_Type = type;
}

void DestroyCrown(int* transform
	, std::vector<DroppedItem>& dropItems
	, std::vector<Crown>& crowns)
{
	for (int i = 0; i < crowns.size(); i++)
	{
		if (crowns.at(i).m_Transform[0] == transform[0] && crowns.at(i).m_Transform[1] == transform[1])
		{
			int8_t type = crowns.at(i).m_Type;
			crowns.at(i) = crowns.at(crowns.size() - 1);
			crowns.pop_back();
			int decider = rand() % 2;
			if (decider)
			{
				switch (type)
				{
				case crown_Snow:
				case crown_SnowSmall:
					dropItems.emplace_back(transform[0], transform[1], 0, i_SnowSapling, 1, true);
					break;
				default:
					dropItems.emplace_back(transform[0], transform[1], 0, i_Sapling, 1, true);
					break;
				}
			}
			break;
		}
	}
}
damagedWood::damagedWood(int x
	, int y
	, int rotation
	, char HP)
:m_Transform{x,y}, m_Rotation(rotation), m_HP(HP)
{}

void damagedWood::DrawCut(Shader& sh
	, float* rotation
	, float* transform
	, unsigned int* texture)
{

	if (m_Rotation != 0)
	{
		ChangeRotation(m_Rotation, rotation);
		sh.SetUniformMat4(CrownRotation, rotation);
	}
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(CrownTransform, transform);
	int index = m_HP / 3;
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[index]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	if (m_Rotation != 0)
	{
		ChangeRotation(0, rotation);
		sh.SetUniformMat4( CrownRotation, rotation);
	}
}


seedling::seedling(char type
	, int x
	, int y
	, unsigned int* structuresTextures
	, std::vector<std::vector<Block>>& blocks)
:m_Transform{x, y}
,m_Timer(0)
,m_Type(type)
,m_Texture(structuresTextures[type])
{
}

void checkCrowns(std::vector<Crown>& Crowns
	, int* objVertices4
	, bool& inBlock)
{
	if (!inBlock)
	{
		for (int i = 0; i < Crowns.size(); i++)
		{

			int vertices[4];
			switch (Crowns.at(i).m_Type)
			{
			case crown_Forest:
			case crown_Snow:

				vertices[0] = Crowns.at(i).m_Transform[0] - 3; vertices[1] = Crowns.at(i).m_Transform[1] + 3;
				vertices[2] = Crowns.at(i).m_Transform[0] + 3; vertices[3] = Crowns.at(i).m_Transform[1];
				if (DoTheyIntersect(objVertices4, vertices))
				{
					inBlock = true;
					return;
				}
				break;
			case crown_ForestSmall:
			case crown_SnowSmall:
				switch (Crowns.at(i).m_Rotation)
				{

				case 90:
					vertices[0] = Crowns.at(i).m_Transform[0] - 2; vertices[1] = Crowns.at(i).m_Transform[1] + 1;
					vertices[2] = Crowns.at(i).m_Transform[0]; vertices[3] = Crowns.at(i).m_Transform[1] - 1;
					break;
				case -90:
					vertices[0] = Crowns.at(i).m_Transform[0]; vertices[1] = Crowns.at(i).m_Transform[1] + 1;
					vertices[2] = Crowns.at(i).m_Transform[0] + 2; vertices[3] = Crowns.at(i).m_Transform[1] - 1;
					break;
				}
				if (DoTheyIntersect(objVertices4, vertices))
				{
					inBlock = true;
					return;
				}
				break;
			}
		}
	}

}

void createBranchs(std::vector<std::vector<Block>>& blocks
	, std::vector<Crown>& crowns
	,std::vector<seedling>& saplings
	, std::vector<int>& chunksToRebuild
	, int* transform 
	, int* vertices
	, int leanght
	, int type
	, int amountBranches)
{
	std::vector<int8_t> heights;
	int8_t size = leanght - 3;
	heights.assign(size, 0);
	
	for (int i = 2; i < leanght-1;i++)
	{
		heights.at(i - 2) = i;
	}
	for (int i = 0 ; i < size;i++)
	{
		int8_t place = rand() % size;
		int8_t hold = heights.at(i);
		heights.at(i) = heights.at(place);
		heights.at(place) = hold;
	}
	for (int i = 0;i < heights.size();i++)
	{
		vertices[1] = transform[1] + heights.at(i) + 1 ;
		vertices[3] = transform[1] + heights.at(i) - 1 ;
		bool inCrown = false;
		checkCrowns(crowns, vertices, inCrown);
		if (FindBlock(blocks, vertices) || FindSeedling(saplings,vertices) || inCrown)
		{
			heights.at(i) = heights.at(heights.size() - 1);
			heights.pop_back();
			i--;
			continue;
		}
		for (int j = i+1 ; j < heights.size(); j++)
		{
			if (abs(heights.at(i) - heights.at(j)) < 3)
			{
				heights.at(j) = heights.at(heights.size() - 1);
				heights.pop_back();
				j--;
			}
		}
	}
	if (amountBranches < heights.size())
	{
		heights.resize(amountBranches);
	}
	std::vector<int> fill;
	int8_t side = (transform[0] - vertices[0])/abs(transform[0] - vertices[0]);
	if (type == s_ForestSapling)
	{
		for (int i = 0; i < heights.size(); i++)
		{
			vertices[1] = transform[1] + heights.at(i) + 1;
			vertices[3] = transform[1] + heights.at(i) - 1;
			int y = heights.at(i) + transform[1];

			CreateBlock(1 * -side + transform[0], y, t_LightWoodHor, chunksToRebuild, blocks, fill);

			crowns.emplace_back(2 * -side + transform[0], y, 90 * side, crown_ForestSmall);
		}
	}
	else
	{
		for (int i = 0; i < heights.size(); i++)
		{
			vertices[1] = transform[1] + heights.at(i) + 1;
			vertices[3] = transform[1] + heights.at(i) - 1;
			int y = heights.at(i) + transform[1];

			CreateBlock(1 * -side + transform[0], y, t_DarkWoodHor, chunksToRebuild, blocks, fill);

			crowns.emplace_back(2 * -side + transform[0], y, 90 * side, crown_SnowSmall);
		}
	}
}

bool seedling::everyFrame(float deltaTime 
	, std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<Crown>& Crowns)
{
	bool noGround = true;
	if (blocks.at(m_Transform[0]).at(m_Transform[1] - 1  - Blocks::yMin).m_Behavior != b_Air)
	{
		noGround = false;
	}
	m_Timer += deltaTime;
	if (m_Timer > TIMETOGROW && !noGround)
	{	
		if (m_Type ==  s_CactusSapling)
		{
			int leangth = (rand() % 9) + 3;
			int vertices[4];
			vertices[0] = m_Transform[0]; vertices[1] = m_Transform[1] + leangth;
			vertices[2] = m_Transform[0]; vertices[3] = m_Transform[1];
			
			bool inBlock = (Blocks::yMax <= vertices[1] || Blocks::xMax <= vertices[2] || Blocks::xMin >= vertices[0]);

			if (!inBlock)
			{
				inBlock = FindBlock(blocks, vertices);
			}
			checkCrowns(Crowns, vertices, inBlock);
			if (inBlock)
			{
				m_Timer = 0;
			}
			else
			{
				std::vector<int> fill;

				blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin).m_Behavior = b_Indestructible;
				for (int i = 0; i < leangth; i++)
				{
					CreateBlock(m_Transform[0], m_Transform[1] + i, t_CactusVer, chunksToRebuild, blocks, fill);
				}
				CreateBlock(m_Transform[0], m_Transform[1] +leangth, t_CactusTop, chunksToRebuild, blocks, fill);
				return true;

			}

		}
		else
		{
			int leangth = (rand() % 16) + 6;
			int crownVertices[4];
			crownVertices[0] = m_Transform[0] - 3; crownVertices[1] = m_Transform[1] + leangth + 3;
			crownVertices[2] = m_Transform[0] + 3; crownVertices[3] = m_Transform[1] + leangth;
			int vertices[4];
			vertices[0] = m_Transform[0]; vertices[1] = m_Transform[1] + leangth;
			vertices[2] = m_Transform[0]; vertices[3] = m_Transform[1];
			bool inBlock = (Blocks::yMax <= crownVertices[1] || Blocks::xMax <= crownVertices[2] || Blocks::xMin >= crownVertices[0]);

			if (!inBlock)
			{
				inBlock = FindBlock(blocks, vertices);
			}
			checkCrowns(Crowns, vertices, inBlock);
			checkCrowns(Crowns, crownVertices, inBlock);
			if (!inBlock)
			{
				inBlock = FindBlock(blocks, crownVertices);
			}
			if (inBlock)
			{
				m_Timer = 0;
			}
			else
			{
				blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin).m_Behavior = b_Indestructible;
				int typeOfWood = t_LightWoodVer;
				if (m_Type == s_ForestSapling)
				{
					Crowns.emplace_back(m_Transform[0], m_Transform[1] + leangth, 0, crown_Forest);
				}
				else
				{
					Crowns.emplace_back(m_Transform[0], m_Transform[1] + leangth, 0, crown_Snow);
					typeOfWood = t_DarkWoodVer;
				}
				for (int i = 0; i < leangth; i++)
				{
					std::vector<int> fill;
					CreateBlock(m_Transform[0], m_Transform[1] + i, typeOfWood, chunksToRebuild, blocks, fill);
				}
				
				int branchsMaxCount = floor(leangth / 4);
				unsigned int leftBranchs = rand() % branchsMaxCount;
				unsigned int rightBranchs = rand() % branchsMaxCount;

				int branchVertices[4];

				if (Blocks::xMin < m_Transform[0] - 5 && leftBranchs != 0)
				{
					branchVertices[0] = m_Transform[0] - 4;
					branchVertices[2] = m_Transform[0] - 1;

					createBranchs(blocks, Crowns, seedlings, chunksToRebuild, m_Transform, branchVertices, leangth, m_Type, leftBranchs);
				}

				if (Blocks::xMax > m_Transform[0] + 5 && rightBranchs != 0)
				{
					branchVertices[0] = m_Transform[0] + 1;
					branchVertices[2] = m_Transform[0] + 4;

					createBranchs(blocks, Crowns, seedlings, chunksToRebuild, m_Transform, branchVertices, leangth, m_Type, rightBranchs);
				}
				return true;

			}
		}
	}
	return noGround;
}
void seedling::drawSeedling(Shader sh
	, float* Transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], Transform);
	sh.SetUniformMat4(basicTransform, Transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}