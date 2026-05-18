#include"SaveAndLoad.h"

#include<iostream>
#include<fstream>
#include<filesystem>
#include<sstream>


bool LoadBlocks(std::string path
	, std::vector<std::vector<Block>>& blocks)
{
	path += "Blocks0.dat";
	blocks.assign(Blocks::xMax, std::vector<Block> {});
	std::ifstream file(path, std::ios::binary);
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
	if (!file.good())
	{
		return false;
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
		
		bool copied = std::filesystem::copy_file(path + "Blocks0.dat", path + "Blocks1.dat", std::filesystem::copy_options::overwrite_existing);
		
	return copied;
}