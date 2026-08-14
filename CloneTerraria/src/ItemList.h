#pragma once
#include <cmath>
#define GRAVITY -40
#define BlockSize (1.0f/(5.0f + 75.0f * Window::gameZoom))
#define TEXSLOTDISTANCE 1.0f/10.0f

enum Items
{
	i_Nothing = 0
	, i_CopperPickaxe
	, i_CopperAxe
	, i_CopperHammer
	, i_CopperSword
	, i_IronIngot
	, i_CopperIngot
	, i_CraftingTable
	, i_Forge
	, i_Anvil
	, i_Chest
	, i_Dirt
	, i_Ice
	, i_Asphalt
	, i_ForestPlank
	, i_Platform
	, i_WallDirt/////////
	, i_WallForestPlank
	, i_WallCloude
	, i_WallTerracotta
	, i_WallStone
	, i_WallStoneBrick
	, i_WallHardStone
	, i_WallHardStoneBrick
	, i_WallSandBrick
	, i_WallSandStone
	, i_WallSnow
	, i_WallSnowPlank
	, i_WallCactusPlank
	, i_WallIce////////
	, i_Sand
	, i_Sapling
	, i_WoodBow
	, i_BasicArrow
	, i_PierceArrow
	, i_BouncingArrow
	, i_FireArrow
	, i_Cannon
	, i_BasicCannonBall
	, i_PierceCannonBall
	, i_BouncingCannonBall
	, i_FireCannonBall
	, i_Pistol
	, i_BasicBullet
	, i_PierceBullet
	, i_BouncingBullet
	, i_FireBullet
	, i_WoodHelmet
	, i_WoodChestPlate
	, i_WoodPants
	, i_WoodShoes
	, i_AccessoriseArrowBag
	, i_AccessoriseWallClimb
	, i_AccessoriseShackle
	, i_AccessoriseFastShoes
	, i_Door
	, i_TrapDoor
	, i_Gate
	, i_IronOre
	, i_CopperOre
	, i_AdamantiteOre
	, i_GoldOre
	, i_Stone
	, i_CloudeBlock
	, i_TitanOre
	, i_StoneBrick
	, i_HardStone
	, i_HardStoneBrick
	, i_SandStone
	, i_SandBrick
	, i_Terracotta
	, i_SnowPlatform
	, i_SandPlatform
	, i_Snow
	, i_HeartGem
	, i_SnowPlank
	, i_CactusPlank
	, i_SnowSapling
	, i_CactusSapling
	, i_WorkBench
	, i_AlchemyTable
	, i_Lathe
	, i_ItemSize
};
enum Structures
{
	s_CraftingTable = 0
	, s_AlchemyTable
	, s_WorkBench
	, s_Lathe
	, s_Forge
	, s_Anvil
	, s_Chest
	, s_ForestSapling
	, s_SnowSapling
	, s_CactusSapling
	, s_Gate
	, s_Door
	, s_TrapDoor
	,s_StructureSize

};
enum GameState
{
	stateMainMenu = 0
	, stateInGame
	, stateEditor
};
enum MenuState
{
	stateDefault = 0
	, stateLoad
	, stateSave
	, stateOptions
	, stateNone
};
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
void memoryDefender(int* vertices
	, int lenghtOfArray);