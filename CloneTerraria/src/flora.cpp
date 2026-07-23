#include"flora.h"

#include "Opengl/ErrorSystem.h"
#include "math/matrix.h"
#include "glfw/Window.h"
#include "ItemList.h"
#define TIMETOGROW 10

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

void checkTreesWithCrowns(std::vector<Crown>& trees
	, int* objVertices4
	, bool& inBlock)
{
	if (!inBlock)
	{
		for (int i = 0; i < trees.size(); i++)
		{

			float vertices[4];
			switch (trees.at(i).m_Type)
			{
			case crown_Forest:
			case crown_Snow:

				vertices[0] = trees.at(i).m_Transform[0] - 3; vertices[1] = trees.at(i).m_Transform[1] + 3;
				vertices[2] = trees.at(i).m_Transform[0] + 3; vertices[3] = trees.at(i).m_Transform[1];
				if ( vertices[1] >= objVertices4[3] && vertices[3] <= objVertices4[1])
				{
					if (vertices[2] >= objVertices4[0] && vertices[0] <= objVertices4[2])
					{
						inBlock = true;
						break;
					}
				} 
				break;
			case crown_ForestSmall:
			case crown_SnowSmall:
				switch (trees.at(i).m_Rotation)
				{
				case 0:
					vertices[0] = trees.at(i).m_Transform[0] - 1; vertices[1] = trees.at(i).m_Transform[1] + 2;
					vertices[2] = trees.at(i).m_Transform[0] + 1; vertices[3] = trees.at(i).m_Transform[1];
					break;
				case 90:
					vertices[0] = trees.at(i).m_Transform[0] - 2; vertices[1] = trees.at(i).m_Transform[1] + 1;
					vertices[2] = trees.at(i).m_Transform[0]; vertices[3] = trees.at(i).m_Transform[1] - 1;
					break;
				case -90:
					vertices[0] = trees.at(i).m_Transform[0]; vertices[1] = trees.at(i).m_Transform[1] + 1;
					vertices[2] = trees.at(i).m_Transform[0] + 2; vertices[3] = trees.at(i).m_Transform[1] - 1;
					break;
				default:
					vertices[0] = 0;
					vertices[1] = 0;
					vertices[2] = 0;
					vertices[3] = 0;
					printf("error in sapling everyFrame wrong Rotation : %d\n", trees.at(i).m_Rotation);
					break;
				}
				if (vertices[1] >= objVertices4[3] && vertices[3] <= objVertices4[1])
				{
					if (vertices[2] >= objVertices4[0] && vertices[0] <= objVertices4[2])
					{
						inBlock = true;
						break;
					}
				}

				break;
			}
		}
	}

}

void DrawTrees(Shader& sh
	, std::vector<Crown> crowns
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
					sh.SetUniformMat4(treeRotation, rotation);
				}
				ErrorGL(glBindVertexArray(DDs[crown.m_Type]));
				ChangeTransform(crown.m_Transform[0], crown.m_Transform[1], transform);
				sh.SetUniformMat4(treeTransform, transform);
				ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[crown.m_Type]));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				if (crown.m_Rotation != 0)
				{
					ChangeRotation(0, rotation);
					sh.SetUniformMat4(treeRotation, rotation);
				}
			}
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
		sh.SetUniformMat4(treeRotation, rotation);
	}
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(treeTransform, transform);
	int index = m_HP / 3;
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[index]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	if (m_Rotation != 0)
	{
		ChangeRotation(0, rotation);
		sh.SetUniformMat4( treeRotation, rotation);
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

bool seedling::everyFrame(float deltaTime
	, unsigned int* treeTextures
	, unsigned int* treeDD
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees)
{

	bool noGround = true;
	if (blocks.at(m_Transform[0]).at(m_Transform[1] - 1  - Blocks::yMin).m_Behavior != b_Air)
	{
		noGround = false;
	}
	m_Timer += deltaTime;
	if (m_Timer > TIMETOGROW && !noGround)
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
		if (!inBlock)
		{
			checkTreesWithCrowns(trees, vertices, inBlock);
		}
		if (!inBlock)
		{
			checkTreesWithCrowns(trees, crownVertices, inBlock);
		}
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
			blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin) = b_Indestructible;

			for (int i = m_Transform[1]; i < m_Transform[1] + leangth; i++)
			{
				trees.emplace_back(treeTextures[part_Log], treeDD[part_Log], i_ForestPlank, 35, part_Log, m_Transform[0], i, 0);
			}
			int decider = rand() % 2;
			if (decider)
			{
				decider = i_Sapling;
			}
			else
			{
				decider = i_Nothing;
			}
			trees.emplace_back(treeTextures[part_Crown], treeDD[part_Crown], decider, 35, part_Crown, m_Transform[0], m_Transform[1] + leangth, 0);

			int branchsMaxCount = floor(leangth / 4);
			unsigned int leftBranchs = rand() % branchsMaxCount;
			unsigned int rightBranchs = rand() % branchsMaxCount;

			int branchVertices[4];

			if (Blocks::xMin < m_Transform[0] - 5)
			{
				branchVertices[0] = m_Transform[0] - 4; 
				branchVertices[2] = m_Transform[0] - 1; 
				createBranchs(blocks, trees, seedlings, treeTextures, treeDD, m_Transform, branchVertices, leangth, leftBranchs);
			}

			if (Blocks::xMax > m_Transform[0] + 5)
			{
				branchVertices[0] = m_Transform[0] + 1; 
				branchVertices[2] = m_Transform[0] + 4; 

				createBranchs(blocks, trees, seedlings, treeTextures, treeDD, m_Transform, branchVertices, leangth, rightBranchs);
			}
			return true;
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