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

struct EditorHUD
{
	unsigned int m_DDs[3] = {};
	unsigned int m_VBOs[3] = {};
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