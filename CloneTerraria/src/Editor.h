#pragma once
#include"Opengl/Shader.h"
#include "BlocksAndWalls.h"

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
	std::vector<std::vector<Block>> m_BlocksInBox;
	Editor(unsigned int eob);
	void Update(float deltaTime
		, char cursorState);
	void Draw(Shader& basic
		, float* transform
		, float* scale);
};
enum drawDataHUD
{
	rightHUDSlots = 0
	, leftHUDSlots
	, useSlotDD
	, defaultSlotUV
};
struct EditorHUD
{
	unsigned int m_DDs[4] = {};
	unsigned int m_VBOs[4] = {};
	unsigned int m_EOBs[2] = {};
	unsigned int m_EOBSizes[2] = {};
	unsigned int m_SlotTexs = {};
	unsigned int m_Icons[3] = {};
	unsigned int m_SelectZoneTex = 0;
	float m_Scroll = 0;
	int m_WantedScroll = 0;
	float m_SideLength = 0;
	float m_GapLength = 0;
	EditorHUD(unsigned int eob
		, unsigned int backGroundTex);
	void Create(unsigned int eob
		, bool first);
	int Update(float deltaTime, Editor& editor);
	void Draw(Shader& Sh
		, Editor editor
		, unsigned int* itemsTex
		, unsigned int* blockTex
		, float* transform);
};