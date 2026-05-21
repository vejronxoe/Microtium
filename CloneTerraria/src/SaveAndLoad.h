#pragma once
#include"BlocksAndWalls.h"
#include"structures.h"
#include"flora.h"
bool Load(std::string path
	, std::vector<std::vector<Block>>& blocks);

bool Save(std::string path
	, std::vector<std::vector<Block>>& blocks);

bool Load(std::string path
	, std::vector<std::vector<Wall>>& walls);

bool Save(std::string path
	, std::vector<std::vector<Wall>>& walls);
bool Save(std::string path
	, std::vector<CraftStation>& craftStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<tree>& trees
	, std::vector<seedling>& sapling);
bool Load(std::string path
	, std::vector<std::vector<Block>>& blocks
	, std::vector<CraftStation>& craftStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<tree>& trees
	, std::vector<seedling>& sapling
	, unsigned int* structTex
	, unsigned int* treeTex
	, unsigned int* treeDD);