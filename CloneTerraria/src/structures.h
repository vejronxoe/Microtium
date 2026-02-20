#pragma once
#include"Opengl/Shader.h"


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
bool isCraftStationOnThisSpot(std::vector<CraftStation>& structures
	, float x
	, float y);
bool IsInAreaCraftStation(std::vector<CraftStation>& structures
	, int* vertice);
void getStructureVertices(int x
	, int y
	, unsigned int ID
	, int* vertices);
char GetStructureID(unsigned char Item);