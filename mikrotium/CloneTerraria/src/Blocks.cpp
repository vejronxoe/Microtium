#include"Blocks.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
#include"math/matrix.h"

enum BlockBehavior
{
	basicSolid, air, slippery, asphalt
};
Block::Block(unsigned int tex, int x, int y, bool hitboxActive)
	: m_te(tex), m_Transform{x,y}, m_CollisionActive(hitboxActive)
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

void Block::DrawBlock(unsigned int blocksDrawData, Shader& basicShader, unsigned int location, float* transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(location, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_te));
	ErrorGL(glBindVertexArray(blocksDrawData));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	
}


enum TexturesOfBlocks 
{
	Grass = 0, 
	Dirt,

};
void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[Grass] = CreateTexture("res/textures/GrassBlock.png", true);
	IDs[Dirt] = CreateTexture("res/textures/DirtBlock.png", true);
}

void CreateBlock(int x, int y, unsigned int texture,  std::vector<Block>& blocks)
{
	blocks.emplace_back(texture, x, y, true);
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
		int y = 0.0f;
		{
			lines.emplace_back(" ");
			int i = 0;
			while (std::getline(map, lines[i]))
			{
				lines.emplace_back(" ");
				i++;
			}
		}
		lines.pop_back();
		map.close();
		for (int i = 0; i < lines.size(); i++)
		{


			int x = -56;
			{
				for (int j = 0; j < lines.at(i).length(); j++)
				{
					switch (lines.at(i).at(j))
					{
					case'd':
							if (lines.at(i-1).at(j) == ' ')
							{
								CreateBlock(x, y, texturesIDs[Grass], blocks);
							}
							else
							{
								CreateBlock(x, y, texturesIDs[Dirt], blocks);
							}
						break;
					}

					x++;
				}
			}
			y--;
		}
	}
}
