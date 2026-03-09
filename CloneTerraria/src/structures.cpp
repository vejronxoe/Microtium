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
int findChest(std::vector<Chest>& structures
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
	, unsigned int* structureDDs
	, unsigned int* structureTexs)
{
	sh.Bind();
	ErrorGL(glBindTexture(GL_TEXTURE_2D, structureTexs[s_Chest]));
	ErrorGL(glBindVertexArray(structureDDs[s_Chest]));
	for (int i = 0; i < structures.size(); i++)
	{
		ChangeTransform(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], transform);
		sh.SetUniformMat4(basicTransform, transform);

		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
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
	default:
		std::cout << "error structure.cpp unknown item :" << (unsigned int)Item << std::endl;
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
	findChest(chests,x,y,inBlock);
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
	return inBlock;
}
bool isAnythinginArea(int* vertices
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Chest>& chests)
	{
	bool inBlock = false;
	if (vertices[0] <= Blocks::xMin)
	{
		inBlock = true;
	}
	else if (vertices[1] >= Blocks::yMax)
	{
		inBlock = true;
	}
	else if (vertices[2] >= Blocks::xMax)
	{
		inBlock = true;

	}
	else if (vertices[3] <= Blocks::yMin)
	{
		inBlock = true;
	}
	if (inBlock)
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

	inBlock = IsInAreaChest(chests, vertices);
	return inBlock;
}
