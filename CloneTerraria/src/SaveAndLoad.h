#pragma once
#include"BlocksAndWalls.h"
#include"structures.h"
#include"flora.h"
#include "Player.h"
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
bool Save(std::string path
	, Player& player);

bool Load(std::string path
	, Player& player
	, std::vector<damagedWood>& damagedWoods
	, std::vector<DamagedBlock>& damageblocks
	, std::vector<DamagedBlock>& damagedWalls
	, std::vector<Projectile>& projectiles
	, std::vector<Enemy>& enemies
	, std::vector<DroppedItem>& droppedItems
	, std::vector<Letter>& Ascii
	, unsigned int eob);