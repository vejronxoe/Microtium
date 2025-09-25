#include"Blocks.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
#include"Opengl/drawData.h"
#include"math/matrix.h"


Block::Block(unsigned int tex, int x, int y, bool hitboxActive, unsigned int behavior)
	: m_te(tex), m_Transform{x,y}, m_CollisionActive(hitboxActive), m_BlockBehavior(behavior)
{}
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob)
{
	blocksDrawData = CreateDrawData(eob,0.5f,-0.5,-0.5f, 0.5f,1,0,0,1);
}

void Block::DrawBlock( Shader& basicShader, unsigned int location, float* transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(location, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_te));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	
}


enum TexturesOfBlocks 
{
	TopGrass = 0, 
	LeftGrass,
	DownGrass,
	RightGrass,
	TopDownGrass,
	LeftRightGrass,
	TopLeftGrass,
	DownLeftGrass,
	DownRightGrass,
	TopRightGrass,
	MissingTopGrass,
	MissingLeftGrass,
	MissingDownGrass,
	MissingRightGrass,
	FullGrass,
	Dirt,
	Ice,
	Asphalt,
	Platform,

};
void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[TopGrass] = CreateTextureRGBA("res/textures/topGrassBlock.png");
	IDs[LeftGrass] = CreateTextureRGBA("res/textures/leftGrassBlock.png");
	IDs[DownGrass] = CreateTextureRGBA("res/textures/downGrassBlock.png");
	IDs[RightGrass] = CreateTextureRGBA("res/textures/rightGrassBlock.png");
	IDs[TopDownGrass] = CreateTextureRGBA("res/textures/topDownGrassBlock.png");
	IDs[LeftRightGrass] = CreateTextureRGBA("res/textures/leftRightGrassBlock.png");
	IDs[TopLeftGrass] = CreateTextureRGBA("res/textures/topLeftGrassBlock.png");
	IDs[DownLeftGrass] = CreateTextureRGBA("res/textures/downLeftGrassBlock.png");
	IDs[DownRightGrass] = CreateTextureRGBA("res/textures/downRightGrassBlock.png");
	IDs[TopRightGrass] = CreateTextureRGBA("res/textures/topRightGrassBlock.png");
	IDs[MissingTopGrass] = CreateTextureRGBA("res/textures/missingTopGrassBlock.png");
	IDs[MissingLeftGrass] = CreateTextureRGBA("res/textures/missingLeftGrassBlock.png");
	IDs[MissingDownGrass] = CreateTextureRGBA("res/textures/missingDownGrassBlock.png");
	IDs[MissingRightGrass] = CreateTextureRGBA("res/textures/missingRightGrassBlock.png");
	IDs[FullGrass] = CreateTextureRGBA("res/textures/fullGrassBlock.png");
	IDs[Dirt] = CreateTextureRGBA("res/textures/dirtBlock.png");
	IDs[Ice] = CreateTextureRGBA("res/textures/ice.png");
	IDs[Asphalt] = CreateTextureRGBA("res/textures/Asphalt.png");
	IDs[Platform] = CreateTextureRGBA("res/textures/platform.png");
}


unsigned int GrassBlockTextureSelector(unsigned int *TexturesIDs, std::vector<std::string>& lines, int i , int j)
{
	if (lines.at(i - 1).at(j) == ' ')
	{
		if (lines.at(i + 1).at(j) == ' ')
		{
			if (lines.at(i).at(j - 1) == ' ')
			{
				if (lines.at(i).at(j + 1) == ' ')
				{
					return TexturesIDs[FullGrass];
				}
				else
				{
					return TexturesIDs[MissingRightGrass];
				}
			}
			else if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[MissingLeftGrass];
			}
			else
			{
				return TexturesIDs[TopDownGrass];
			}
		}
		else if (lines.at(i).at(j - 1) == ' ')
		{
			if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[MissingDownGrass];
			}
			else
			{
				return TexturesIDs[TopLeftGrass];
			}
		}
		else if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[TopRightGrass];
		}
		else
		{
			return TexturesIDs[TopGrass];
		}
	}
	else if (lines.at(i + 1).at(j) == ' ')
	{
		if (lines.at(i).at(j - 1) == ' ')
		{
			if (lines.at(i).at(j + 1) == ' ')
			{
				return TexturesIDs[MissingTopGrass];
			}
			else
			{
				return TexturesIDs[DownLeftGrass];
			}
		}
		else if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[DownRightGrass];
		}
		else
		{
			return TexturesIDs[DownGrass];
		}
	}
	else if (lines.at(i).at(j - 1) == ' ')
	{
		if (lines.at(i).at(j + 1) == ' ')
		{
			return TexturesIDs[LeftRightGrass];
		}
		else
		{
			return TexturesIDs[LeftGrass];
		}
	}
	else if (lines.at(i).at(j + 1) == ' ')
	{
		return TexturesIDs[RightGrass];
	}
	else
	{
		return TexturesIDs[Dirt];
	}
}
void LoadMap(const char* filepath, std::vector<Block>& blocks, unsigned int* texturesIDs)
{
	std::ifstream map(filepath);
	std::vector<std::string> lines;
	if (!map)
	{
		std::cout << "can not open map file" << filepath << std::endl;
	}
	else
	{

		{
			lines.emplace_back(" ");
			lines.emplace_back(" ");
			int i = 1;
			while (std::getline(map, lines[i]))
			{
				lines.at(i) = " " + lines.at(i) + " ";
				lines.emplace_back(" ");
				i++;
			}

		}
		int maxLenght = 1;
		for (int i = 0; i < lines.size(); i++)
		{
			if (maxLenght < lines.at(i).length())
			{
				maxLenght = lines.at(i).length();
			}
		}
		for (int i = 0; i < lines.size(); i++)
		{
			while (lines.at(i).length() < maxLenght)
			{
				lines.at(i) += " ";
			}
		}
		map.close();
		int y = 0.0f;
		for (int i = 0; i < lines.size(); i++)
		{
			int x = -56;
			{
				for (int j = 0; j < lines.at(i).length(); j++)
				{
					switch (lines.at(i).at(j))
					{
					case'd':
						blocks.emplace_back(GrassBlockTextureSelector(texturesIDs, lines, i, j), x, y, true,basicSolid);
						break;
					case'p':
						blocks.emplace_back(texturesIDs[Platform], x, y, true,platform);
						break;
					case'a':
						blocks.emplace_back(texturesIDs[Asphalt], x, y, true, asphalt);
						break;
					case'i':
						blocks.emplace_back(texturesIDs[Ice], x, y, true, slippery);
						break;
					}
					x++;
				}
			}
			y--;
		}
	}
}
