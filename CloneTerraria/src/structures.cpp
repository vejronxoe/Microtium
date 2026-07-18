#include"structures.h"

#include"ItemList.h"
#include"math/matrix.h"

Chest::Chest(int x
	, int y
	, std::vector<std::vector<Block>>& blocks)
{

	m_Indestrucrtible = false;
	for (int i = 0; i < 50; i++)
	{
		m_amount[i] = 0;
		m_Items[i] = 0;
	}

	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Open = 0;
	if (blocks.at(x).at(y - 1 - Blocks::yMin).m_Behavior != b_Air)
	{
		blocks.at(x).at(y - 1 - Blocks::yMin).m_Behavior = b_Indestructible;
	}

	if (blocks.at(x + 1).at(y - 1 - Blocks::yMin).m_Behavior != b_Air)
	{
		blocks.at(x + 1).at(y - 1 - Blocks::yMin).m_Behavior = b_Indestructible;
	}
}


void Chest::DestroyChest(std::vector<std::vector<Block>>& blocks)
{
	blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin).m_Behavior = getBehaviorByType(blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin).m_Type);
	blocks.at(m_Transform[0] + 1).at(m_Transform[1] - 1 - Blocks::yMin).m_Behavior = getBehaviorByType(blocks.at(m_Transform[0] + 1).at(m_Transform[1] - 1 - Blocks::yMin).m_Type);
}
Door::Door(int x
	, int y
	, short type
	, std::vector<std::vector<Block>>& blocks)
	:m_Transform{x, y}
	, m_Type(type)
	, m_OpenSide(0)
	, m_Vertices{}
{
	std::vector<int> fill;
	getStructureVertices(x, y, type, m_Vertices);
	std::vector<int> fillSand;
	switch (type)
	{
	case s_Gate:
	case s_Door:
		for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
		{
			CreateBlock(m_Transform[0], i, t_DoorBlock, fill, blocks, fillSand);
		}
		break;
	case s_TrapDoor:
		for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
		{
			CreateBlock(i, m_Transform[1], t_DoorBlock,fill , blocks, fillSand);
		}
		break;
	default:
		Assert(true);
		break;
	}


}
bool FindDoor(std::vector<Door>& structures
	, float x
	, float y
	, int& index)
{
	for (int i = 0; i < structures.size(); i++)
	{
		if (structures.at(i).m_Vertices[0] <= x && structures.at(i).m_Vertices[2] >= x && structures.at(i).m_Vertices[3] <= y && structures.at(i).m_Vertices[1] >= y)
		{
			index = i;
			return true;
		}
	}
	return false;
}
bool FindDoor(std::vector<Door>& structures
	, int* vertice)
{

	for (int i = 0; i < structures.size(); i++)
	{
		if (structures.at(i).m_Vertices[0] <= vertice[2] && structures.at(i).m_Vertices[2] >= vertice[0] && structures.at(i).m_Vertices[3] <= vertice[1] && structures.at(i).m_Vertices[1] >= vertice[3])
		{
			return true;
		}
	}
	return false;

}
void Door::DoorInteract(std::vector<std::vector<Block>>& blocks
	, std::vector< std::vector<uint8_t>>& Walls
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<int>& sandX
	, float* playerTransforms)
{

	std::vector<int> chunksFill;
	if (m_OpenSide)
	{
		getStructureVertices(m_Transform[0], m_Transform[1], m_Type, m_Vertices);
		switch (m_Type)
		{
		case s_Gate:
		case s_Door:
			for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
			{
				CreateBlock(m_Transform[0], i, t_DoorBlock,chunksFill, blocks, sandX);
			}
			break;
		case s_TrapDoor:
			for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
			{
				CreateBlock(i, m_Transform[1], t_DoorBlock, chunksFill, blocks, sandX);
			}
			break;
		default:
			Assert(true);
			break;
		}
		m_OpenSide = 0;
	}
	else
	{
		float playerTransform[2] = { playerTransforms[0],playerTransforms[1] };
		switch (m_Type)
		{
		case s_Door:
			{

				playerTransform[0] -= m_Transform[0];
				int preferSide = 0;
				if (playerTransform[0])
				{
					preferSide = abs(playerTransform[0]) / playerTransform[0];
				}
				else
				{
					preferSide = 1;
				}
				int vertices[4] = { preferSide + m_Vertices[0], m_Vertices[1],preferSide + m_Vertices[2] ,m_Vertices[3] };
				if (!isAnythinginArea(vertices, blocks, seedlings, trees, craftingStations, doors, chests))
				{

					for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
					{
						DestroyBlock(chunksFill,blocks, sandX, m_Transform[0], i);
					}
					if (preferSide == -1)
					{
						m_Vertices[0] += -1;

					}
					else
					{
						m_Vertices[2] += 1;
					}
					m_OpenSide = preferSide;
				}
				else
				{
					vertices[0] = m_Vertices[0] - preferSide;
					vertices[1] = m_Vertices[1];
					vertices[2] = m_Vertices[2] - preferSide;
					vertices[3] = m_Vertices[3];
					if (!isAnythinginArea(vertices, blocks, seedlings, trees, craftingStations, doors, chests))
					{
						for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
						{
							DestroyBlock(chunksFill,blocks, sandX, m_Transform[0], i);
						}
						if (preferSide == -1)
						{
							m_Vertices[0] += -1;

						}
						else
						{
							m_Vertices[2] += 1;
						}
						m_OpenSide = -preferSide;
					}
				}
				break;
			}
		case s_TrapDoor:
			{
				playerTransform[1] -= m_Transform[1];
				int preferSide = 0;
				if (playerTransform[1])
				{
					preferSide = abs(playerTransform[1]) / playerTransform[1];
				}
				else
				{
					preferSide = 1;
				}
				int vertices[4] = { m_Vertices[0], preferSide + m_Vertices[1], m_Vertices[2] ,preferSide + m_Vertices[3] };
				if (!isAnythinginArea(vertices, blocks, seedlings, trees, craftingStations, doors, chests))
				{
					for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
					{
						DestroyBlock(chunksFill, blocks, sandX, i, m_Transform[1]);
					}

					m_Vertices[0] = vertices[0];
					m_Vertices[1] = vertices[1];
					m_Vertices[2] = vertices[2];
					m_Vertices[3] = vertices[3];

					m_OpenSide = preferSide;
				}
				else
				{

					vertices[0] = m_Vertices[0];
					vertices[1] = m_Vertices[1] - preferSide;
					vertices[2] = m_Vertices[2];
					vertices[3] = m_Vertices[3] - preferSide;
					if (!isAnythinginArea(vertices, blocks, seedlings, trees, craftingStations, doors, chests))
					{
						for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
						{
							DestroyBlock(chunksFill,blocks, sandX, i, m_Transform[1]);
						}
						m_Vertices[0] = vertices[0];
						m_Vertices[1] = vertices[1];
						m_Vertices[2] = vertices[2];
						m_Vertices[3] = vertices[3];

						m_OpenSide = -preferSide;

					}
				}
				break;
			}
		case s_Gate:
		{
			for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
			{
				DestroyBlock(chunksFill,blocks, sandX, m_Transform[0], i);
			}
			m_OpenSide = 1;
			break;
		}
			default:
				Assert(true);
				break;
		}
	}
}
bool Door::CheckFloorAndCeil(std::vector<Door>& doors
	, std::vector<std::vector<Block>>& blocks
	, std::vector<DroppedItem>& droppedItems)
{
	switch (m_Type)
	{
	case s_TrapDoor:
	{
		int fill;
		if (blocks.at(m_Transform[0] - 1).at(m_Transform[1] - Blocks::yMin).m_Behavior != b_Air)
		{
			return blocks.at(m_Vertices[2] + 1).at(m_Transform[1] - Blocks::yMin).m_Behavior == b_Air;
		}
		break;
	}
	default:
	{
		
		int fill;
		if (blocks.at(m_Transform[0]).at(m_Vertices[1] + 1 - Blocks::yMin).m_Behavior != b_Air)
		{
			return blocks.at(m_Transform[0]).at(m_Transform[1] - 1 - Blocks::yMin).m_Behavior == b_Air;
		}
		break;
	}
	}
	return true;

}
void DrawDoors(std::vector<Door>& doors
	, Shader& sh
	, unsigned int* structuresDD
	, unsigned int* structuresTexs
	, unsigned int* doorTexture
	, unsigned int* trapDoorTexture
	, float* transform
	, float* scale
	, float* rotation)
{
	sh.Bind();
	ChangeRotation(0, rotation);
	sh.SetUniformMat4(advancedRotation, rotation);
	for (int i = 0; i < doors.size(); i++)
	{
		switch (doors.at(i).m_Type)
		{
		case s_Door:
		{
			ErrorGL(glBindVertexArray(structuresDD[s_Door]));
			if (!doors.at(i).m_OpenSide)
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, doorTexture[0]));
				ChangeScale(1, 1, scale);
				sh.SetUniformMat4(advancedScale, scale);
			}
			else
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, doorTexture[1]));
				ChangeScale(doors.at(i).m_OpenSide, 1, scale);
				sh.SetUniformMat4(advancedScale, scale);
			}
			break;
		}
		case s_Gate:
		{
			ChangeScale(1, 1, scale);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, structuresTexs[s_Gate]));
			sh.SetUniformMat4(advancedScale, scale);
			ErrorGL(glBindVertexArray(structuresDD[s_Gate]));
			if (!doors.at(i).m_OpenSide)
			{
				sh.SetUniform1i(advancedSize + ShadowLocation, 0);
			}
			else
			{
				sh.SetUniform1i(advancedSize + ShadowLocation, 1);
			}
			break;
		}
		case s_TrapDoor:
		{
			ErrorGL(glBindVertexArray(structuresDD[s_TrapDoor]));
			if (!doors.at(i).m_OpenSide)
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, trapDoorTexture[0]));
				ChangeScale(1, 1, scale);
				sh.SetUniformMat4(advancedScale, scale);
			}
			else
			{
				ErrorGL(glBindTexture(GL_TEXTURE_2D, trapDoorTexture[1]));
				ChangeScale(1, doors.at(i).m_OpenSide, scale);
				sh.SetUniformMat4(advancedScale, scale);
			}
			break;
		}
		default:
			Assert(true);
			break;
		}
		ChangeTransform(doors.at(i).m_Transform[0], doors.at(i).m_Transform[1], transform);
		sh.SetUniformMat4(advancedTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		sh.SetUniform1i(advancedSize + ShadowLocation, 0);

	}
}
void Door::DestroyDoor(std::vector<std::vector<Block>>& blocks
	, std::vector< std::vector<uint8_t>>& Walls
	, std::vector<int>& sandX)
{
	std::vector<int> fill;
	if (!m_OpenSide)
	{
		getStructureVertices(m_Transform[0], m_Transform[1], m_Type, m_Vertices);
		switch (m_Type)
		{
		case s_Gate:
		case s_Door:
			for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
			{
				DestroyBlock(fill,blocks, sandX, m_Transform[0], i);
			}
			break;
		case s_TrapDoor:
			for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
			{
				DestroyBlock(fill,blocks, sandX, i, m_Transform[1]);
			}
			break;
		default:
			Assert(true);
			break;
		}
	}
}
bool FindChest(std::vector<Chest>& structures
	, float x
	, float y
	, int& index)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], s_Chest, structureVertices);
		if (structureVertices[0] <= x && structureVertices[2] >= x && structureVertices[3] <= y && structureVertices[1] >= y)
		{
			index = i;
			return true;
		}
	}
	return false;
}
bool FindChest(std::vector<Chest>& structures
	, int* vertice)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], s_Chest, structureVertices);
		if (structureVertices[0] <= vertice[2] && structureVertices[2] >= vertice[0] && structureVertices[3] <= vertice[1] && structureVertices[1] >= vertice[3])
		{
			return true;
		}
	}
	return false;
}
void DrawChests(std::vector<Chest>& structures
	, Shader& sh
	, float* transform
	, unsigned int  openChestTex
	, unsigned int* structureDDs
	, unsigned int* structureTexs)
{
	sh.Bind();
	ErrorGL(glBindTexture(GL_TEXTURE_2D, structureTexs[s_Chest]));
	ErrorGL(glBindVertexArray(structureDDs[s_Chest]));
	sh.SetUniform1i(basicSize + structureLookAt, 1);
	for (int i = 0; i < structures.size(); i++)
	{
		if (structures.at(i).m_Open)
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, openChestTex));
			ChangeTransform(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, structureTexs[s_Chest]));
		}
		else
		{
			ChangeTransform(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], transform);
			sh.SetUniformMat4(basicTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		}
	}
}


void DrawCraftStations(std::vector<CraftStation>& structures
	, Shader& sh
	, float* transform
	, unsigned int* structureDDs
	, unsigned int* structureTexs)
{
	sh.Bind();
	for(int i = 0; i < structures.size(); i++)
	{
		ChangeTransform(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], transform);
		sh.SetUniformMat4(basicTransform, transform);
		sh.SetUniform1i(basicSize + structureLookAt, structures.at(i).m_LookAt);

		ErrorGL(glBindVertexArray(structureDDs[structures.at(i).m_CraftStationtype]));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, structureTexs[structures.at(i).m_CraftStationtype]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
}

bool FindCraftStation(std::vector<CraftStation>& structures
	, int* vertice)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], structures.at(i).m_CraftStationtype, structureVertices);

		bool a = structureVertices[0] <= vertice[2];
		bool b = structureVertices[2] >= vertice[0];
		bool c = structureVertices[3] <= vertice[1];
		bool d = structureVertices[1] >= vertice[3];
		if (structureVertices[0] <= vertice[2] && structureVertices[2] >= vertice[0] && structureVertices[3] <= vertice[1] && structureVertices[1] >= vertice[3])
		{
			return true;
		}
	}
	return false;
}
bool FindCraftStation(std::vector<CraftStation>& structures
	, float x
	, float y
	, int& index)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], structures.at(i).m_CraftStationtype, structureVertices);
		if (structureVertices[0] <= x && structureVertices[2] >= x && structureVertices[3] <= y && structureVertices[1] >= y)
		{
			index = i;
			return true;
		}
	}
	return false;
}
void getStructureVertices(int x
	, int y
	, unsigned int ID
	, int* vertices)
{
	switch (ID)
	{
	case s_Sapling:
		vertices[0] = x;
		vertices[1] = y + 1;
		vertices[2] = x;
		vertices[3] = y;
		break;
	case s_Anvil:
	case s_TrapDoor:
	case s_CraftingTable:
		vertices[0] = x;
		vertices[1] = y;
		vertices[2] = x + 1;
		vertices[3] = y;
		break;
	case s_Forge:
		vertices[0] = x;
		vertices[1] = y + 2;
		vertices[2] = x + 1;
		vertices[3] = y;
		break;
	case s_Chest:
		vertices[0] = x;
		vertices[1] = y + 1;
		vertices[2] = x + 1;
		vertices[3] = y;
		break;
	case s_Door:
		vertices[0] = x;
		vertices[1] = y + 2;
		vertices[2] = x;
		vertices[3] = y;
		break;
	case s_Gate:
		vertices[0] = x;
		vertices[1] = y + 3;
		vertices[2] = x;
		vertices[3] = y;
		break;
	default:
		Assert(true);
		break;
		
	}
}
char GetStructureID(unsigned char Item)
{

	switch (Item)
	{
	case i_Sapling:
		return s_Sapling;
	case i_CraftingTable:
		return s_CraftingTable;
	case i_Anvil:
		return s_Anvil;
	case i_Forge:
		return s_Forge;
	case i_Chest:
		return s_Chest;
	case i_Door:
		return s_Door;
	case i_Gate:
		return s_Gate;
	case i_TrapDoor:
		return s_TrapDoor;

	default:
		std::cout << "error structure.cpp unknown item :" << (unsigned int)Item << std::endl;
		Assert(true);
		return s_CraftingTable;
	}
}
unsigned char GetItemIDByStructure(char structure)
{

	switch (structure)
	{
	case s_Sapling:
		return i_Sapling;
	case s_CraftingTable:
		return i_CraftingTable;
	case s_Anvil:
		return i_Anvil;
	case s_Forge:
		return i_Forge;
	case s_Chest:
		return i_Chest;
	case s_Door:
		return i_Door;
	case s_Gate:
		return i_Gate;
	case s_TrapDoor:
		return i_TrapDoor;
	default:
		std::cout << "error structure.cpp unknown structure :" << (unsigned int)structure << std::endl;
		Assert(true);
		return i_CraftingTable;
	}
}
void CheckFloorCraftStations(std::vector<CraftStation>& craftingStation
	, std::vector<std::vector<Block>>& blocks
	, std::vector<DroppedItem>& droppedItems)
{
	for (int i = 0; i < craftingStation.size(); i++)
	{
		bool floorFound = false;
		int vertices[4];
		getStructureVertices(craftingStation.at(i).m_Transform[0], craftingStation.at(i).m_Transform[1], craftingStation.at(i).m_CraftStationtype, vertices);
		for (int j = vertices[0]; j <= vertices[2]; j++)
		{
			if (blocks.at(j).at(craftingStation.at(i).m_Transform[1] - 1 - Blocks::yMin).m_Behavior == b_Air)
			{
				droppedItems.emplace_back(craftingStation.at(i).m_Transform[0], craftingStation.at(i).m_Transform[1], 0, GetItemIDByStructure(craftingStation.at(i).m_CraftStationtype),1,true);
				craftingStation.erase(i + craftingStation.begin());
				i--;
				break;
			}
			floorFound = false;
		}
	}
}

bool isAnythingOnThisTransform(int x
	, int y 
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Door>& doors
	, std::vector<Chest>& chests)
{

	bool inBlock = false;
	
	int fill;
	if (blocks.at(x).at(y - Blocks::yMin).m_Type != t_Air)
	{
		return true;
	}
	if (FindWood(trees, x, y, fill))
	{
		return true;
	}
	if (FindChest(chests, x, y, fill))
	{
		return true;
	}
	if (FindCraftStation(craftingStations, x, y, fill))
	{
		return true;
	}
	if (FindSeedling(seedlings, x, y, fill))
	{
		return true;
	}
	return FindDoor(doors, x, y, fill);
}
bool isAnythinginArea(int* vertices
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Door>& doors
	, std::vector<Chest>& chests)
	{

	if (vertices[0] <= Blocks::xMin)
	{
		return true;
	}
	else if (vertices[1] >= Blocks::yMax)
	{
		return true;
	}
	else if (vertices[2] >= Blocks::xMax)
	{
		return true;

	}
	else if (vertices[3] <= Blocks::yMin)
	{
		return true;

	}

	if (FindBlock(blocks, vertices))
	{
		return true;
	}
	if (FindWood(trees, vertices))
	{
		return true;
	}
	if (FindCraftStation(craftingStations, vertices))
	{
		return true;
	}
	if (FindSeedling(seedlings, vertices))
	{
		return true;
	}
	if (FindDoor(doors, vertices))
	{
		return true;
	}

	return FindChest(chests, vertices);
}
void CreateStructure(int StructureID
	,int x
	,int y
	,int directionLook
	,unsigned int* structuresTex 
	, std::vector<std::vector<Block>>& blocks
	,std::vector<seedling>& seedlings
	,std::vector<CraftStation>& craftStations
	,std::vector<Chest>& chests
	,std::vector<Door>& doors)
{
	switch (StructureID)
	{
	case s_Sapling:
		seedlings.emplace_back(s_Sapling, x, y, structuresTex, blocks);
		break;
	case s_CraftingTable:
	{
		CraftStation table;
		table.m_CraftStationtype = s_CraftingTable;
		table.m_Transform[0] = x;
		table.m_Transform[1] = y;
		table.m_LookAt = directionLook;
		craftStations.emplace_back(table);
		break;
	}
	case s_Forge:
	{
		CraftStation forge;
		forge.m_CraftStationtype = s_Forge;
		forge.m_Transform[0] = x;
		forge.m_Transform[1] = y;
		forge.m_LookAt = directionLook;
		craftStations.emplace_back(forge);
		break;
	}
	case s_Anvil:
	{
		CraftStation anvil;
		anvil.m_CraftStationtype = s_Anvil;
		anvil.m_Transform[0] = x;
		anvil.m_Transform[1] = y;
		anvil.m_LookAt = directionLook;
		craftStations.emplace_back(anvil);
		break;
	}
	case s_Chest:
	{
		chests.emplace_back(x, y, blocks);


		break;
	}
	case s_Door:
	case s_TrapDoor:
	case s_Gate:
		doors.emplace_back(x, y,StructureID, blocks);
		break;
	}
}