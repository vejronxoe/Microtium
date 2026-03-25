#pragma once
#include<vector>
#include<iostream>


struct Letter
{
	float DefalutSize[2] = {};
	float UVCoordinates[4] = {};
};
void LoadFont(std::vector<Letter>& letters
	, float& lineHeight
	, const char* fileName);
struct Format
{
	Format() = default;
	Format(int LastLetterOfFormat
		, float sizeOfLetter);
	int m_LastLetterOfFormat = 0;
	float m_SizeOfLetter = 0;
};
struct Text
{
	unsigned int m_DrawData;
	unsigned int m_VBO;
	unsigned int m_Tex;
	float m_color[4] = {};
	float m_Transform[2] = {};
	float m_TextVerticers[4] = {};

	Text(std::string Letters
		, std::vector<Format>& formats
		, std::vector<Letter>& Anscii
		, float lineHeight);
};