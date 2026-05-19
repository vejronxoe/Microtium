#pragma once
#include"BlocksAndWalls.h"
bool Load(std::string path
	, std::vector<std::vector<Block>>& blocks);

bool Save(std::string path
	, std::vector<std::vector<Block>>& blocks);

bool Load(std::string path
	, std::vector<std::vector<Wall>>& walls);

bool Save(std::string path
	, std::vector<std::vector<Wall>>& walls);