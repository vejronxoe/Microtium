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
	, std::vector<std::vector<Block>>& blocks)
{
	if (!LoadingSafely(path +"Blocks"))
	{
		return false;
	}
	
	blocks.assign(Blocks::xMax, std::vector<Block> {});
	std::ifstream file(path+ "Blocks0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j =  Blocks::yMax; j >= Blocks::yMin ; j--)
		{
			int16_t type = -1;
			file.read(reinterpret_cast<char*>(&type), sizeof(type));
			switch (type)
			{
				
			case -1:
				break;
			default:
				blocks.at(i).emplace_back(type,j);
				break;
			}
		}
	}
	
	file.close();
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j =0; j < blocks.at(i).size();j++)
		{
			if (blocks.at(i).at(j).m_Y < 0)
			{
				break;
			}
			if (blocks.at(i).at(j).m_Type >= t_Dirt && blocks.at(i).at(j).m_Type <= t_FullGrass)
			{
				int sides[4] = {1,1,1,1};
				int fill;
				if (FindBlock(blocks,i-1, blocks.at(i).at(j).m_Y,fill))
				{
					sides[0] = 0;
				}
				if (FindBlock(blocks, i, blocks.at(i).at(j).m_Y-1, fill))
				{
					sides[1] = 0;

				}
				if (FindBlock(blocks, i+1, blocks.at(i).at(j).m_Y, fill))
				{
					sides[2] = 0;

				}
				if (FindBlock(blocks, i, blocks.at(i).at(j).m_Y+1, fill))
				{
					sides[3] = 0;

				}
				blocks.at(i).at(j).m_Type = sides[0] + sides[1] * 2 + sides[2] * 4 + sides[3] * 8;
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
		int cursor = Blocks::yMax - Blocks::yMin;
		for (int j = 0; j < blocks.at(i).size();j++)
		{
			if (blocks.at(i).at(j).m_Y <= Blocks::yMax && blocks.at(i).at(j).m_Y >= Blocks::yMin && blocks.at(i).at(j).m_Type != t_DoorBlock)
			{
				int y = blocks.at(i).at(j).m_Y - Blocks::yMin;
				while (cursor > y)
				{
					int16_t type = -1;
					file.write(reinterpret_cast<char*>(&type), sizeof(type));
					cursor--;
				}
				int16_t type = blocks.at(i).at(j).m_Type;
				file.write(reinterpret_cast<char*>(&type), sizeof(type));
				cursor--;

			}
		}
		while (cursor >= 0)
		{
			int16_t type = -1;
			file.write(reinterpret_cast<char*>(&type), sizeof(type));
			cursor--;
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
	, std::vector<std::vector<Wall>>& walls)
{
	if (!LoadingSafely(path + "Walls"))
	{
		return false;
	}

	walls.assign(Blocks::xMax, std::vector<Wall> {});
	std::ifstream file(path + "Walls0.dat", std::ios::binary);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		for (int j = Blocks::yMax; j >= Blocks::yMin; j--)
		{
			int16_t type = -1;
			file.read(reinterpret_cast<char*>(&type), sizeof(type));
			switch (type)
			{

			case -1:
				break;
			default:
				walls.at(i).emplace_back(type, j);
				break;
			}
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
	, std::vector<std::vector<Wall>>& walls)
{
	
	std::ofstream file(path + "Walls0.dat", std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		return false;
	}
	for (int i = 0; i < Blocks::xMax;i++)
	{
		int cursor = Blocks::yMax - Blocks::yMin;
		for (int j = 0; j < walls.at(i).size();j++)
		{
			if (walls.at(i).at(j).m_Y <= Blocks::yMax && walls.at(i).at(j).m_Y >= Blocks::yMin)
			{
				int y = walls.at(i).at(j).m_Y - Blocks::yMin;
				while (cursor > y)
				{
					int16_t type = -1;
					file.write(reinterpret_cast<char*>(&type), sizeof(type));
					cursor--;
				}
				int16_t type = walls.at(i).at(j).m_Type;
				file.write(reinterpret_cast<char*>(&type), sizeof(type));
				cursor--;

			}
		}
		while (cursor >= 0)
		{
			int16_t type = -1;
			file.write(reinterpret_cast<char*>(&type), sizeof(type));
			cursor--;
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
	, std::vector<tree>& trees
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
		uint8_t type = s_Sapling;
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
	for (int i = 0 ; i < trees.size(); i++)
	{
		uint8_t type = s_StructureSize + trees.at(i).m_PartOfTree;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		int16_t x = trees.at(i).m_Transform[0];
		file.write(reinterpret_cast<char*>(&x), sizeof(x));
		int16_t y = trees.at(i).m_Transform[1];
		file.write(reinterpret_cast<char*>(&y), sizeof(y));
		int16_t rotation = trees.at(i).m_Rotation;
		file.write(reinterpret_cast<char*>(&rotation), sizeof(rotation));
		int16_t itemDrop = trees.at(i).m_ItemDrop;
		file.write(reinterpret_cast<char*>(&itemDrop), sizeof(itemDrop));
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
	, std::vector<tree>& trees
	, std::vector<seedling>& sapling
	, unsigned int* structTex
	, unsigned int* treeTex
	, unsigned int* treeDD)
{
	chests.clear();
	doors.clear();
	trees.clear();
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
		case s_Sapling:
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
		case s_StructureSize + part_Crown:
		case s_StructureSize + part_SmallCrown:
		case s_StructureSize + part_Log:
		{
			int16_t x;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			totalSize -= sizeof(x);
			int16_t y;
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			totalSize -= sizeof(y);
			int16_t rotation;
			file.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
			totalSize -= sizeof(rotation);
			int16_t itemDrop;
			file.read(reinterpret_cast<char*>(&itemDrop), sizeof(itemDrop));
			totalSize -= sizeof(itemDrop);
			trees.emplace_back(treeTex[type - s_StructureSize],treeDD[type - s_StructureSize],itemDrop,35, type - s_StructureSize,x,y,rotation);
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
