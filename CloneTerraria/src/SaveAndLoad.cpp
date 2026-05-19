#include"SaveAndLoad.h"

#include<iostream>
#include<fstream>
#include<filesystem>
#include<sstream>
#include"libraries/Crc32.h"

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
			return false;
		}
		fileRead.close();
		std::ifstream checkFile(pathAndName + std::to_string(i) + ".crc", std::ios::binary);
		if (!checkFile.good())
		{
			repair[i] = true;
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

bool LoadBlocks(std::string path
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
bool SaveBlocks(std::string path
	, std::vector<std::vector<Block>>& blocks)
{
	if (!SavingSafely(path + "Blocks"))
	{
		return false;
	}
		std::ofstream file(path+"Blocks0.dat", std::ios::binary | std::ios::trunc);
		if (!file.good())
		{
			return false;
		}
		for (int i = 0; i < Blocks::xMax;i++)
		{
			int cursor = Blocks::yMax - Blocks::yMin;
			for (int j = 0; j < blocks.at(i).size();j++)
			{
				if (blocks.at(i).at(j).m_Y <= Blocks::yMax && blocks.at(i).at(j).m_Y >= Blocks::yMin)
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
	
	return true;
}