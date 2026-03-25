#include "Font.h"

#include<iostream>
#include<fstream>

#include "../math/VectorOperation.h"
#include "window.h"
#include "../libraries/json.hpp"
#define BEGINLETTER 32	

void LoadFont(std::vector<Letter>& letters
	, float& lineHeight
	, const char* fileName)
{
	std::ifstream f(fileName);
	nlohmann::json j = nlohmann::json::parse(f);
	float atlasWidth = j["atlas"]["width"];
	float atlasHeight = j["atlas"]["height"];
	float sizeDefalut = GetVisibleScale(60,120,Window::height,Window::width);
	float pixelDefalut = 16;
	auto glyphs = j["glyphs"];
	if('A' - BEGINLETTER < glyphs.size())
	{
		
		float pixelLeft = glyphs['A' - BEGINLETTER]["atlasBounds"]["left"];
		float pixelBottom = glyphs['A' - BEGINLETTER]["atlasBounds"]["bottom"];
		float pixelRight = glyphs['A' - BEGINLETTER]["atlasBounds"]["right"];
		float pixelTop = glyphs['A' - BEGINLETTER]["atlasBounds"]["top"];
		pixelDefalut = pixelTop - pixelBottom;

	}

	letters.resize(glyphs.size());
	lineHeight = sizeDefalut;
	for (int i = 0; i < glyphs.size(); ++i)
	{
		const auto& glyph = glyphs[i];

		if (glyph.contains("atlasBounds"))
		{
			float pixelLeft = glyph["atlasBounds"]["left"];
			float pixelBottom = glyph["atlasBounds"]["bottom"];
			float pixelRight = glyph["atlasBounds"]["right"];
			float pixelTop = glyph["atlasBounds"]["top"];
			letters[i].DefalutSize[0] = sizeDefalut * (pixelRight - pixelLeft) / pixelDefalut;
			letters[i].DefalutSize[1] = sizeDefalut * (pixelTop - pixelBottom) / pixelDefalut;
			if (lineHeight < letters[i].DefalutSize[1])
			{
				lineHeight = letters[i].DefalutSize[1];
			}
			letters[i].UVCoordinates[0] = pixelLeft / atlasWidth;
			letters[i].UVCoordinates[1] = 1.0f - (pixelBottom / atlasHeight);
			letters[i].UVCoordinates[2] = pixelRight / atlasWidth;
			letters[i].UVCoordinates[3] = 1.0f - (pixelTop / atlasHeight);   

		}
		else
		{
			
			letters[i].UVCoordinates[0] = -1;
			letters[i].UVCoordinates[1] = -1;
			letters[i].UVCoordinates[2] = -1;
			letters[i].UVCoordinates[3] = -1;
		}
	}
	
}
Format::Format(int LastLetterOfFormat
	,float sizeOfLetter)
{
	m_LastLetterOfFormat = 0;
	m_SizeOfLetter = 0;
}

Text::Text(std::string Letters
	, std::vector<Format>& formats
	, std::vector<Letter>& Anscii
	, float lineHeight)
{
	assert(formats.size() == 0);
	assert(Letters.length() == 0);
	for (int i = 0; formats.size() ;i++)
	{
		Clamp(formats.at(i).m_LastLetterOfFormat, 0, Letters.length());
	}
	for (int i = 0; i < formats.size(); i++)
	{
		for (int j = 1; j < formats.size(); j++)
		{
			if (formats.at(j-1).m_LastLetterOfFormat > formats.at(j).m_LastLetterOfFormat)
			{
				Format holder = formats[j - 1];
				formats[j - 1] = formats[j];
				formats[j] = holder;

			}
		}
	}

	for (int i = 0;i < Letters.length(); i++)
	{
		int sizeOfLetter = 1;
		for (int j = 0; j < formats.size();j++)
		{
			if (i < formats.at(j).m_LastLetterOfFormat)
			{
				sizeOfLetter = formats[j].m_SizeOfLetter;
			}
		}




	}

}