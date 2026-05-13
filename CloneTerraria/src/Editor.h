#pragma once
#include"Opengl/Shader.h"
#include "BlocksAndWalls.h"
#include"structures.h"
#include"flora.h"
#include"glfw/Font.h"
enum PlacingType
{
	brushType =0
	, selectType
};
struct Editor
{
	float m_Transform[2] = {40, 0};
	float m_Velocity[2] = {0,0};
	int m_Selected = 0;
	char m_placingType = brushType;
	bool m_Eraser = false;
	unsigned int m_SelectBoxDD = 0;
	unsigned int m_SelectBoxTex = 0;
	int m_FirstPointBox[2] = {};
	int m_SelectBoxSides[4] = {};
	bool m_BoxSelected = false;
	std::vector<std::vector<int>> m_CopiedBlocks;
	std::vector<std::vector<int>> m_CopiedWalls;
	Editor(unsigned int eob);
	void Update(float deltaTime
		, char cursorState
		, unsigned int* blocksTex
		, unsigned int* structureTex
		, std::vector<int>& ChunksToBulidBlocks
		, std::vector<std::vector<Block>>& blocks
		, std::vector<int>& ChunksToBulidWalls
		, std::vector<std::vector<Wall>>& Walls
		, std::vector<seedling>& saplings
		, std::vector<CraftStation>& CraftingStations
		, std::vector<Chest>& Chests
		, std::vector<Door>& Doors
		, std::vector<int>& SandsXs);
	void Draw(Shader& animSh
		, float* transform
		, float* scale);
	private:
	void CopyBlocksAndWalls( unsigned int* blocksTex
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<Wall>>& Walls);
};
enum drawDataHUD
{
	rightHUDSlots = 0
	, leftHUDSlots
	, rightItemSlots
	, InvOfChest
	, useSlotDD
	, defaultSlotUV
	, slotChestDefaultUV
};
struct EditorHUD
{
	unsigned int m_DDs[7] = {};
	unsigned int m_VBOs[7] = {};
	unsigned int m_EOBs[4] = {};
	unsigned int m_EOBSizes[4] = {};
	unsigned int m_SlotTexs = {};
	unsigned int m_Icons[3] = {};
	unsigned int m_SelectZoneTex = 0;
	bool m_OpenChest = false;
	int m_ChestIndex = 0;
	float m_Scroll = 0;
	int m_WantedScroll = 0;
	float m_SideLength = 0;
	float m_GapLength = 0;
	float m_SideLengthChest = 0;
	float m_GapLengthChest = 0;
	Text m_ChestAmount[50];
	EditorHUD(unsigned int eob
		, unsigned int backGroundTex);
	void Create(unsigned int eob
		, std::vector<Chest>& chests
		, std::vector<Letter>& ancii
		, bool first);
	int Update(float deltaTime
		, unsigned int eob
		, std::vector<Letter>& ancii
		, std::vector<Chest>& chests
		, Editor& editor);
	void Draw(Shader& Sh
		, Shader& fontSh
		, Editor editor
		, std::vector<Chest>& chests
		, unsigned int fontTexture
		, unsigned int* itemsTex
		, unsigned int* blockTex
		, float* transform);
};