#include"flora.h"

#include "Opengl/ErrorSystem.h"
#include "math/matrix.h"
#include "glfw/Window.h"
#include "ItemList.h"
#define TIMETOGROW 10
void checkTreesWithCrowns(std::vector<tree> trees
	, float* objVertices4
	, bool& inBlock)
{
	if (!inBlock)
	{
		for (int i = 0; i < trees.size(); i++)
		{

			float vertices[4];
			switch (trees.at(i).m_PartOfTree)
			{


			case p_Log:
				if (objVertices4[1] >= trees.at(i).m_Transform[1] && objVertices4[3] <= trees.at(i).m_Transform[1] && objVertices4[2] >= trees.at(i).m_Transform[0] && objVertices4[0] <= trees.at(i).m_Transform[0])
				{
					inBlock = true;
					break;
				}
				break;
			case p_Crown:

				vertices[0] = trees.at(i).m_Transform[0] - 3; vertices[1] = trees.at(i).m_Transform[1] + 3;
				vertices[2] = trees.at(i).m_Transform[0] + 3; vertices[3] = trees.at(i).m_Transform[1];
				if ((vertices[1] >= objVertices4[1] || vertices[1] >= objVertices4[3]) && (vertices[3] <= objVertices4[1] || vertices[3] <= objVertices4[3]))
				{
					if ((vertices[2] >= objVertices4[0] || vertices[2] >= objVertices4[2]) && (vertices[0] <= objVertices4[0] || vertices[0] <= objVertices4[2]))
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
					printf("error in sapling everyFrame wrong Rotation : %d", trees.at(i).m_Rotation);
					break;
				}
				if ((vertices[1] >= objVertices4[1] || vertices[1] >= objVertices4[3]) && (vertices[3] <= objVertices4[1] || vertices[3] <= objVertices4[3]))
				{
					if ((vertices[2] >= objVertices4[0] || vertices[2] >= objVertices4[2]) && (vertices[0] <= objVertices4[0] || vertices[0] <= objVertices4[2]))
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

void tree::drawTree(Shader& sh
	, unsigned int transformLocation
	, unsigned int rotateLocation
	, float* cameraCoordinate
	, float* transform
	, float* rotation)
{
	if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform + 5) >= m_Transform[0] && floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform - 5) <= m_Transform[0])
	{
		if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform - 5) <= m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform + 5) >= m_Transform[1])
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
	, unsigned int* structuresTextures
	, std::vector<std::vector<Block>>& blocks)
:m_Transform{x, y}
,m_Timer(0)
,m_Type(type)
,m_Texture(structuresTextures[type])
{
	for (int i = 0; i < blocks.at(m_Transform[0]).size(); i++)
	{	
		if (blocks.at(m_Transform[0]).at(i).m_Transform[1] == m_Transform[1] - 1)
		{
			m_IndexOfGroundBlock = i;
			break;
		}
	}
}
bool seedling::everyFrame(float deltaTime
	, unsigned int* treeTextures
	, unsigned int* treeDD
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings 
	, std::vector<tree>& trees)
{
	
	bool noGround = true;
	if (m_IndexOfGroundBlock < blocks.at(m_Transform[0]).size())
	{
		noGround = !(blocks.at(m_Transform[0]).at(m_IndexOfGroundBlock).m_Transform[1] == m_Transform[1] - 1);
		if(noGround)
		{
			for (int i = 0; i < blocks.at(m_Transform[0]).size(); i++)
			{
				if (blocks.at(m_Transform[0]).at(i).m_Transform[1] == m_Transform[1] - 1)
				{
					m_IndexOfGroundBlock = i;
					noGround = false;
					break;
				}
			}
		}
	}
	m_Timer += deltaTime;
	if (m_Timer > TIMETOGROW && !noGround)
	{
		int leangth = (rand() % 16) + 6;
		float crownVertices[4];
		crownVertices[0] = m_Transform[0] - 3; crownVertices[1] = m_Transform[1] + leangth + 3;
		crownVertices[2] = m_Transform[0] + 3; crownVertices[3] = m_Transform[1] + leangth;
		float vertices[4];
		vertices[0] = m_Transform[0]; vertices[1] = m_Transform[1] + leangth;
		vertices[2] = m_Transform[0]; vertices[3] = m_Transform[1];
		bool inBlock = (Blocks::yMax <= crownVertices[1] || Blocks::xMax <= crownVertices[2] || Blocks::xMin >= crownVertices[0]);

		if (!inBlock)
		{
			for (int i = 0; i < blocks.at(m_Transform[0]).size(); i++)
			{

				if (blocks.at(m_Transform[0]).at(i).m_Transform[1] >= m_Transform[1] && blocks.at(m_Transform[0]).at(i).m_Transform[1] <= m_Transform[1] + leangth)
				{
					inBlock = true;
					break;
				}
			}
		}
		
		checkTreesWithCrowns(trees, vertices, inBlock);
		if (!inBlock)
		{
			
			checkTreesWithCrowns(trees, crownVertices, inBlock);

			if (!inBlock)
			{
				for (int j = crownVertices[0]; j < crownVertices[2] + 1; j++)
				{
					for (int i = 0; i < blocks.at(j).size(); i++)
					{
						if (crownVertices[1] >= blocks.at(j).at(i).m_Transform[1] && crownVertices[3] <= blocks.at(j).at(i).m_Transform[1])
						{
							inBlock = true;
							break;
						}
					}

				}
			}
		}
		if (inBlock)
		{
			m_Timer = 0;
		}
		else
		{
			blocks.at(m_Transform[0]).at(m_IndexOfGroundBlock).m_BlockBehavior = b_Indestructible;

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

			int branchsMaxCount = floor(leangth / 4);
			unsigned int leftBranchs = rand() % branchsMaxCount;
			unsigned int rightBranchs = rand() % branchsMaxCount;
			
			std::vector<int> order;
			if (Blocks::xMin < m_Transform[0] - 5)
			{
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
				for (int j = 0; j < order.size(); j++)
				{
					for (int i = 0; i < order.size(); i++)
					{
						if (order.at(j) != order.at(i) && order.at(j) + 2 >= order.at(i) && order.at(j) - 2 <= order.at(i))
						{
							if (i < j)
							{
								j--;
							}

							order.erase(order.begin() + i);
							i--;
						}
					}
				}

				while (leftBranchs != 0 && order.size())
				{
					inBlock = false;
					for (int j = m_Transform[0] - 4; j < m_Transform[0]; j++)
					{
						for (int i = 0; i < blocks.at(j).size(); i++)
						{
							if (blocks.at(j).at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 1 && blocks.at(j).at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && blocks.at(j).at(i).m_Transform[0] != m_Transform[0] - 1)
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

					float vertices[4] = { m_Transform[0] - 4 ,m_Transform[1] + order.at(0) + 1,m_Transform[0] - 1 ,m_Transform[1] + order.at(0) - 1 };
					checkTreesWithCrowns(trees, vertices, inBlock);
					for (int i = 0; i < seedlings.size(); i++)
					{
						if (seedlings.at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 2 && seedlings.at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && seedlings.at(i).m_Transform[0] >= m_Transform[0] - 4 && seedlings.at(i).m_Transform[0] < m_Transform[0] - 1)
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
						int decider = rand() % 4;
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
			if (Blocks::xMax > m_Transform[0] + 5)
			{
				order.clear();
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
				for (int j = 0; j < order.size(); j++)
				{
					for (int i = 0; i < order.size(); i++)
					{

						if (order.at(j) != order.at(i) && order.at(j) + 2 >= order.at(i) && order.at(j) - 2 <= order.at(i))
						{
							if (i < j)
							{
								j--;
							}
							order.erase(order.begin() + i);
						}
					}
				}
				while (rightBranchs != 0 && order.size())
				{
					inBlock = false;
					for (int j = m_Transform[0]; j < m_Transform[0] + 4; j++)
					{
						for (int i = 0; i < blocks.at(j).size(); i++)
						{
							if (blocks.at(j).at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 1 && blocks.at(j).at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && blocks.at(j).at(i).m_Transform[0] != m_Transform[0] + 1)
							{
								inBlock = true;
								break;
							}
							else if (blocks.at(j).at(i).m_Transform[0] == m_Transform[0] + 1 && blocks.at(j).at(i).m_Transform[1] == m_Transform[1] + order.at(0))
							{
								inBlock = true;
								break;
							}
						}
					}

					float vertices[4] = { m_Transform[0] + 1 ,m_Transform[1] + order.at(0) + 1,  m_Transform[0] + 4,m_Transform[1] + order.at(0) - 1 };
					checkTreesWithCrowns(trees, vertices, inBlock);
					for (int i = 0; i < seedlings.size(); i++)
					{
						if (seedlings.at(i).m_Transform[1] >= m_Transform[1] + order.at(0) - 2 && seedlings.at(i).m_Transform[1] <= m_Transform[1] + order.at(0) + 1 && seedlings.at(i).m_Transform[0] <= m_Transform[0] + 4 && seedlings.at(i).m_Transform[0] > m_Transform[0] + 1)
						{
							inBlock = true;
							break;
						}
						else if (seedlings.at(i).m_Transform[0] == m_Transform[0] + 1 && seedlings.at(i).m_Transform[1] == m_Transform[1] + order.at(0))
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
						int decider = rand() % 4;
						if (decider)
						{
							decider = i_Sapling;
						}
						else
						{
							decider = i_Nothing;
						}
						trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, m_Transform[0] + 1, m_Transform[1] + order.at(0), -90.0f);
						trees.emplace_back(treeTextures[p_SmallCrown], treeDD[p_SmallCrown], decider, 35, p_SmallCrown, m_Transform[0] + 2, m_Transform[1] + order.at(0), -90.0f);
						leftBranchs--;
						order.erase(order.begin());
					}

				}
			}
			return true;
		}
	}
	return noGround;
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