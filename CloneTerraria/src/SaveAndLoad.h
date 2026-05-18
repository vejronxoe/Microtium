#pragma once
#include"BlocksAndWalls.h"


bool LoadBlocks(std::string path
	, std::vector<std::vector<Block>>& blocks);

bool SaveBlocks(std::string path
	, std::vector<std::vector<Block>>& blocks);