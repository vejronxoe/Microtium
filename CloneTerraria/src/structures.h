#pragma once
#include"Opengl/Shader.h"
#include"BlocksAndWalls.h"
#include "DroppedItems.h"

struct CraftStation
{
	char m_CraftStationtype;
	int m_Transform[2];
	char m_LookAt;
};
void DrawCraftStations(std::vector<CraftStation>& structures
	, Shader& sh
	, float* transform
	, unsigned int* structureDDs
	, unsigned int* structureTexs);
int FindCraftStation(std::vector<CraftStation>& structures
	, float x
	, float y
	, bool& found);
bool IsInAreaCraftStation(std::vector<CraftStation>& structures
	, int* vertice);
void getStructureVertices(int x
	, int y
	, unsigned int ID
	, int* vertices);
char GetStructureID(unsigned char Item);
unsigned char GetItemIDByStructure(char structure);
void CheckFloorCraftStations(std::vector<CraftStation>& craftingStation
	, std::vector<std::vector<Block>>& blocks
	, std::vector<DroppedItem>& droppedItems);

