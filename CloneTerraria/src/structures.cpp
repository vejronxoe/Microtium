#include"structures.h"

#include"ItemList.h"
#include"math/matrix.h"

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
	default:
		std::cout << "error playyer.cpp unknown structure :" << (unsigned int)structure << std::endl;
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