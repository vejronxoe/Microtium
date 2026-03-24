#include "Font.h"

#include<iostream>
#include<fstream>

#include "../libraries/json.hpp"


void LoadFont(std::vector<Letter>& letters
	, const char* fileName)
{
	std::ifstream f(fileName);
	nlohmann::json j = nlohmann::json::parse(f);
	float atlasWidth = j["atlas"]["width"];
	float atlasHeight = j["atlas"]["height"];

	auto glyphs = j["glyphs"];

	letters.resize(glyphs.size());

	for (int i = 0; i < glyphs.size(); ++i)
	{
		const auto& glyph = glyphs[i];

		if (glyph.contains("atlasBounds"))
		{
			float pixelLeft = glyph["atlasBounds"]["left"];
			float pixelBottom = glyph["atlasBounds"]["bottom"];
			float pixelRight = glyph["atlasBounds"]["right"];
			float pixelTop = glyph["atlasBounds"]["top"];

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
