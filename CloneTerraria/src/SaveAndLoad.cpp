#include"SaveAndLoad.h"

#include<iostream>
#include<fstream>
#include<filesystem>
#include<sstream>
#include"libraries/Crc32.h"
#include"ItemList.h"


bool LoadingSafely(std::string pathAndName)
{
	bool repair[2] = { false ,false };

	for (int i = 0; i < 2; i++)
	{
		uint32_t checkSum;
		uint32_t Sum;
		std::ifstream fileRead(pathAndName + std::to_string(i) + ".dat", std::ios::binary);
		if (!fileRead.good())
		{
			repair[i] = true;
			fileRead.close();
			continue;
		}

		fileRead.seekg(0, std::ifstream::end);
		int size = fileRead.tellg();
		fileRead.seekg(0, std::ifstream::beg);
		std::vector<uint8_t> data;
		data.resize(size);
		fileRead.read(reinterpret_cast<char*>(data.data()), size);
		checkSum = crc32_fast(data.data(), size);
		if (fileRead.gcount() != size)
		{
			fileRead.close();
			return false;
		}
		fileRead.close();
		std::ifstream checkFile(pathAndName + std::to_string(i) + ".crc", std::ios::binary);
		if (!checkFile.good())
		{
			repair[i] = true;
			checkFile.close();
			continue;

		}

		checkFile.read(reinterpret_cast<char*>(&Sum), sizeof(Sum));

		if (!repair[i])
		{
			repair[i] = Sum != checkSum;
		}
		if (!checkFile.good())
		{
			repair[i] = true;
			checkFile.close();
			continue;
		}
		checkFile.close();
	}
	int repairing = -1;
	if (repair[0] && repair[1])
	{
		std::cout << "error something happened to save files. they will be reset to default" << std::endl;


		// add default 
	}
	if (repair[0])
	{
		if (!std::filesystem::copy_file(pathAndName + "1.dat", pathAndName + "0.dat", std::filesystem::copy_options::overwrite_existing))
		{
			return false;
		}



	}
	else if (repair[1])
	{
		if (!std::filesystem::copy_file(pathAndName + "0.dat", pathAndName + "1.dat", std::filesystem::copy_options::overwrite_existing))
		{
			return false;
		}
	

	}
	return true;
}
bool SavingSafely(std::string pathAndName)
{
	{
		uint32_t checkSum;
		std::ifstream fileRead(pathAndName + "0.dat", std::ios::binary);
		if (!fileRead.good())
		{
			return false;
		}

		fileRead.seekg(0, std::ifstream::end);
		int size = fileRead.tellg();
		fileRead.seekg(0, std::ifstream::beg);
		std::vector<uint8_t> data;
		data.resize(size);
		fileRead.read(reinterpret_cast<char*>(data.data()), size);
		checkSum = crc32_fast(data.data(), size);
		if (fileRead.gcount() != size)
		{
			return false;
		}
		fileRead.close();
		std::ofstream checkFile(pathAndName + "0.crc", std::ios::binary | std::ios::trunc);
		if (!checkFile.good())
		{
			return false;
		}

		checkFile.write(reinterpret_cast<char*>(&checkSum), sizeof(checkSum));


		if (!checkFile.good())
		{
			return false;
		}
		checkFile.close();
	}
	if (!std::filesystem::copy_file(pathAndName + "0.dat", pathAndName + "1.dat", std::filesystem::copy_options::overwrite_existing))
	{
		return false;
	}
	{
		uint32_t checkSum;
		std::ifstream fileRead(pathAndName + "1.dat", std::ios::binary);
		if (!fileRead.good())
		{
			return false;
		}

		fileRead.seekg(0, std::ifstream::end);
		int size = fileRead.tellg();
		fileRead.seekg(0, std::ifstream::beg);
		std::vector<uint8_t> data;
		data.resize(size);
		fileRead.read(reinterpret_cast<char*>(data.data()), size);
		checkSum = crc32_fast(data.data(), size);
		if (fileRead.gcount() != size)
		{
			return false;
		}
		fileRead.close();
		std::ofstream checkFile(pathAndName + "1.crc", std::ios::binary | std::ios::trunc);
		if (!checkFile.good())
		{
			return false;
		}

		checkFile.write(reinterpret_cast<char*>(&checkSum), sizeof(checkSum));


		if (!checkFile.good())
		{
			return false;
		}
		checkFile.close();
	}
	return true;
	
}
bool Load(std::string path
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& sandXs)
{
	if (!LoadingSafely(path +"Blocks"))
	{
		return false;
	}
	sandXs.clear();
	{
		std::vector<Block> fill;
		fill.assign(Blocks::yMax - Blocks::yMin,0);
		blocks.assign(Blocks::xMax, fill);
	}

	std::ifstream file(path+ "Blocks0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j = Blocks::yMax-1; j >= Blocks::yMin; j--)
		{
			uint8_t type = 0;
			file.read(reinterpret_cast<char*>(&type), sizeof(type));

			blocks.at(i).at(j - Blocks::yMin).Create(type);

			if (type == t_Sand)
			{
				bool isAlreadyThere = false;
				for (int k = 0; k < sandXs.size(); k++)
				{
					if (sandXs.at(k) == i)
					{
						isAlreadyThere = true;
						break;
					}
				}
				if (!isAlreadyThere)
				{
					sandXs.emplace_back(i);
				}
			}

		}
	}
	
	file.close();
	for (int i = 3; i < Blocks::xMax-3;i++)
	{
		for (int j = 0; j < Blocks::yMax-1;j++)
		{
			if (blocks.at(i).at(j - Blocks::yMin).m_Type >= t_Dirt && blocks.at(i).at(j - Blocks::yMin).m_Type <= t_FullGrass)
			{
				int sides[4] = {1,1,1,1};
				int fill;
				if (blocks.at(i - 1).at(j- Blocks::yMin).m_Behavior != b_Air)
				{
					sides[0] = 0;
				}
				if (blocks.at(i ).at(j - 1 - Blocks::yMin).m_Behavior != b_Air)
				{
					sides[1] = 0;

				}
				if (blocks.at(i + 1).at(j - Blocks::yMin).m_Behavior != b_Air)
				{
					sides[2] = 0;

				}
				if (blocks.at(i).at(j + 1 - Blocks::yMin).m_Behavior != b_Air)
				{
					sides[3] = 0;

				}
				blocks.at(i).at(j - Blocks::yMin).m_Type = sides[0] + sides[1] * 2 + sides[2] * 4 + sides[3] * 8 + 1;
			}
		}
	}
	return true;
}
bool Save(std::string path
	, std::vector<std::vector<Block>>& blocks)
{

	std::ofstream file(path + "Blocks0.dat", std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j = Blocks::yMax - Blocks::yMin-1 ; j >= 0; j--)
		{

			uint8_t type = blocks.at(i).at(j).m_Type;
			file.write(reinterpret_cast<char*>(&type), sizeof(type));
		}

	}
	if (!file.good())
	{
		return false;
	}


	file.close();
	if (!SavingSafely(path + "Blocks"))
	{
		return false;
	}
	return true;
}
bool Load(std::string path
	, std::vector<std::vector<uint8_t>>& walls)
{
	if (!LoadingSafely(path + "Walls"))
	{
		return false;
	}
	{
		std::vector<uint8_t> fill;
		fill.assign(Blocks::yMax - Blocks::yMin, 0);
		walls.assign(Blocks::xMax, fill);
	}
	std::ifstream file(path + "Walls0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j = Blocks::yMax-1; j >= Blocks::yMin; j--)
		{

			uint8_t type = 0;
			file.read(reinterpret_cast<char*>(&type), sizeof(type));
			walls.at(i).at(j - Blocks::yMin) = type;
			
		}
	}
	if (!file.good())
	{
		return false;
	}
	file.close();
	return true;
}

bool Save(std::string path
	, std::vector<std::vector<uint8_t>>& walls)
{
	
	std::ofstream file(path + "Walls0.dat", std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j = Blocks::yMax - Blocks::yMin - 1; j >= 0; j--)
		{
			uint8_t type = walls.at(i).at(j);
			file.write(reinterpret_cast<char*>(&type), sizeof(type));
		}

	}
	if (!file.good())
	{
		return false;
	}

	file.close();
	if (!SavingSafely(path + "Walls"))
	{
		return false;
	}
	return true;
}

bool Save(std::string path
	, std::vector<CraftStation>& craftStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<Crown>& Crowns
	, std::vector<seedling>& sapling)
{
	std::ofstream file(path + "Struct0.dat", std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0;i <craftStations.size();i++)
	{
		uint8_t type = craftStations.at(i).m_CraftStationtype;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = craftStations.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = craftStations.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));
		int8_t lookAt = craftStations.at(i).m_LookAt;
		file.write(reinterpret_cast<char*>(&lookAt), sizeof(lookAt));
	}
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < sapling.size();i++)
	{
		uint8_t type = sapling.at(i).m_Type;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = sapling.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = sapling.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));

	}
	if (!file.good())
	{
		return false;
	}
	for (int i = 0;i < doors.size(); i++)
	{
		uint8_t type = doors.at(i).m_Type;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = doors.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = doors.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));
		
	}
	if (!file.good())
	{
		return false;
	}
	
	for (int i = 0;i < chests.size(); i++)
	{
		uint8_t type = s_Chest;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = chests.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = chests.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));
		for (int j = 0; j < 50; j++)
		{
			uint16_t item = chests.at(i).m_Items[j];
			file.write(reinterpret_cast<char*>(&item), sizeof(item));

			uint16_t amount = chests.at(i).m_amount[j];
			file.write(reinterpret_cast<char*>(&amount), sizeof(amount));
		}
	}
	if (!file.good())
	{
		return false;
	}
	for (int i = 0 ; i < Crowns.size(); i++)
	{
		uint8_t type = s_StructureSize + Crowns.at(i).m_Type;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = Crowns.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = Crowns.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));
		int8_t rotation = Crowns.at(i).m_Rotation;
		file.write(reinterpret_cast<char*>(&rotation), sizeof(rotation));
	}
	if (!file.good())
	{
		return false;
	}
	file.close();
	if (!SavingSafely(path + "Struct"))
	{
		return false;
	}
	return true;
}
bool Load(std::string path
	, std::vector<std::vector<Block>>& blocks
	, std::vector<CraftStation>& craftStations
	, std::vector<Chest>& chests
	, std::vector<Door>& doors
	, std::vector<Crown>& Crowns
	, std::vector<seedling>& sapling
	, unsigned int* structTex
	, unsigned int* CrownTex
	, unsigned int* CrownDD)
{
	chests.clear();
	doors.clear();
	Crowns.clear();
	craftStations.clear();
	sapling.clear();
	if (!LoadingSafely(path +"Struct"))
	{
		return false;
	}
	std::uintmax_t totalSize = std::filesystem::file_size(path + "Struct0.dat");
	std::ifstream file(path + "Struct0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	while (totalSize > 0)
	{
		uint8_t type;
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		totalSize -= sizeof(type);
		switch (type)
		{
		case s_Anvil:
		case s_CraftingTable:
		case s_Forge:
		{
			int16_t x;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			totalSize -= sizeof(x);
			int16_t y;
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			totalSize -= sizeof(y);
			int8_t lookAt;
			file.read(reinterpret_cast<char*>(&lookAt), sizeof(lookAt));
			totalSize -= sizeof(lookAt);
			CreateStructure(type,x,y,lookAt,structTex,blocks,sapling,craftStations,chests,doors);
			break;
		}
		case s_Gate:
		case s_Door:
		case s_TrapDoor:
		case s_ForestSapling:
		case s_SnowSapling:
		case s_CactusSapling:
		{
			int16_t x;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			totalSize -= sizeof(x);
			int16_t y;
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			totalSize -= sizeof(y);
			CreateStructure(type, x, y, 0, structTex, blocks, sapling, craftStations, chests, doors);

			break;
		}
		case s_Chest:
		{
			int16_t x;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			totalSize -= sizeof(x);
			int16_t y;
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			totalSize -= sizeof(y);
			chests.emplace_back(x, y, blocks);
			for (int j = 0; j < 50; j++)
			{
				uint16_t item;
				file.read(reinterpret_cast<char*>(&item), sizeof(item));
				totalSize -= sizeof(item);
				uint16_t amount;
				file.read(reinterpret_cast<char*>(&amount), sizeof(amount));
				totalSize -= sizeof(amount);
				chests.at(chests.size() - 1).m_Items[j] = item;
				chests.at(chests.size() - 1).m_amount[j] = amount;
			}
			break;
		}
		case s_StructureSize + crown_Forest:
		case s_StructureSize + crown_ForestSmall:
		{
			int16_t x;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			totalSize -= sizeof(x);
			int16_t y;
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			totalSize -= sizeof(y);
			int8_t rotation;
			file.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
			totalSize -= sizeof(rotation);
			Crowns.emplace_back(x,y,rotation, type - s_StructureSize);
			break;
		}
		default:
			std::cout << "error unknow struct fail to load" << std::endl;
			return false;
			break;
		}
		if (!file.good())
		{
			return false;
		}
	}
	return true;
}
bool Save(std::string path
	, Player& player)
{
	std::ofstream file(path + "Player0.dat", std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		return false;
	}
	float x = player.m_Transform[0];
	file.write(reinterpret_cast<char*>(&x), sizeof(x));
	float y = player.m_Transform[1];
	file.write(reinterpret_cast<char*>(&y), sizeof(y));
	float VX = player.m_Velocity[0];
	file.write(reinterpret_cast<char*>(&VX), sizeof(VX));
	float VY = player.m_Velocity[1];
	file.write(reinterpret_cast<char*>(&VY), sizeof(VY));
	int16_t HP = player.m_CurrentHealth;
	file.write(reinterpret_cast<char*>(&HP), sizeof(HP));
	int16_t maxHP = player.m_maxHealth;
	file.write(reinterpret_cast<char*>(&maxHP), sizeof(maxHP));
	int8_t useSlot = player.m_UseSlot;
	file.write(reinterpret_cast<char*>(&useSlot), sizeof(useSlot));
	int8_t useHUDSlot = player.m_HUDUseSlot;
	file.write(reinterpret_cast<char*>(&useHUDSlot), sizeof(useHUDSlot));
	float lastY = player.m_LastStandingY;
	file.write(reinterpret_cast<char*>(&lastY), sizeof(lastY));
	for (int i = 0; i < 60;i++)
	{
		int16_t item = player.m_PlayerSlots[i];
		file.write(reinterpret_cast<char*>(&item), sizeof(item));
	}
	for (int i =0;i < 52;i++)
	{
		int16_t amount = player.m_AmountInSlots[i];
		file.write(reinterpret_cast<char*>(&amount), sizeof(amount));
	}
	if (!file.good())
	{
		return false;
	}
	file.close();
	if (!SavingSafely(path + "Player"))
	{
		return false;
	}
	return true;
}
bool Load(std::string path
	, Player& player
	, std::vector<damagedWood>& damagedWoods
	, std::vector<DamagedBlock>& damageblocks
	, std::vector<DamagedBlock>& damagedWalls
	, std::vector<Projectile>& projectiles
	, std::vector<Enemy>& enemies
	, std::vector<DroppedItem>& droppedItems
	, std::vector<Letter>& Ascii
	, unsigned int eob)
{
	damagedWoods.clear();
	damageblocks.clear();
	damagedWalls.clear();
	projectiles.clear();
	enemies.clear();
	droppedItems.clear();
	player.clear();
	if (!LoadingSafely(path+"Player"))
	{
		return false;
	}
	std::ifstream file(path + "Player0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	
	float x;
	file.read(reinterpret_cast<char*>(&x), sizeof(x));
	player.m_Transform[0] = x;

	float y;
	file.read(reinterpret_cast<char*>(&y), sizeof(y));
	player.m_Transform[1] = y;

	float VX;
	file.read(reinterpret_cast<char*>(&VX), sizeof(VX));
	player.m_Velocity[0] = VX;

	float VY;
	file.read(reinterpret_cast<char*>(&VY), sizeof(VY));
	player.m_Velocity[1] = VY;

	int16_t HP;
	file.read(reinterpret_cast<char*>(&HP), sizeof(HP));
	player.m_CurrentHealth = HP;
	
	int16_t maxHP;
	file.read(reinterpret_cast<char*>(&maxHP), sizeof(maxHP));
	player.m_maxHealth = maxHP;
	
	int8_t useSlot;
	file.read(reinterpret_cast<char*>(&useSlot), sizeof(useSlot));
	player.m_UseSlot = useSlot;
	
	int8_t useHUDSlot;
	file.read(reinterpret_cast<char*>(&useHUDSlot), sizeof(useHUDSlot));
	player.m_HUDUseSlot = useHUDSlot;
	
	float lastY;
	file.read(reinterpret_cast<char*>(&lastY), sizeof(lastY));
	player.m_LastStandingY = lastY;
	
	for (int i = 0; i < 52;i++)
	{
		int16_t item;
		file.read(reinterpret_cast<char*>(&item), sizeof(item));
		player.m_PlayerSlots[i] = item;
	}
	int hold = player.m_PlayerSlots[0];
	player.m_PlayerSlots[0] = i_Nothing;
	for (int i = 0; i < 4;i++)
	{
		int16_t item;
		file.read(reinterpret_cast<char*>(&item), sizeof(item));
		player.m_PlayerSlots[0] = item;
		player.SwapArmor(0,i);
	}

	for (int i = 0; i < 4 ;i++)
	{
		int16_t item;
		file.read(reinterpret_cast<char*>(&item), sizeof(item));
		player.m_PlayerSlots[0] = item;
		player.SwapAccessorise(0,56+i);
	}
	player.m_PlayerSlots[0] = hold;

	for (int i = 0;i < 52;i++)
	{
		int16_t amount;
		file.read(reinterpret_cast<char*>(&amount), sizeof(amount));
		player.m_AmountInSlots[i] = amount;
	}
	
	player.SwapItemStats();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			player.ChangeAmountText(player.m_AmountText[i * 10 + j], Ascii, eob, 0, player.m_AmountInSlots[i * 10 + j + 1], player.m_InvOffset[0] + player.m_SlotGap * j, player.m_InvOffset[1] - player.m_HalfOfSlotLeanght - player.m_SlotGap * i);
		}
	}
	player.ChangeAmountText(player.m_AmountText[50], Ascii, eob, 0, player.m_AmountInSlots[51], player.m_InvOffset[0] + player.m_SlotGap * 9, player.m_InvOffset[1] - player.m_HalfOfSlotLeanght - player.m_SlotGap * 5);

	if (!file.good())
	{
		return false;
	}
	file.close();
}