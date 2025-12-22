#include"flora.h"

#include "Opengl/ErrorSystem.h"
#include "math/matrix.h"
#include "glfw/Window.h"
#include "ItemList.h"
#define TIMETOGROW 10

void createBranchs(unsigned int* treeTextures
	, unsigned int* treeDD
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<int> order
	, int* m_Transform
	, unsigned int& leftBranchs
	, bool& inBlock)
{
	while (leftBranchs != 0 && order.size())
	{
		inBlock = false;
		for (int j = m_Transform[0] - 4; j < m_Transform[0]; j++)
		{
			for (int i = 0; i < blocks.at(j).size(); i++)
			{
				if (blocks.at(j).at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 1
					&& blocks.at(j).at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1
					&& blocks.at(j).at(i).m_Transform[0] != m_Transform[0] - 1)
				{
					inBlock = true;
					break;
				}
				else if (blocks.at(j).at(i).m_Transform[0] == m_Transform[0] - 1 && blocks.at(j).at(i).m_Transform[1] == m_Transform[1] + order.at(0))
				{
					inBlock = true;
					break;
				}
			}
		}
		for (int i = 0; i < trees.size(); i++)
		{
			if (trees.at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 1 && trees.at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && trees.at(i).m_Transform[0] >= m_Transform[0] - 4 && trees.at(i).m_Transform[0] < m_Transform[0])
			{
				inBlock = true;
				break;
			}
			else if (trees.at(i).m_Transform[0] == m_Transform[0] - 1 && trees.at(i).m_Transform[1] == m_Transform[1] + order.at(0))
			{
				inBlock = true;
				break;
			}
		}
		for (int i = 0; i < seedlings.size(); i++)
		{
			if (seedlings.at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 2 && seedlings.at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && seedlings.at(i).m_Transform[0] >= m_Transform[0] - 4 && seedlings.at(i).m_Transform[0] < m_Transform[0])
			{
				inBlock = true;
				break;
			}
			else if (seedlings.at(i).m_Transform[0] == m_Transform[0] - 1 && seedlings.at(i).m_Transform[1] == m_Transform[1] + order.at(0))
			{
				inBlock = true;
				break;
			}
		}
		if (inBlock)
		{
			order.erase(order.begin());
		}
		else
		{
			int decider = rand() % 3;
			if (decider)
			{
				decider = i_Sapling;
			}
			else
			{
				decider = i_Nothing;
			}
			trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, m_Transform[0] - 1, m_Transform[1] + order.at(0), 90.0f);
			trees.emplace_back(treeTextures[p_SmallCrown], treeDD[p_SmallCrown], decider, 35, p_SmallCrown, m_Transform[0] - 2, m_Transform[1] + order.at(0), 90.0f);
			leftBranchs--;
			order.erase(order.begin());
		}

	}
}

void tree::drawTree(Shader& sh
	, unsigned int transformLocation
	, unsigned int rotateLocation
	, float* cameraCoordinate
	, float* transform
	, float* rotation)
{
	if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform) >= m_Transform[0] && floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform) <= m_Transform[0])
	{
		if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= m_Transform[1])
		{
			if (m_Rotation != 0)
			{
				ChangeRotation(m_Rotation, rotation);
				sh.SetUniformMat4(rotateLocation, rotation);
			}
			ErrorGL(glBindVertexArray(m_DrawData));
			ChangeTransform(m_Transform[0], m_Transform[1], transform);
			sh.SetUniformMat4(transformLocation, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_texture));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			if (m_Rotation != 0)
			{
				ChangeRotation(0, rotation);
				sh.SetUniformMat4(rotateLocation, rotation);
			}
		}
	}
}
tree::tree(unsigned int texture
	, unsigned int drawData
	, unsigned short int itemDrop
	, unsigned char hardness
	, char partOfTree
	, int x
	, int y
	, int rotation)
	: m_Transform{ x, y }
	, m_PartOfTree(partOfTree)
	, m_ItemDrop(itemDrop)
	, m_Hardness(hardness)
	, m_texture(texture)
	, m_DrawData(drawData)
	, m_Rotation(rotation)
{}
damagedWood::damagedWood(int x
	, int y
	, int rotation
	, char HP)
:m_Transform{x,y}, m_Rotation(rotation), m_HP(HP)
{}

void damagedWood::DrawCut(Shader& sh
	, unsigned int transformLocation
	, unsigned int rotationLocation
	, float* rotation
	, float* transform
	, unsigned int* texture)
{

	if (m_Rotation != 0)
	{
		ChangeRotation(m_Rotation, rotation);
		sh.SetUniformMat4(rotationLocation, rotation);
	}
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(transformLocation, transform);
	int index = m_HP / 3;
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[index]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	if (m_Rotation != 0)
	{
		ChangeRotation(0, rotation);
		sh.SetUniformMat4(rotationLocation, rotation);
	}
}


seedling::seedling(char type
	, int x
	, int y
	, unsigned int* structuresTextures)
	:m_Transform{x, y}
	,m_Timer(0)
	,m_Type(type)
	,m_Texture(structuresTextures[type])
{}
bool seedling::everyFrame(float deltaTime
	, unsigned int* treeTextures
	, unsigned int* treeDD
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings 
	, std::vector<tree>& trees)
{
	m_Timer += deltaTime;
	if (m_Timer > TIMETOGROW)
	{
		int leangth = (rand() % 16) + 6;
		bool inBlock = false;
		for (int i = 0; i < blocks.at(m_Transform[0]).size(); i++)
		{
			if (blocks.at(m_Transform[0]).at(i).m_Transform[1] >= m_Transform[1] && blocks.at(m_Transform[0]).at(i).m_Transform[1] <= m_Transform[1] + leangth)
			{
				inBlock = true;
				break;
			}
		}
		for (int i = 0; i < trees.size(); i++)
		{
			float vectices[4];
			switch (trees.at(i).m_PartOfTree)
			{
			
			
			case p_Log:
				if (trees.at(i).m_Transform[1] >= m_Transform[1] && trees.at(i).m_Transform[1] <= m_Transform[1] + leangth && trees.at(i).m_Transform[0] == m_Transform[0])
				{
					inBlock = true;
					break;
				}
			break;
			case p_Crown:

				vectices[0] = trees.at(i).m_Transform[0] - 3; vectices[1] = trees.at(i).m_Transform[1] + 3;
				vectices[2] = trees.at(i).m_Transform[0] + 3; vectices[3] = trees.at(i).m_Transform[1];
				if(vectices[1] >= (m_Transform[1] + leangth) && vectices[3] <= (m_Transform[1] + leangth) || vectices[1] >= m_Transform[1] && vectices[3] <= m_Transform[1])
				{
					if (vectices[2] >= m_Transform[0] && vectices[0] <= m_Transform[0])
					{
						inBlock = true;
						break;
					}
				}
			break;
			case p_SmallCrown:
				switch (trees.at(i).m_Rotation)
				{
				case 0:
					vectices[0] = trees.at(i).m_Transform[0] - 1; vectices[1] = trees.at(i).m_Transform[1] + 2;
					vectices[2] = trees.at(i).m_Transform[0] + 1; vectices[3] = trees.at(i).m_Transform[1];
				break;
				case 90:
					vectices[0] = trees.at(i).m_Transform[0] - 2; vectices[1] = trees.at(i).m_Transform[1] + 1;
					vectices[2] = trees.at(i).m_Transform[0]; vectices[3] = trees.at(i).m_Transform[1] - 1;
				break;
				case -90:
					vectices[0] = trees.at(i).m_Transform[0]; vectices[1] = trees.at(i).m_Transform[1] + 1;
					vectices[2] = trees.at(i).m_Transform[0] + 2; vectices[3] = trees.at(i).m_Transform[1] - 1;
				break;
				default:
					printf("error in sapling everyFrame wrong Rotation : %d", trees.at(i).m_Rotation);
				break;
				}
				if (vectices[1] >= (m_Transform[1] + leangth) && vectices[3] <= (m_Transform[1] + leangth) || vectices[1] >= m_Transform[1] && vectices[3] <= m_Transform[1])
				{
					if (vectices[2] >= m_Transform[0] && vectices[0] <= m_Transform[0])
					{
						inBlock = true;
						break;
					}
				}

			break;
			}
			
		}

		float crownVectices[4];
		crownVectices[0] = m_Transform[0] - 3; crownVectices[1] = m_Transform[1] + leangth + 3;
		crownVectices[2] = m_Transform[0] + 3; crownVectices[3] = m_Transform[1] + leangth;
		for (int j = crownVectices[0]; j < crownVectices[2] + 1; j++)
		{
			if (inBlock)
			{
				break;
			}
			for (int i = 0; i < blocks.at(j).size(); i++)
			{
				if (crownVectices[1] >= blocks.at(j).at(i).m_Transform[1] && crownVectices[3] <= blocks.at(j).at(i).m_Transform[1])
				{
					inBlock = true;
					break;
				}
			}
		
		}
		for (int i = 0; i < trees.size(); i++)
		{
			if (inBlock)
			{
				break;
			}
			float vectices[4];
			switch (trees.at(i).m_PartOfTree)
			{


			case p_Log:
				if (crownVectices[1] >= trees.at(i).m_Transform[1] && crownVectices[3] <= trees.at(i).m_Transform[1] && crownVectices[2] >= trees.at(i).m_Transform[0] && crownVectices[0] <= trees.at(i).m_Transform[0])
				{
					inBlock = true;
					break;
				}
				break;
			case p_Crown:

				vectices[0] = trees.at(i).m_Transform[0] - 3; vectices[1] = trees.at(i).m_Transform[1] + 3;
				vectices[2] = trees.at(i).m_Transform[0] + 3; vectices[3] = trees.at(i).m_Transform[1];
				if (vectices[1] >= crownVectices[1] && vectices[3] <= crownVectices[1] || vectices[1] >= crownVectices[3] && vectices[3] <= crownVectices[3])
				{
					if (vectices[2] >= crownVectices[0] && vectices[0] <= crownVectices[0] || vectices[0] >= crownVectices[2] && vectices[2] <= crownVectices[2])
					{
						inBlock = true;
						break;
					}
				}
				break;
			case p_SmallCrown:
				switch (trees.at(i).m_Rotation)
				{
				case 0:
					vectices[0] = trees.at(i).m_Transform[0] - 1; vectices[1] = trees.at(i).m_Transform[1] + 2;
					vectices[2] = trees.at(i).m_Transform[0] + 1; vectices[3] = trees.at(i).m_Transform[1];
				break;
				case 90:
					vectices[0] = trees.at(i).m_Transform[0] - 2; vectices[1] = trees.at(i).m_Transform[1] + 1;
					vectices[2] = trees.at(i).m_Transform[0]; vectices[3] = trees.at(i).m_Transform[1] - 1;
				break;
				case -90:
					vectices[0] = trees.at(i).m_Transform[0]; vectices[1] = trees.at(i).m_Transform[1] + 1;
					vectices[2] = trees.at(i).m_Transform[0] + 2; vectices[3] = trees.at(i).m_Transform[1] - 1;
				break;
				default:
					printf("error in sapling everyFrame wrong Rotation : %d", trees.at(i).m_Rotation);
				break;
				}
				if (vectices[1] >= crownVectices[1] && vectices[3] <= crownVectices[1] || vectices[1] >= crownVectices[3] && vectices[3] <= crownVectices[3])
				{
					if (vectices[2] >= crownVectices[0] && vectices[0] <= crownVectices[0] || vectices[2] >= crownVectices[2] && vectices[0] <= crownVectices[2])
					{
						inBlock = true;
						break;
					}
				}

				break;
			}
		}

		if (inBlock)
		{
			m_Timer = 0;
		}
		else
		{
			
			for (int i = m_Transform[1]; i < m_Transform[1] + leangth; i++)
			{
				trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, m_Transform[0], i, 0);
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
			trees.emplace_back(treeTextures[p_Crown], treeDD[p_Crown], decider, 35, p_Crown, m_Transform[0], m_Transform[1] + leangth, 0);

			int branchsMaxCount = floor(leangth / 3);
			unsigned int leftBranchs = rand() % branchsMaxCount;
			unsigned int rightBranchs = rand() % branchsMaxCount;
			
			std::vector<int> order;

			for (int i = 2; i < leangth - 2; i++)
			{
				order.emplace_back(i);
			}
			
			for (int i = 0; i < (leangth - 4); i++)
			{
				int swapNumber = rand() % (leangth - 4);
				int holder = order.at(swapNumber);
				order.at(swapNumber) = order.at(i);
				order.at(i) = holder;
			}
			
			createBranchs(treeTextures, treeDD, blocks, seedlings, trees, order, m_Transform, leftBranchs, inBlock);
			

			for (int i = 0; i < (leangth - 4); i++)
			{
				int swapNumber = rand() % (leangth - 4);
				int holder = order.at(swapNumber);
				order.at(swapNumber) = order.at(i);
				order.at(i) = holder;
			}

			createBranchs(treeTextures, treeDD, blocks, seedlings, trees, order, m_Transform, leftBranchs, inBlock);

			return true;
		}
	}
	return false;
}
void seedling::drawSeedling(Shader sh
	, unsigned int transformLocation
	, float* Transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], Transform);
	sh.SetUniformMat4(transformLocation, Transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}