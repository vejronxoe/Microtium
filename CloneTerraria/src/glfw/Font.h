#pragma once
#include<vector>
#include<iostream>
#include"../Opengl/Shader.h"

enum StablePoint
{
	leftTop = 0
	, leftMiddle
	, leftBottom
	, middleTop
	, middleMiddle
	, middleBottom
	, rightTop
	, rightMiddle
	, rightBottom
};
struct Letter
{
	float m_DefalutSize[2] = {};
	float m_UVCoordinates[4] = {};
};
void LoadFont(std::vector<Letter>& letters
	, float& lineHeight
	, const char* fileName);
struct Format
{
	Format() = default;
	Format(int LastLetterOfFormat
		, float sizeOfLetter
		, float red
		, float green
		, float blue
		, float alpha);
	float m_Color[4] = {1,1,1,1};
	int m_LastLetterOfFormat = 0;
	float m_SizeOfLetter = 0;
};
struct Text
{
	unsigned int m_DrawData;
	unsigned int m_VBO;
	unsigned int m_EOB;
	int m_OrderSize;
	float m_Transform[2] = {};
	float m_TextVertices[4] = {};

	Text(std::string Letters
		, std::vector<Format> formats
		, std::vector<Letter>& Anscii
		, float lineHeight
		, char stablePoint
		, float x
		, float y);
	void Draw(Shader& fontSh
		,float* transform
		,unsigned int fontTex);
	void deleteText();
};