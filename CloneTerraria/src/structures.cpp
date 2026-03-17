#include"structures.h"

#include"ItemList.h"
#include"math/matrix.h"

Chest::Chest(int x
	, int y
	, std::vector<std::vector<Block>>& blocks)
{
	 
	m_Indestrucrtible = false;
	for (int i= 0; i < 50; i++)
	{
		m_amount[i] = 0;
		m_Items[i] = 0;
	}

	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Open = 0;
	bool found = false;
	int index = FindBlock(blocks, x, y - 1, found);
	blocks.at(x).at(index).m_BlockBehavior = b_Indestructible;
	index = FindBlock(blocks, x + 1, y - 1, found);
	blocks.at(x + 1).at(index).m_BlockBehavior = b_Indestructible;
}

void Chest::DestroyChest(std::vector<std::vector<Block>>& blocks)
{
	bool found = false;
	int index = FindBlock(blocks, m_Transform[0], m_Transform[1] - 1, found);
	blocks.at(m_Transform[0]).at(index).m_BlockBehavior = getBehaviorByTexture(blocks.at(m_Transform[0]).at(index).m_te);
	index = FindBlock(blocks, m_Transform[0] + 1, m_Transform[1] - 1, found);
	blocks.at(m_Transform[0] + 1).at(index).m_BlockBehavior = getBehaviorByTexture(blocks.at(m_Transform[0] + 1).at(index).m_te);

}
Door::Door(int x
	, int y
	, short type
	, std::vector<int>& sandX
	, std::vector < std::vector<wall>>& walls
	, std::vector<std::vector<Block>>& blocks)
	:m_Transform{x, y}
	, m_Type(type)
	, m_OpenSide(0)
	, m_Vertices{}
{
	getStructureVertices(x, y, type, m_Vertices);
	switch (type)
	{
	case s_Gate:
	case s_Door:
		for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
		{
			CreateBlock(m_Transform[0], i, i_DoorBlock, walls, blocks, sandX, NULL);
		}
		break;
	case s_TrapDoor:
		for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
		{
			CreateBlock(i, m_Transform[1], i_DoorBlock, walls, blocks, sandX, NULL);
		}
		break;
	default:
		Assert(true);
		break;
	}


}
int FindDoors(std::vector<Door>& structures
	, float x
	, float y
	, bool& found)
{
	for (int i = 0; i < structures.size(); i++)
	{
		if (structures.at(i).m_Vertices[0] <= x && structures.at(i).m_Vertices[2] >= x && structures.at(i).m_Vertices[3] <= y && structures.at(i).m_Vertices[1] >= y)
		{
			found = true;
			return i;
		}
	}
	return -1;
}
bool IsInAreaDoors(std::vector<Door>& structures
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
	, std::vector< std::vector<wall>>& walls
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<int>& sandX
	, float* playerTransforms)
{

	if (m_OpenSide)
	{
		getStructureVertices(m_Transform[0], m_Transform[1], m_Type, m_Vertices);
		switch (m_Type)
		{
		case s_Gate:
		case s_Door:
			for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
			{
				CreateBlock(m_Transform[0], i, i_DoorBlock, walls, blocks, sandX, NULL);
			}
			break;
		case s_TrapDoor:
			for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
			{
				CreateBlock(i, m_Transform[1], i_DoorBlock, walls, blocks, sandX, NULL);
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
		float playerTransform[2] = {playerTransforms[0],playerTransforms[1]};
		if (m_Type != s_TrapDoor)
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
					DestroyBlock(blocks, walls, sandX, m_Transform[0], i);
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
						DestroyBlock(blocks, walls, sandX, m_Transform[0], i);
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
		}
		else
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
			int vertices[4] = {m_Vertices[0], preferSide + m_Vertices[1], m_Vertices[2] ,preferSide + m_Vertices[3] };
			if (!isAnythinginArea(vertices, blocks, seedlings, trees, craftingStations, doors, chests))
			{
				for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
				{
					DestroyBlock(blocks, walls, sandX, i, m_Transform[1]);
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
						DestroyBlock(blocks, walls, sandX, i, m_Transform[1]);
					}
					m_Vertices[0] = vertices[0];
					m_Vertices[1] = vertices[1];
					m_Vertices[2] = vertices[2];
					m_Vertices[3] = vertices[3];

					m_OpenSide = -preferSide;
					
				}
			}
		}
	}
}
bool Door::CheckFloorAndCeil(std::vector<Door>& doors
	, std::vector<std::vector<Block>>& blocks
	, std::vector<DroppedItem>& droppedItems)
{
	bool found =false;
	FindBlock(blocks, m_Transform[0], m_Vertices[1]+1, found);
	if (found)
	{
		found = false;
		FindBlock(blocks, m_Transform[0], m_Transform[1] - 1, found);
		if (found)
		{
			return false;
		}
	}
	return true;

}
void DrawDoors(std::vector<Door>& doors
	, Shader& sh
	, unsigned int doorDD
	, unsigned int* doorTexture
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
			ErrorGL(glBindVertexArray(doorDD));
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

			break;
		}
		case s_TrapDoor:
		{

			break;
		}
		default:
			Assert(true);
			break;
		}
		ChangeTransform(doors.at(i).m_Transform[0], doors.at(i).m_Transform[1], transform);
		sh.SetUniformMat4(advancedTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
}
void Door::DestroyDoor(std::vector<std::vector<Block>>& blocks
	, std::vector< std::vector<wall>>& walls
	, std::vector<int>& sandX)
{
	if (!m_OpenSide)
	{
		getStructureVertices(m_Transform[0], m_Transform[1], m_Type, m_Vertices);
		switch (m_Type)
		{
		case s_Gate:
		case s_Door:
			for (int i = m_Vertices[3]; i <= m_Vertices[1]; i++)
			{
				DestroyBlock(blocks, walls, sandX, m_Transform[0], i);
			}
			break;
		case s_TrapDoor:
			for (int i = m_Vertices[0]; i <= m_Vertices[2]; i++)
			{
				DestroyBlock(blocks, walls, sandX, i, m_Transform[1]);
			}
			break;
		default:
			Assert(true);
			break;
		}
	}
}
int FindChest(std::vector<Chest>& structures
	, float x
	, float y
	, bool& found)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], s_Chest, structureVertices);
		if (structureVertices[0] <= x && structureVertices[2] >= x && structureVertices[3] <= y && structureVertices[1] >= y)
		{
			found = true;
			return i;
		}
	}
	return -1;
}
bool IsInAreaChest(std::vector<Chest>& structures
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

bool IsInAreaCraftStation(std::vector<CraftStation>& structures
	, int* vertice)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], structures.at(i).m_CraftStationtype, structureVertices);
		if (structureVertices[0] <= vertice[2] && structureVertices[2] >= vertice[0] && structureVertices[3] <= vertice[1] && structureVertices[1] >= vertice[3])
		{
			return true;
		}
	}
	return false;
}
int FindCraftStation(std::vector<CraftStation>& structures
	, float x
	, float y
	, bool& found)
{
	for (int i = 0; i < structures.size(); i++)
	{
		int structureVertices[4];
		getStructureVertices(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], structures.at(i).m_CraftStationtype, structureVertices);
		if (structureVertices[0] <= x && structureVertices[2] >= x && structureVertices[3] <= y && structureVertices[1] >= y)
		{
			found =  true;
			return i;
		}
	}
	return -1;
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
		vertices[1] = y + 4;
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
	default:
		std::cout << "error structure.cpp unknown structure :" << (unsigned int)structure << std::endl;
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
			FindBlock(blocks, j, craftingStation.at(i).m_Transform[1] - 1, floorFound);
			if (!floorFound)
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
	

	FindBlock(blocks, x, y, inBlock);
	if (inBlock)
	{
		return true;
	}
	FindWood(trees, x, y, inBlock);
	if (inBlock)
	{
		return true;
	}
	FindChest(chests, x, y, inBlock);
	if (inBlock)
	{
		return true;
	}
	FindCraftStation(craftingStations, x, y, inBlock);
	if (inBlock)
	{
		return true;
	}
	FindSeedling(seedlings, x, y, inBlock);
	if (inBlock)
	{
		return true;
	}
	FindDoors(doors, x, y, inBlock);
	return inBlock;
}
bool isAnythinginArea(int* vertices
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Door>& doors
	, std::vector<Chest>& chests)
	{
	bool inBlock = false;
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
	inBlock = blockInArea(blocks, vertices);
	if (inBlock)
	{
		return true;
	}
	inBlock = WoodInArea(trees, vertices);
	if (inBlock)
	{
		return true;
	}
	inBlock = IsInAreaCraftStation(craftingStations, vertices);
	if (inBlock)
	{
		return true;
	}
	inBlock = SeedlingInArea(seedlings, vertices);

	if (inBlock)
	{
		return true;
	}
	inBlock = IsInAreaDoors(doors, vertices);

	if (inBlock)
	{
		return true;
	}

	return IsInAreaChest(chests, vertices);
}
