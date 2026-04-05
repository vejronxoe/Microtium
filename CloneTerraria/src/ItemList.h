#pragma once
#define GRAVITY -40
#define BlockSize 1.0f/(60.0f - (Window::gameZoom-50))

enum Items
{
	i_Nothing = 0
	, i_CopperPickaxe
	, i_CopperAxe
	, i_CopperHammer
	, i_CopperSword
	, i_IronOre
	, i_CopperOre
	, i_IronIngot
	, i_CopperIngot
	, i_CraftingTable
	, i_Forge
	, i_Anvil
	, i_Chest
	, i_Dirt
	, i_Ice
	, i_Asphalt
	, i_Platform
	, i_ForestPlank
	, i_WallDirt
	, i_WallIce
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
	, i_DoorBlock
};
enum Structures
{
	s_CraftingTable = 0
	, s_Forge
	, s_Anvil
	, s_Chest
	, s_Sapling
	, s_Gate
	, s_Door
	, s_TrapDoor

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