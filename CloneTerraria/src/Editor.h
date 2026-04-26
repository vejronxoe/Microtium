#pragma once
#include"Opengl/Shader.h"
#include "BlocksAndWalls.h"
struct Editor
{
	float m_Transform[2] = {40, 0};
	float m_Velocity[2] = {0,0};
	unsigned int selectedBlock = 0;
	void Update(float deltaTime);

};
enum drawDataHUD
{
	rightBackground = 0
	, leftBackground
	, rightHUDSlots
	, leftHUDSlots
	, slot
	, defaultSlotUV
};
struct EditorHUD
{
	unsigned int m_DDs[6] = {};
	unsigned int m_VBOs[6] = {};
	unsigned int m_EOBs[2] = {};
	unsigned int m_EOBSizes[2] = {};
	unsigned int m_Textures[2] = {};
	unsigned int m_Scroll = 0;
	unsigned int m_WantedScroll = 0;
	float m_SideLength = 0;
	float m_GapLength = 0;
	EditorHUD(unsigned int eob
		, unsigned int backGroundTex);
	void Create(unsigned int eob
		, bool first);
	void Update(float deltaTime);
	void Draw(Shader& Sh
		, float* transform);
};