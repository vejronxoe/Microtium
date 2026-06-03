#include"Editor.h"
#include "glfw/input.h"
#include "Opengl/Texture.h"
#include "Opengl/drawData.h"
#include "glfw/UI.h"
#include "glfw/Window.h"
#include "math/VectorOperation.h"
#include "math/matrix.h"
#include "ItemList.h"
#include "glfw/cursor.h"

#include <string>
#include <iostream>
#define NUMBEROFSLOTS (t_BlocksSize + (i_WallIce - i_WallDirt + 1) + s_StructureSize)


Editor::Editor(unsigned int eob)
{
	m_SelectBoxDD = CreateDrawData(eob,0.5f,-0.5f,0.5f,-0.5f,1,0,1.0f/4.0f,0);
	m_SelectBoxTex = CreateTextureRGBA("res/textures/SelectZone.png");
}
void DeleteStructuresInArea(int* vertices
, std::vector<Door>& doors
, std::vector<Chest>& chests
, std::vector<CraftStation>& craftingStations
, std::vector<seedling>& saplings)
{
	int verticesObj[4];
	for (int i = 0; i < doors.size(); i++)
	{
		if (DoTheyIntersect(vertices, doors[i].m_Vertices))
		{
			doors.erase(doors.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < chests.size();i++)
	{
		getStructureVertices(chests[i].m_Transform[0], chests[i].m_Transform[1], s_Chest, verticesObj);
		if (DoTheyIntersect(vertices, verticesObj))
		{
			chests.erase(chests.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < craftingStations.size(); i++)
	{
		getStructureVertices(craftingStations[i].m_Transform[0], craftingStations[i].m_Transform[1], craftingStations[i].m_CraftStationtype, verticesObj);
		if (DoTheyIntersect(vertices, verticesObj))
		{
			craftingStations.erase(craftingStations.begin()+i);
			i--;
		}
	}

	for (int i = 0; i < saplings.size(); i++)
	{
		getStructureVertices(saplings[i].m_Transform[0], saplings[i].m_Transform[1], s_Sapling, verticesObj);
		if (DoTheyIntersect(vertices, verticesObj))
		{

			saplings.erase(saplings.begin() + i);
			i--;
		}
	}
}
void DeleteBlocksInArea(std::vector<int>& ChunksToBulid
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& SandsXs
	, int* vertices)
{
	for (int i = vertices[0]; i <= vertices[2]; i++)
	{
		for (int j = vertices[3]; j <= vertices[1]; j++)
		{
			DestroyBlock(ChunksToBulid,blocks, SandsXs, i, j);
		}
	}
}
void DeleteWallsInArea(std::vector<int>& ChunksToBulid
, std::vector<std::vector<Wall>>& Walls
, int* vertices)
{

	for (int i = vertices[0]; i <= vertices[2]; i++)
	{
		for (int j = vertices[3]; j <= vertices[1]; j++)
		{
			DestroyWall(Walls, ChunksToBulid, i, j);
		}
	}
}

void Editor::CopyBlocksAndWalls(unsigned int* blocksTex
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<Wall>>& Walls)
{
	m_CopiedBlocks.assign(m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1, std::vector<int> {});
	for (int i = 0; i < m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1; i++)
	{
		m_CopiedBlocks[i].assign(m_SelectBoxSides[1] - m_SelectBoxSides[3] + 1, -1);
	}
	for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
	{
		for (int j = 0; j < blocks.at(i).size();j++)
		{
			if (blocks.at(i)[j].m_Y < m_SelectBoxSides[3])
			{
				break;
			}
			else if (blocks.at(i)[j].m_Y <= m_SelectBoxSides[1])
			{
				m_CopiedBlocks[i - m_SelectBoxSides[0]].at(abs(blocks.at(i)[j].m_Y - m_SelectBoxSides[1])) = blocks.at(i)[j].m_Type;
			}

		}
	}
	m_CopiedWalls.assign(m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1, std::vector<int> {});
	for (int i = 0; i < m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1; i++)
	{
		m_CopiedWalls[i].assign(m_SelectBoxSides[1] - m_SelectBoxSides[3] + 1, -1);
	}
	for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
	{
		for (int j = 0; j < Walls.at(i).size();j++)
		{
			if (Walls[i][j].m_Y < m_SelectBoxSides[3])
			{
				break;
			}
			else if (Walls[i][j].m_Y <= m_SelectBoxSides[1])
			{
				m_CopiedWalls[i - m_SelectBoxSides[0]].at(abs(Walls[i][j].m_Y - m_SelectBoxSides[1])) = Walls[i][j].m_Type;
			}

		}
	}
}
void Editor::Update(float deltaTime
	,char cursorState
	, unsigned int* blocksTex
	, unsigned int* structureTex
	, std::vector<int>& chunksToRebuildBlocks
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& chunksToRebuildWalls
	, std::vector<std::vector<Wall>>& Walls
	, std::vector<seedling>& saplings
	, std::vector<CraftStation>& CraftingStations
	, std::vector<Chest>& Chests
	, std::vector<Door>& Doors
	, std::vector<int>& SandsXs)
{ 
	float oldVelocity[2] = { m_Velocity[0], m_Velocity[1]};
	if (Input::AHold)
	{
		m_Velocity[0] -= deltaTime * 100;
	}
	if (Input::SHold)
	{
		m_Velocity[1] -= deltaTime * 100;

	}
	if (Input::DHold)
	{
		m_Velocity[0] += deltaTime * 100;

	}
	if (Input::WHold)
	{
		m_Velocity[1] += deltaTime * 100;

	}
	if (!(Input::WHold || Input::SHold))
	{
		m_Velocity[1] = 0;
		oldVelocity[1] = 0;
	}
	if (!(Input::AHold || Input::DHold))
	{
		m_Velocity[0] = 0;
		oldVelocity[0] = 0;
	}
	m_Transform[1] += oldVelocity[1] * deltaTime + 0.5f * (m_Velocity[1] - oldVelocity[1]) * deltaTime;
	m_Transform[0] += oldVelocity[0] * deltaTime + 0.5f * (m_Velocity[0] - oldVelocity[0]) * deltaTime;
	
	m_Transform[0] = CameraHitboxX(m_Transform[0]);
	m_Transform[1] = CameraHitboxY(m_Transform[1]);
	
	
	
	int x = roundf(Input::XMousePos + m_Transform[0]);
	int y = roundf(Input::YMousePos + m_Transform[1]);
	if (cursorState == canNotDoIt || cursorState == canOpenChest)
	{
		switch (m_placingType)
		{
		case selectType:
		{
			if (Input::LeftMousePress)
			{
				m_BoxSelected = false;
				m_FirstPointBox[0] = x;
				m_FirstPointBox[1] = y;
				m_SelectBoxSides[0] = x;
				m_SelectBoxSides[1] = y;
				m_SelectBoxSides[2] = x;
				m_SelectBoxSides[3] = y;
			}
			else if (Input::LeftMouseHold)
			{
				if (m_FirstPointBox[0] < x)
				{
					m_SelectBoxSides[0] = m_FirstPointBox[0];
					m_SelectBoxSides[2] = x;

				}
				else if (m_FirstPointBox[0] > x)
				{

					m_SelectBoxSides[2] = m_FirstPointBox[0];
					m_SelectBoxSides[0] = x;
				}
				else
				{
					m_SelectBoxSides[0] = x;
					m_SelectBoxSides[2] = x;
				}
				if (m_FirstPointBox[1] < y)
				{
					m_SelectBoxSides[1] = y;
					m_SelectBoxSides[3] = m_FirstPointBox[1];
				}
				else if (m_FirstPointBox[1] > y)
				{
					m_SelectBoxSides[1] = m_FirstPointBox[1];
					m_SelectBoxSides[3] = y;
				}
				else
				{
					m_SelectBoxSides[1] = y;
					m_SelectBoxSides[3] = y;
				}
			}
			else if (Input::LeftMouseRelease)
			{
				m_BoxSelected = true;
			}
			if (m_BoxSelected)
			{
				if (m_Eraser)
				{
					DeleteStructuresInArea(m_SelectBoxSides, Doors, Chests, CraftingStations, saplings);
					DeleteBlocksInArea(chunksToRebuildBlocks, blocks, SandsXs, m_SelectBoxSides);
					DeleteWallsInArea(chunksToRebuildWalls, Walls, m_SelectBoxSides);
					m_Eraser = false;
				}
				else if (m_Selected < t_BlocksSize && m_Selected > -1)
				{
					DeleteStructuresInArea(m_SelectBoxSides, Doors, Chests, CraftingStations, saplings);
					DeleteBlocksInArea(chunksToRebuildBlocks, blocks, SandsXs, m_SelectBoxSides);


					for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
					{
						for (int j = m_SelectBoxSides[3]; j <= m_SelectBoxSides[1];j++)
						{
							CreateBlock(i, j, m_Selected, chunksToRebuildBlocks, blocks, SandsXs);

						}
					}
					m_Selected = -1;
				}
				else if (m_Selected >= t_BlocksSize && m_Selected <= t_BlocksSize + (i_WallIce - i_WallDirt + 1))
				{
					DeleteStructuresInArea(m_SelectBoxSides, Doors, Chests, CraftingStations, saplings);
					DeleteWallsInArea(chunksToRebuildWalls, Walls, m_SelectBoxSides);
					for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
					{
						for (int j = m_SelectBoxSides[3]; j <= m_SelectBoxSides[1];j++)
						{

							createWall(i, j, getTypeByItem(m_Selected - t_BlocksSize + i_WallDirt), chunksToRebuildWalls, Walls);
						}
					}
					m_Selected = -1;
				}
				else if (Input::CtrlHold && Input::XPress)
				{

					CopyBlocksAndWalls(blocksTex, blocks, Walls);
					DeleteStructuresInArea(m_SelectBoxSides, Doors, Chests, CraftingStations, saplings);
					DeleteWallsInArea(chunksToRebuildWalls, Walls, m_SelectBoxSides);
					DeleteBlocksInArea(chunksToRebuildBlocks, blocks, SandsXs, m_SelectBoxSides);

				}
				else if (Input::CtrlHold && Input::CPress)
				{
					CopyBlocksAndWalls(blocksTex, blocks, Walls);
				}
				else if (Input::CtrlHold && Input::VPress)
				{
					if (m_CopiedBlocks.size())
					{
						int Vertices[4] = { m_FirstPointBox[0], m_FirstPointBox[1], m_FirstPointBox[0] + m_CopiedBlocks.size() - 1, m_FirstPointBox[1] - m_CopiedBlocks[0].size() + 1 };
						DeleteBlocksInArea(chunksToRebuildBlocks, blocks, SandsXs, Vertices);
						DeleteStructuresInArea(Vertices, Doors, Chests, CraftingStations, saplings);

						for (int i = 0; i < m_CopiedBlocks.size();i++)
						{
							for (int j = 0; j < m_CopiedBlocks[i].size();j++)
							{
								if (m_CopiedBlocks[i][j] != -1)
								{
									CreateBlock(m_FirstPointBox[0] + i, m_FirstPointBox[1] - j, m_CopiedBlocks[i][j], chunksToRebuildBlocks, blocks, SandsXs);

								}
							}
						}

						DeleteWallsInArea(chunksToRebuildWalls, Walls, Vertices);
						for (int i = 0; i < m_CopiedWalls.size();i++)
						{
							for (int j = 0; j < m_CopiedWalls[i].size();j++)
							{
								if (m_CopiedWalls[i][j] != -1)
								{
									createWall(m_FirstPointBox[0] + i, m_FirstPointBox[1] - j, m_CopiedWalls[i][j], chunksToRebuildWalls, Walls);

								}
							}
						}
					}
				}
			}

			break;
		}
		case brushType:
		{
			if (Input::LeftMouseHold)
			{
				if (m_Eraser)
				{
					DestroyWall(Walls, chunksToRebuildWalls, x, y);
					DestroyBlock(chunksToRebuildBlocks, blocks, SandsXs, x, y);
					int vec4[4] = { x,y,x,y };
					DeleteStructuresInArea(vec4, Doors, Chests, CraftingStations, saplings);

				}
				else if (m_Selected >= 0 && m_Selected < t_BlocksSize)
				{

					std::vector<tree> t;
					int index;
					bool found = FindBlock(blocks, x, y, index);
					if (!isAnythingOnThisTransform(x, y, blocks, saplings, t, CraftingStations, Doors, Chests) || found)
					{

						DestroyBlock(chunksToRebuildBlocks, blocks, SandsXs, x, y);
						CreateBlock(x, y, m_Selected, chunksToRebuildBlocks, blocks, SandsXs);

					}
				}
				else if (m_Selected >= t_BlocksSize && m_Selected < t_BlocksSize + (i_WallIce - i_WallDirt + 1))
				{
					DestroyWall(Walls, chunksToRebuildWalls, x, y);
					createWall(x, y, getTypeByItem(m_Selected - t_BlocksSize + i_WallDirt), chunksToRebuildWalls, Walls);
				}
				else if (m_Selected >= t_BlocksSize + (i_WallIce - i_WallDirt + 1) && m_Selected < t_BlocksSize + (i_WallIce - i_WallDirt + 1) + s_StructureSize)
				{
					int vertices[4];
					getStructureVertices(x, y, m_Selected - t_BlocksSize - (i_WallIce - i_WallDirt + 1), vertices);
					std::vector<tree> trees;
					if (!isAnythinginArea(vertices, blocks, saplings, trees, CraftingStations, Doors, Chests))
					{
						int index;
						int  vertices[4];
						bool floors = true;
						getStructureVertices(x, y, m_Selected - t_BlocksSize - (i_WallIce - i_WallDirt + 1), vertices);

						switch (m_Selected - t_BlocksSize - (i_WallIce - i_WallDirt + 1))
						{
						case s_TrapDoor:
						{
							floors = FindBlock(blocks, x - 1, y, index);
							if (!floors)
							{
								break;
							}
							floors = FindBlock(blocks, vertices[2] + 1, y, index);
							break;
						}
						case s_Door:
						case s_Gate:
							floors = FindBlock(blocks, x, vertices[1] + 1, index);
							if (!floors)
							{
								break;
							}
							break;
						default:

							for (int i = vertices[0]; i <= vertices[2]; i++)
							{
								floors = false;
								for (int j = 0; j < blocks.at(i).size(); j++)
								{
									if (blocks.at(i).at(j).m_Y == vertices[3] - 1)
									{
										floors = true;
										break;
									}
									if (blocks.at(i).at(j).m_Y < vertices[3] - 1)
									{
										break;
									}
								}
								if (!floors)
								{
									break;
								}
							}

							break;
						}
						if (floors)
						{
							int directionalLook = 0;
							if (-floorf(Input::XMousePos + m_Transform[0]) + Input::XMousePos + m_Transform[0] >= 0.5f)
							{
								directionalLook = 1;
							}
							else
							{
								directionalLook = -1;
							}
							CreateStructure(m_Selected - t_BlocksSize - (i_WallIce - i_WallDirt + 1), x, y, directionalLook, structureTex, blocks, saplings, CraftingStations, Chests, Doors);

						}
					}
				}
			}
			break;
		}
		case bucketType:
		{
			if (Input::LeftMousePress)
			{
				int indexB;
				bool foundB = FindBlock(blocks, x, y, indexB);
				int indexW;
				bool founW = FindWall(Walls, x, y, indexW);
				if (m_Eraser )
				{
					if (foundB)
					{
						std::vector<int> stack;
						int type = blocks.at(x).at(indexB).m_Type;
						stack.emplace_back(x);
						stack.emplace_back(y);
						while(stack.size())
						{
							int newY = stack.at(stack.size() - 1);
							stack.pop_back();
							int newX = stack.at(stack.size() - 1);
							stack.pop_back();
							int index;
							int table[2][4] = { {0,-1,0,1},{-1, 0,1,0} };
							
							for (int i = 0; i < 4;i++)
							{
								table[0][i] = Clamp(newX + table[0][i], Blocks::xMin, Blocks::xMax) - newX;
								if (FindBlock(blocks, newX + table[0][i], newY+ table[1][i], index))
								{
									if (type == blocks.at(newX + table[0][i]).at(index).m_Type)
									{
										stack.emplace_back(newX + table[0][i]);
										stack.emplace_back(newY + table[1][i]);
									}
								}
							}
							std::vector<int> sandFill;
							DestroyBlock(chunksToRebuildBlocks, blocks, sandFill, newX, newY);

						}
					}
					if(founW)
					{
						std::vector<int> stack;
						int type = Walls.at(x).at(indexW).m_Type;
						stack.emplace_back(x);
						stack.emplace_back(y);
						while (stack.size())
						{
							int newY = stack.at(stack.size() - 1);
							stack.pop_back();
							int newX = stack.at(stack.size() - 1);
							stack.pop_back();
							int index;
							int table[2][4] = { {0,-1,0,1},{-1, 0,1,0} };

							for (int i = 0; i < 4;i++)
							{
								table[0][i] = Clamp(newX + table[0][i], Blocks::xMin, Blocks::xMax) - newX;

								if (FindWall(Walls, newX + table[0][i], newY + table[1][i], index))
								{
									if (type == Walls.at(newX + table[0][i]).at(index).m_Type)
									{
										stack.emplace_back(newX + table[0][i]);
										stack.emplace_back(newY + table[1][i]);
									}
								}
							}
							std::vector<int> sandFill;
							DestroyWall(Walls, chunksToRebuildWalls, newX, newY);
							
						}
					}

				}
				else if (m_Selected >= 0 && m_Selected < t_BlocksSize && foundB)
				{
					if (m_Selected != blocks.at(x).at(indexB).m_Type)
					{

						std::vector<int> stack;
						int type = blocks.at(x).at(indexB).m_Type;
						stack.emplace_back(x);
						stack.emplace_back(y);
						while (stack.size())
						{
							int newY = stack.at(stack.size() - 1);
							stack.pop_back();
							int newX = stack.at(stack.size() - 1);
							stack.pop_back();
							int index;
							int table[2][4] = { {0,-1,0,1},{-1, 0,1,0} };

							for (int i = 0; i < 4;i++)
							{
								table[0][i] = Clamp(newX + table[0][i], Blocks::xMin, Blocks::xMax) - newX;
								if (FindBlock(blocks, newX + table[0][i], newY + table[1][i], index))
								{
									if (type == blocks.at(newX + table[0][i]).at(index).m_Type)
									{
										stack.emplace_back(newX + table[0][i]);
										stack.emplace_back(newY + table[1][i]);
									}
								}
							}
							std::vector<int> sandFill;
							DestroyBlock(chunksToRebuildBlocks, blocks, sandFill, newX, newY);
							CreateBlock(newX, newY, m_Selected, chunksToRebuildBlocks, blocks, sandFill);
						}
					}
				}
				else if (m_Selected >= t_BlocksSize && m_Selected <= t_BlocksSize + (i_WallIce - i_WallDirt))
				{
					int typeWanted = getTypeByItem(m_Selected - t_BlocksSize + i_WallDirt);
					if (typeWanted != Walls.at(x).at(indexW).m_Type)
					{
						std::vector<int> stack;
						int type = Walls.at(x).at(indexW).m_Type;
						stack.emplace_back(x);
						stack.emplace_back(y);
						while (stack.size())
						{
							int newY = stack.at(stack.size() - 1);
							stack.pop_back();
							int newX = stack.at(stack.size() - 1);
							stack.pop_back();
							int index;
							int table[2][4] = { {0,-1,0,1},{-1, 0,1,0} };

							for (int i = 0; i < 4;i++)
							{
								table[0][i] = Clamp(newX + table[0][i], Blocks::xMin, Blocks::xMax) - newX;
								if (FindWall(Walls, newX + table[0][i], newY + table[1][i], index))
								{
									if (type == Walls.at(newX + table[0][i]).at(index).m_Type)
									{
										stack.emplace_back(newX + table[0][i]);
										stack.emplace_back(newY + table[1][i]);
									}
								}
							}
							std::vector<int> sandFill;
							DestroyWall(Walls, chunksToRebuildWalls, newX, newY);
							createWall(newX, newY, typeWanted, chunksToRebuildWalls, Walls);

						}
					}
					
				}
			}
			break;

		}
		}
	}
	else if (cursorState == canBlockPick && Input::LeftMousePress)
	{
		int index = 0;
		bool blockFound = FindBlock(blocks, x, y, index);
		if (blockFound)
		{
			if (blocks.at(x).at(index).m_Type == t_DoorBlock)
			{
				blockFound = false;
			}
		}
		if (blockFound)
		{
			m_Selected = blocks.at(x).at(index).m_Type;
		}
		else if (FindSeedling(saplings,x,y,index))
		{
			int structType = saplings.at(index).m_Type;
			m_Selected = (t_BlocksSize + (i_WallIce - i_WallDirt + 1)) + structType;
		}
		else if(FindDoor(Doors,x,y,index))
		{
			int structType = Doors.at(index).m_Type;
			m_Selected = (t_BlocksSize + (i_WallIce - i_WallDirt + 1)) + structType;
		}
		else if(FindCraftStation(CraftingStations,x,y,index))
		{
			int structType = CraftingStations.at(index).m_CraftStationtype;
			m_Selected = (t_BlocksSize + (i_WallIce - i_WallDirt + 1)) + structType;
		}
		else if (FindChest(Chests, x, y, index))
		{
			int structType = s_Chest;
			m_Selected = (t_BlocksSize + (i_WallIce - i_WallDirt + 1)) + structType;
		}
		else if (FindWall(Walls, x, y, index))
		{
			m_Selected = t_BlocksSize + GetWallItemBytype(Walls.at(x).at(index).m_Type) - i_WallDirt;
		}
	}
}
void Editor::Draw(Shader& animSh
	, float* transform
	, float* scale)
{
	if (m_placingType == selectType)
	{
		animSh.Bind();
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SelectBoxTex));
		ErrorGL(glBindVertexArray(m_SelectBoxDD));
		ChangeScale(1, 1, scale);

		animSh.SetUniformMat4(animScale, scale);
		animSh.SetUniform1i(animLeangth, 4);
		int drawTransform[2] = { m_SelectBoxSides[0],m_SelectBoxSides[3] };
		int addx[4] = { 0,1,0,-1 };
		int addy[4] = { 1,0,-1, 0 };
		int sidelength[2] = { m_SelectBoxSides[2] - m_SelectBoxSides[0],m_SelectBoxSides[1] - m_SelectBoxSides[3] };
		for (int i = 0; i < 4; i++)
		{

			drawTransform[0] -= addx[i];
			drawTransform[1] -= addy[i];
			animSh.SetUniform1i(animNumber, i);
			for (int j = 0; j <= sidelength[abs(i % 2-1)]; j++)
			{
								
			
				drawTransform[0] += addx[i];
				drawTransform[1] += addy[i];
			
				ChangeTransform(drawTransform[0], drawTransform[1], transform);
				animSh.SetUniformMat4(animTransform, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			}
		}
	}
}

EditorHUD::EditorHUD(unsigned int eob
	, unsigned int backGroundTex)
{
	m_Icons[0] = CreateTextureRGBA("res/textures/PaintBrush.png");
	m_Icons[1] = CreateTextureRGBA("res/textures/SelectIcon.png");
	m_Icons[2] = CreateTextureRGBA("res/textures/Bucket.png");
	m_Icons[3] = CreateTextureRGBA("res/textures/RubberIcon.png");
	m_SlotTexs = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_SelectZoneTex = CreateTextureRGBA("res/textures/SelectZone.png");
	std::vector<Letter> l;
	std::vector<Chest> c;
	Create(true,c,l,eob);
}
void EditorHUD::Create(unsigned int eob
	, std::vector<Chest>& chests
	, std::vector<Letter>& ancii
	, bool first)
{
	if (!first)
	{
		ErrorGL(glDeleteBuffers(9, m_VBOs));
		ErrorGL(glDeleteBuffers(4 , m_EOBs));
		ErrorGL(glDeleteVertexArrays(9, m_DDs));
	}
	ErrorGL(glGenBuffers(4, m_EOBs));
	if (m_OpenChest)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				m_ChestAmount[i + j * 10].CreateText(std::to_string(chests[m_ChestIndex].m_amount[i + j * 10]), std::vector<Format> {Format(5,1,1,1,1,1)},ancii,eob,middleBottom,m_GapLengthChest +m_SideLengthChest/2.0f +i*(m_GapLengthChest+m_SideLengthChest), Window::height  -(4 * m_GapLength +3*m_SideLength) - (1+j) * (m_GapLengthChest + m_SideLengthChest));
			}
		}
	}
	m_Scroll = 0;
	m_GapLength = DistanceOnUI(0.016f);
	m_SideLength = DistanceOnUI(0.11f);
	m_GapLengthChest = DistanceOnUI(0.008f);
	m_SideLengthChest = DistanceOnUI(0.055f);
	m_DDs[IconSlotDD] = CreateDrawData(eob, 0.2f * m_SideLength, -0.2f * m_SideLength, 0.2f * m_SideLength, -0.2f * m_SideLength, m_VBOs[IconSlotDD]);
	m_DDs[IconUseSlotDD] = CreateDrawData(eob, 0.3f * m_SideLength, -0.3f * m_SideLength, 0.3f * m_SideLength, -0.3f * m_SideLength, m_VBOs[useSlotDD], 1, 0, 2 * TEXSLOTDISTANCE, TEXSLOTDISTANCE);
	m_DDs[useSlotDD] = CreateDrawData(eob, 0.6f * m_SideLength, -0.6f * m_SideLength, 0.6f * m_SideLength, -0.6f * m_SideLength, m_VBOs[useSlotDD], 1, 0, 2*TEXSLOTDISTANCE, TEXSLOTDISTANCE);
	m_DDs[defaultSlotUV] = CreateDrawData(eob, 0.4f * m_SideLength, -0.4f * m_SideLength, 0.4f * m_SideLength, -0.4f * m_SideLength, m_VBOs[defaultSlotUV]);
	m_DDs[slotChestDefaultUV] = CreateDrawData(eob, 0.4f * m_SideLengthChest, -0.4f * m_SideLengthChest, 0.4f * m_SideLengthChest, -0.4f * m_SideLengthChest, m_VBOs[slotChestDefaultUV]);

	std::vector<float> Vertices;
	std::vector<unsigned char> order;
	Vertices.reserve(NUMBEROFSLOTS * 16);
	int placebleObjectsNumber = floorf(NUMBEROFSLOTS / 2.0f);
	int j;
	for (j = 0; j <placebleObjectsNumber; j++)
	{
		for (int i = 0;i < 2;i++)
		{
			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(1);

			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(1);

		}
	}
	if (j*2 != NUMBEROFSLOTS)
	{
		Vertices.emplace_back(Window::width -  (m_GapLength + m_SideLength));
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - m_GapLength);
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - m_GapLength);
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(1);

		Vertices.emplace_back(Window::width - (m_GapLength + m_SideLength));
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(1);
	}
	order.resize((Vertices.size() / 16)*6);

	for (int i = 0; i < Vertices.size()/16; i++)
	{
		order.emplace_back(0 + i * 4);
		order.emplace_back(1 + i * 4);
		order.emplace_back(2 + i * 4);
		order.emplace_back(0 + i * 4);
		order.emplace_back(3 + i * 4);
		order.emplace_back(2 + i * 4);
	}
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[0]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[rightHUDSlots]));
	ErrorGL(glBindVertexArray(m_DDs[rightHUDSlots]));
	ErrorGL(glGenBuffers(1, &m_VBOs[rightHUDSlots]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[rightHUDSlots]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[0]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[0] = order.size();
	order.clear();
	Vertices.clear();


	Vertices.reserve((i_ItemSize-1) * 16);
	placebleObjectsNumber = floorf((i_ItemSize - 1) / 2.0f);
	for (j = 0; j < placebleObjectsNumber; j++)
	{
		for (int i = 0;i < 2;i++)
		{
			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(1);

			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(1);

		}
	}
	if (j * 2 != (i_ItemSize - 1))
	{
		Vertices.emplace_back(Window::width - (m_GapLength + m_SideLength));
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - m_GapLength);
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - m_GapLength);
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(1);

		Vertices.emplace_back(Window::width - (m_GapLength + m_SideLength));
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(1);
	}
	order.resize((Vertices.size() / 16) * 6);

	for (int i = 0; i < Vertices.size() / 16; i++)
	{
		order.emplace_back(0 + i * 4);
		order.emplace_back(1 + i * 4);
		order.emplace_back(2 + i * 4);
		order.emplace_back(0 + i * 4);
		order.emplace_back(3 + i * 4);
		order.emplace_back(2 + i * 4);
	}
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[2]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[rightItemSlots]));
	ErrorGL(glBindVertexArray(m_DDs[rightItemSlots]));
	ErrorGL(glGenBuffers(1, &m_VBOs[rightItemSlots]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[rightItemSlots]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[2]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[2] = order.size();
	order.clear();
	Vertices.clear();
	for (int i = 0; i < 4;i++)
	{

		Vertices.emplace_back(0.5f * m_GapLength);
		Vertices.emplace_back(Window::height - 0.5f * (m_GapLength + i * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(0);
		Vertices.emplace_back(0);

		Vertices.emplace_back(0.5f * (m_GapLength + m_SideLength));
		Vertices.emplace_back(Window::height - 0.5f * (m_GapLength + i * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(0);

		Vertices.emplace_back(0.5f * (m_GapLength + m_SideLength));
		Vertices.emplace_back(Window::height - 0.5f * ((i+1) * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(1);

		Vertices.emplace_back(0.5f * m_GapLength);
		Vertices.emplace_back(Window::height - 0.5f * ((i + 1) * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(0);
		Vertices.emplace_back(1);
	}




	order.resize((Vertices.size()/16)*6);
	for (int i = 0; i<Vertices.size() / 16; i++)
	{
		order.emplace_back(0 + i*4);
		order.emplace_back(1 + i*4);
		order.emplace_back(2 + i*4);
		order.emplace_back(0 + i*4);
		order.emplace_back(3 + i*4);
		order.emplace_back(2 + i*4);
	}

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[1]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[leftHUDSlots]));
	ErrorGL(glBindVertexArray(m_DDs[leftHUDSlots]));
	ErrorGL(glGenBuffers(1, &m_VBOs[leftHUDSlots]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[leftHUDSlots]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[1]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[1] = order.size();
	order.clear();
	Vertices.clear();
	for (j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10;i++)
		{
			
			Vertices.emplace_back( m_GapLengthChest + i * (m_GapLengthChest + m_SideLengthChest));
			Vertices.emplace_back(Window::height - (3 * (m_GapLength + m_SideLength) + m_GapLength) - (m_GapLengthChest + j * (m_GapLengthChest + m_SideLengthChest)));
			Vertices.emplace_back(TEXSLOTDISTANCE* chestSlot);
			Vertices.emplace_back(0);

			Vertices.emplace_back( (1+i) * (m_GapLengthChest + m_SideLengthChest));
			Vertices.emplace_back(Window::height - (3 * (m_GapLength + m_SideLength) + m_GapLength) - (m_GapLengthChest + j * (m_GapLengthChest + m_SideLengthChest)));
			Vertices.emplace_back(TEXSLOTDISTANCE * (chestSlot + 1));
			Vertices.emplace_back(0);

			Vertices.emplace_back( + (1 + i) * (m_GapLengthChest + m_SideLengthChest));
			Vertices.emplace_back(Window::height - (3 * (m_GapLength + m_SideLength) + m_GapLength) - ((j + 1) * (m_GapLengthChest + m_SideLengthChest)));
			Vertices.emplace_back(TEXSLOTDISTANCE * (chestSlot + 1));
			Vertices.emplace_back(1);

			Vertices.emplace_back(  m_GapLengthChest + i * (m_GapLengthChest + m_SideLengthChest));
			Vertices.emplace_back(Window::height - (3 * (m_GapLength + m_SideLength) + m_GapLength) - ((j + 1) * (m_GapLengthChest + m_SideLengthChest)));
			Vertices.emplace_back(TEXSLOTDISTANCE* chestSlot);
			Vertices.emplace_back(1);
		}
	}
	order.resize((Vertices.size() / 16) * 6);
	for (int i = 0; i < Vertices.size() / 16; i++)
	{
		order.emplace_back(0 + i * 4);
		order.emplace_back(1 + i * 4);
		order.emplace_back(2 + i * 4);
		order.emplace_back(0 + i * 4);
		order.emplace_back(3 + i * 4);
		order.emplace_back(2 + i * 4);
	}
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[3]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[InvOfChest]));
	ErrorGL(glBindVertexArray(m_DDs[InvOfChest]));
	ErrorGL(glGenBuffers(1, &m_VBOs[InvOfChest]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[InvOfChest]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[3]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[3] = order.size();

}
int EditorHUD::Update(float deltaTime
	, unsigned int eob 
	, float& blockSize
	, std::vector<Letter>& ancii
	, std::vector<Chest>& chests
	, Editor& editor)
{
	int amountInChest[50] = {};
	int cursorBehavior = canNotDoIt;
	int numberOfElements = NUMBEROFSLOTS;
	if (m_OpenChest)
	{
		for (int i = 0; i < 50; i++)
		{
			amountInChest[i] = chests[m_ChestIndex].m_amount[i];
		}
		numberOfElements = i_ItemSize - 1;
	}

	if (!Input::CtrlHold || m_OpenChest)
	{


	
		int row = -1;
		if (Window::width - m_GapLength - m_SideLength <= Input::XRawMousePos && Window::width - m_GapLength >= Input::XRawMousePos)
		{
			row = 0;
		}

		if (Window::width - 2 * (m_GapLength + m_SideLength) <= Input::XRawMousePos && Window::width - 2 * m_GapLength - m_SideLength >= Input::XRawMousePos)
		{
			row = 1;
		}

		if (m_GapLength <= Input::XRawMousePos && m_GapLength + m_SideLength >= Input::XRawMousePos)
		{
			row = 2;
		}


		switch (row)
		{
		case 0:
		case 1:
		{
			int i;
			for (i = 0; i < numberOfElements / 2; i++)
			{
				if (-i * (m_GapLength + m_SideLength) + m_GapLength + m_Scroll * (m_GapLength + m_SideLength) < Window::height - Input::YRawMousePos && m_GapLength + m_SideLength - i * (m_GapLength + m_SideLength) + m_Scroll * (m_GapLength + m_SideLength) > Window::height - Input::YRawMousePos)
				{
					if (Input::LeftMousePress)
					{
						editor.m_Selected = i * 2 + row;
					}
					cursorBehavior = canClickOnIt;
					break;
				}
			}
			if (row == 0 && i * 2 != numberOfElements && -i * (m_GapLength + m_SideLength) + m_GapLength + m_Scroll * (m_GapLength + m_SideLength) < Window::height - Input::YRawMousePos && m_GapLength + m_SideLength - i * (m_GapLength + m_SideLength) + m_Scroll * (m_GapLength + m_SideLength) > Window::height - Input::YRawMousePos)
			{
				if (Input::LeftMousePress)
				{
					editor.m_Selected = i * 2 + row;
				}
				cursorBehavior = canClickOnIt;

			}
			break;
		}
		case 2:
		{

			for (int i = 0; i < 4; i++)
			{
				if (0.5f * (m_GapLength + i * (m_GapLength + m_SideLength)) < Input::YRawMousePos && 0.5f * (i + 1) * (m_GapLength + m_SideLength) > Input::YRawMousePos)
				{
					if (Input::LeftMousePress)
					{
						switch (i)
						{
						case 0:
							editor.m_placingType = brushType;
							break;
						case 1:
							editor.m_Selected = -1;
							editor.m_Eraser = false;
							editor.m_placingType = selectType;
							break;
						case 2:
							editor.m_placingType = bucketType;
							break;

						case 3:
							editor.m_Eraser = !editor.m_Eraser;
							break;
						}
					}
					cursorBehavior = canClickOnIt;
					break;
				}
			}

			break;
		}
		}
		if (m_OpenChest)
		{
			int trasform[2] = { -1,-1 };
			for (int i = 0; i < 10;i++)
			{
				if (Input::XRawMousePos > m_GapLengthChest + i * (m_GapLengthChest + m_SideLengthChest) && Input::XRawMousePos < (1 + i) * (m_GapLengthChest + m_SideLengthChest))
				{
					trasform[0] = i;
					break;
				}
			}
			for (int i = 0; i < 5;i++)
			{
				if (Input::YRawMousePos > 4 * m_GapLength + 3 * m_SideLength + m_GapLengthChest + i * (m_GapLengthChest + m_SideLengthChest) && Input::YRawMousePos < 4 * m_GapLength + 3 * m_SideLength + (i + 1) * (m_GapLengthChest + m_SideLengthChest))
				{
					trasform[1] = i;
					break;
				}
			}
			if (trasform[0] != -1 && trasform[1] != -1)
			{
				cursorBehavior = canClickOnIt;
				int aimingSlot = trasform[0] + trasform[1] * 10;
				if (Input::LeftMousePress)
				{
					if (editor.m_Eraser)
					{
						chests[m_ChestIndex].m_amount[aimingSlot] = 0;
						chests[m_ChestIndex].m_Items[aimingSlot] = i_Nothing;
					}
					else if (editor.m_Selected != -1)
					{
						chests[m_ChestIndex].m_amount[aimingSlot] += 1;
						chests[m_ChestIndex].m_Items[aimingSlot] = editor.m_Selected + 1;
					}
				}
				else if (Input::RightMousePress)
				{
					if (chests[m_ChestIndex].m_Items[aimingSlot] != i_Nothing)
					{
						chests[m_ChestIndex].m_amount[aimingSlot] *= 2;
					}
				}
			}
		}
		if (!cursorBehavior)
		{
			int x = roundf(Input::XMousePos + editor.m_Transform[0]);
			int y = roundf(Input::YMousePos + editor.m_Transform[1]);


			int index;
			if (FindChest(chests, x, y, index))
			{
				cursorBehavior = canOpenChest;
				if (Input::RightMousePress)
				{
					for (int j = 0; j < 5; j++)
					{
						for (int i = 0; i < 10; i++)
						{
							m_ChestAmount[i + j * 10].CreateText(std::to_string(chests[m_ChestIndex].m_amount[i + j * 10]), std::vector<Format> {Format(5, 1, 1, 1, 1, 1)}, ancii, eob, middleBottom, m_GapLengthChest + m_SideLengthChest / 2.0f + i * (m_GapLengthChest + m_SideLengthChest), Window::height - (4 * m_GapLength + 3 * m_SideLength) - (1 + j) * (m_GapLengthChest + m_SideLengthChest));
						}
					}
					if (m_OpenChest)
					{
						chests[m_ChestIndex].m_Open = false;
					}
					chests[index].m_Open = true;
					m_ChestIndex = index;
					m_OpenChest = true;
					editor.m_placingType = -1;
					editor.m_Eraser = false;
				}

			}

		}

		if (!Input::LeftMouseHold)
		{
			if (Input::EPress)
			{
				editor.m_Eraser = !editor.m_Eraser;

			}
			if (Input::NumberPress[0])
			{
				editor.m_placingType = brushType;
			}
			else if (Input::NumberPress[1])
			{
				editor.m_Selected = -1;
				editor.m_Eraser = false;
				editor.m_placingType = selectType;
			}
			else if (Input::NumberPress[2])
			{
				editor.m_placingType = bucketType;
			}
		}
	}
	else if(!(Input::XPress || Input::CPress || Input::VPress))
	{
		cursorBehavior = canBlockPick;
	}
	if (Window::width - 3 * m_GapLength - 2 * m_SideLength <= Input::XRawMousePos)
	{

		if (Input::MouseWheel)
		{
			m_WantedScroll -= 2 * Input::MouseWheel;
		}
		m_WantedScroll = Clamp(m_WantedScroll, 0, numberOfElements / 2.0f);
		editor.m_Selected = Clamp(editor.m_Selected, -1, numberOfElements - 1);

	}
	else
	{
		if (Input::MouseWheel)
		{
			Window::gameZoom += 4 * Input::MouseWheel;
			Window::gameZoom = Clamp(Window::gameZoom, 0, 100);
			blockSize = DistanceOnUI(BlockSize);
			Window::halfHeightOfGameTransform = (Window::height / blockSize) / 2.0f;
			Window::halfWidthOfGameTransform = (Window::width / blockSize) / 2.0f;
		}		
		if (m_StaticBlockBar)
		{
			m_WantedScroll = 0;
		}
	}
	if (m_WantedScroll != m_Scroll)
	{
		float diff = (m_WantedScroll - m_Scroll);
		m_Scroll += (diff + 1 * diff / abs(diff))*4.0f * deltaTime;
		float newDiff = (m_WantedScroll - m_Scroll);
		if (newDiff != 0)
		{
			if (diff / abs(diff) != newDiff / abs(newDiff))
			{
				m_Scroll = m_WantedScroll;
			}
		}
	}
	if (editor.m_placingType != -1 && m_OpenChest)
	{
		chests[m_ChestIndex].m_Open = false;
		m_OpenChest = false;
	}
	if (m_OpenChest)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (amountInChest[i + j * 10] != chests[m_ChestIndex].m_amount[i + j * 10] && chests[m_ChestIndex].m_amount[i + j * 10] > 1)
				{


					m_ChestAmount[i + j*10].CreateText(std::to_string(chests[m_ChestIndex].m_amount[i + j * 10]), std::vector<Format> {Format(5, 1, 1, 1, 1, 1)}, ancii, eob, middleBottom, m_GapLengthChest + m_SideLengthChest / 2.0f + i * (m_GapLengthChest + m_SideLengthChest), Window::height - (4 * m_GapLength + 3 * m_SideLength) - (1 + j) * (m_GapLengthChest + m_SideLengthChest));

				}
			}
		}
	}
	if (Input::RPress)
	{
		m_StaticBlockBar = !m_StaticBlockBar;
	}
	return cursorBehavior;
}
void EditorHUD::Draw(Shader& sh
	, Shader& fontSh
	, Editor editor
	, std::vector<Chest>& chests
	, unsigned int fontTexture
	, unsigned int* itemsTex
	, unsigned int * blockTex
	, float* transform)
{

	sh.Bind();
	ChangeTransform(0, 0, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexs));
	ErrorGL(glBindVertexArray(m_DDs[leftHUDSlots]));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[1], GL_UNSIGNED_BYTE, 0));
	if (m_OpenChest)
	{
		ErrorGL(glBindVertexArray(m_DDs[InvOfChest]));
		ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[3], GL_UNSIGNED_BYTE, 0));
		ChangeTransform(0, m_Scroll * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);

		ErrorGL(glBindVertexArray(m_DDs[rightItemSlots]));
		ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[2], GL_UNSIGNED_BYTE, 0));
		ErrorGL(glBindVertexArray(m_DDs[slotChestDefaultUV]));
		for (int j =0 ; j < 5; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				DrawItem(sh, basicSize,transform,m_GapLengthChest + i*(m_GapLengthChest +m_SideLengthChest) + m_SideLengthChest / 2.0f, Window::height - (4*m_GapLength + 3* m_SideLength) - (1+j)*(m_SideLengthChest+m_GapLengthChest) + m_SideLengthChest / 2.0f,chests[m_ChestIndex].m_Items[i + j*10],itemsTex);
			}
		}
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexs));

	}
	else
	{
		ChangeTransform(0, m_Scroll * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindVertexArray(m_DDs[rightHUDSlots]));
		ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[0], GL_UNSIGNED_BYTE, 0));
	}
	ErrorGL(glBindVertexArray(m_DDs[useSlotDD]));
	if (editor.m_Selected != -1)
	{
		ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (editor.m_Selected / 2 - roundf(editor.m_Selected / 2.0f)) * (m_GapLength + m_SideLength) , m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(editor.m_Selected / 2) * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

	}
	ErrorGL(glBindVertexArray(m_DDs[IconUseSlotDD]));

	if (editor.m_placingType != -1)
	{
		ChangeTransform(0.5f * (m_SideLength / 2.0f + m_GapLength), Window::height - 0.5f * (m_SideLength / 2.0f + m_GapLength + editor.m_placingType * (m_GapLength + m_SideLength)), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	if (editor.m_Eraser)
	{
		ChangeTransform(0.5f * (m_SideLength / 2.0f + m_GapLength), Window::height - 0.5f * (m_SideLength / 2.0f + m_GapLength + 3 * (m_GapLength + m_SideLength)), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	ErrorGL(glBindVertexArray(m_DDs[IconSlotDD]));

	for (int i = 0; i < 4; i++)
	{
		ChangeTransform(0.5f * (m_SideLength / 2.0f + m_GapLength), Window::height - 0.5f * (m_SideLength / 2.0f + m_GapLength + i * (m_GapLength + m_SideLength)), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Icons[i]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	ErrorGL(glBindVertexArray(m_DDs[defaultSlotUV]));

	if (m_OpenChest)
	{ 

		for (int i = 0; i < i_ItemSize-1; i++)
		{
			ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (i / 2 - roundf(i / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(i / 2) * (m_GapLength + m_SideLength), transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, itemsTex[i+1]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		fontSh.Bind();
		ChangeTransform(0,0,transform);
		fontSh.SetUniformMat4(fontTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTexture));
		for (int i = 0; i < 50; i++)
		{
			if (chests[m_ChestIndex].m_amount[i] > 1)
			{
				m_ChestAmount[i].Draw(fontSh, sh, transform, fontTexture, fontTexture, false);
			}
		}

	}
	else
	{

		for (int i = 0; i < t_BlocksSize; i++)
		{
			ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (i / 2 - roundf(i / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(i / 2) * (m_GapLength + m_SideLength), transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[i]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		sh.SetUniform1i(basicSize + ShadowLocation, 1);
		for (int i = 0; i < (i_WallIce - i_WallDirt + 1);i++)
		{
			int index = t_BlocksSize + i;
			ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (index / 2 - roundf(index / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(index / 2) * (m_GapLength + m_SideLength), transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, itemsTex[i_WallDirt + i]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		}
		sh.SetUniform1i(basicSize + ShadowLocation, 0);
		for (int i = 0; i < s_StructureSize; i++)
		{
			int index = t_BlocksSize + (i_WallIce - i_WallDirt + 1) + i;
			ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (index / 2 - roundf(index / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(index / 2) * (m_GapLength + m_SideLength), transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, itemsTex[GetItemIDByStructure(i)]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
	}

}
