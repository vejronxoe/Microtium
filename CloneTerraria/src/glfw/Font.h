#pragma once
#include<vector>
#include<iostream>
#include"../Opengl/Shader.h"


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

	unsigned int m_BackgroundDrawData =-1;
	unsigned int m_BackgroundVBO = -1;
	unsigned int m_DrawData = -1;
	unsigned int m_VBO = -1;
	unsigned int m_EOB = -1;
	int m_OrderSize = -1;
	float m_Transform[2] = {};
	float m_TextVertices[4] = {};
	Text() = default;
	Text(std::string Letters
		, std::vector<Format> formats
		, std::vector<Letter>& Anscii
		, unsigned int eob
		, float lineHeight
		, char stablePoint
		, float x
		, float y);
	void CreateText(std::string Letters
		, std::vector<Format> formats
		, std::vector<Letter>& Anscii
		, unsigned int eob
		, float lineHeight
		, char stablePoint
		, float x
		, float y);
	void Draw(Shader& fontSh
		, Shader& basicSh
		,float* transform
		,unsigned int fontTex
		, unsigned int BackgroundTex
		, bool BackGround);
	void deleteText();
};