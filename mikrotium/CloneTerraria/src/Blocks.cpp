#include"Blocks.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
#include"math/matrix.h"


Block::Block(unsigned int tex, int x, int y, bool hitboxActive, unsigned int behavior)
	: m_te(tex), m_Transform{x,y}, m_CollisionActive(hitboxActive), m_BlockBehavior(behavior)
{}
void SetupBlockDrawData(unsigned int& blocksDrawData, unsigned int eob)
{
	unsigned int blocksvertexBuffer;
	float blocksVertices[20];
	blocksVertices[0] = -0.5f; blocksVertices[1] = 0.5f; blocksVertices[2] = 0.1f; blocksVertices[3] = 0.0f; blocksVertices[4] = 1.0f;
	blocksVertices[5] = 0.5f; blocksVertices[6] = 0.5f; blocksVertices[7] = 0.1f; blocksVertices[8] = 1.0f; blocksVertices[9] = 1.0f;
	blocksVertices[10] = 0.5f; blocksVertices[11] = -0.5f; blocksVertices[12] = 0.1f; blocksVertices[13] = 1.0f; blocksVertices[14] = 0.0f;
	blocksVertices[15] = -0.5f; blocksVertices[16] = -0.5f; blocksVertices[17] = 0.1f; blocksVertices[18] = 0.0f; blocksVertices[19] = 0.0f;
	ErrorGL(glGenVertexArrays(1, &blocksDrawData));
	ErrorGL(glBindVertexArray(blocksDrawData));
	ErrorGL(glGenBuffers(1, &blocksvertexBuffer));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, blocksvertexBuffer));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), blocksVertices , GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

	ErrorGL(glBindVertexArray(0));
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
	IDs[TopGrass] = CreateTexture("res/textures/topGrassBlock.png", true);
	IDs[LeftGrass] = CreateTexture("res/textures/leftGrassBlock.png", true);
	IDs[DownGrass] = CreateTexture("res/textures/downGrassBlock.png", true);
	IDs[RightGrass] = CreateTexture("res/textures/rightGrassBlock.png", true);
	IDs[TopDownGrass] = CreateTexture("res/textures/topDownGrassBlock.png", true);
	IDs[LeftRightGrass] = CreateTexture("res/textures/leftRightGrassBlock.png", true);
	IDs[TopLeftGrass] = CreateTexture("res/textures/topLeftGrassBlock.png", true);
	IDs[DownLeftGrass] = CreateTexture("res/textures/downLeftGrassBlock.png", true);
	IDs[DownRightGrass] = CreateTexture("res/textures/downRightGrassBlock.png", true);
	IDs[TopRightGrass] = CreateTexture("res/textures/topRightGrassBlock.png", true);
	IDs[MissingTopGrass] = CreateTexture("res/textures/missingTopGrassBlock.png", true);
	IDs[MissingLeftGrass] = CreateTexture("res/textures/missingLeftGrassBlock.png", true);
	IDs[MissingDownGrass] = CreateTexture("res/textures/missingDownGrassBlock.png", true);
	IDs[MissingRightGrass] = CreateTexture("res/textures/missingRightGrassBlock.png", true);
	IDs[FullGrass] = CreateTexture("res/textures/fullGrassBlock.png", true);
	IDs[Dirt] = CreateTexture("res/textures/dirtBlock.png", true);
	IDs[Ice] = CreateTexture("res/textures/ice.png", true);
	IDs[Asphalt] = CreateTexture("res/textures/Asphalt.png", true);
	IDs[Platform] = CreateTexture("res/textures/platform.png", true);
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
						blocks.emplace_back(GrassBlockTextureSelector(texturesIDs, lines, i, j), x, y, true, basicSolid);
						break;
					case'p':
						blocks.emplace_back(texturesIDs[Platform], x, y, true, platform);
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
