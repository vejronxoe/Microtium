#pragma once
#include"Opengl/Shader.h"
#include"BlocksAndWalls.h"
#include "DroppedItems.h"
#include "flora.h"

struct CraftStation
{
	char m_CraftStationtype;
	int m_Transform[2];
	char m_LookAt;
};
struct Chest
{

	int m_Transform[2];
	bool m_Indestrucrtible;
	unsigned short int m_Items[50];
	unsigned short int m_amount[50];
	bool m_Open;
	Chest(int x
		,int y
		, std::vector<std::vector<Block>>& blocks);
	void DestroyChest(std::vector<std::vector<Block>>& blocks);
};
struct Door
{
	int m_Transform[2];
	short m_Type;
	short m_OpenSide;
	int m_Vertices[4];
	Door(int x
		,int y
		, short type
		, std::vector<std::vector<Block>>& blocks);
	void DoorInteract(std::vector<std::vector<Block>>& blocks
		, std::vector< std::vector<uint8_t>>& Walls
		, std::vector<seedling>& seedlings
		, std::vector<tree>& trees
		, std::vector<CraftStation>& craftingStations
		, std::vector<Chest>& chests
		, std::vector<Door>& doors
		, std::vector<int>& sandX
		, float* playerTransform);
	void DestroyDoor(std::vector<std::vector<Block>>& blocks
		, std::vector< std::vector<uint8_t>>& Walls
		, std::vector<int>& sandX);
	bool CheckFloorAndCeil(std::vector<Door>& doors
		, std::vector<std::vector<Block>>& blocks
		, std::vector<DroppedItem>& droppedItems);
};
bool FindDoor(std::vector<Door>& structures
	, float x
	, float y
	, int& index);
bool FindDoor(std::vector<Door>& structures
	, int* vertice);
void DrawDoors(std::vector<Door>& doors
	, Shader& sh
	, unsigned int* structuresDD
	, unsigned int* structuresTexs
	, unsigned int* doorTexture
	, unsigned int* trapDoorTexture
	, float* transform
	, float* scale
	, float* rotation);
bool FindChest(std::vector<Chest>& structures
	, float x
	, float y
	, int& index);
bool FindChest(std::vector<Chest>& structures
	, int* vertice);

void DrawChests(std::vector<Chest>& structures
	, Shader& sh
	, float* transform
	, unsigned int  openChestTex
	, unsigned int* structureDDs
	, unsigned int* structureTexs);

void DrawCraftStations(std::vector<CraftStation>& structures
	, Shader& sh
	, float* transform
	, unsigned int* structureDDs
	, unsigned int* structureTexs);
bool FindCraftStation(std::vector<CraftStation>& structures
	, float x
	, float y
	, int& index);
bool FindCraftStation(std::vector<CraftStation>& structures
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

bool isAnythingOnThisTransform(int x
	, int y
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Door>& doors
	, std::vector<Chest>& chests);
bool isAnythinginArea(int* vertices
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<tree>& trees
	, std::vector<CraftStation>& craftingStations
	, std::vector<Door>& doors
	, std::vector<Chest>& chests);
void CreateStructure(int StructureID
	, int x
	, int y
	, int directionLook
	, unsigned int* structuresTex
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedlings
	, std::vector<CraftStation>& craftStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors);