#pragma once
#include<vector>
struct Letter
{
	float UVCoordinates[4] = {};
};
void LoadFont(std::vector<Letter>& letters
	, const char* fileName);