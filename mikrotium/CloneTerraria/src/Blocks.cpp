#include"Blocks.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"

Block::Block(unsigned int tex, float x, float y,unsigned int eob)
{
	m_te = tex;
	m_transform[0] = x;
	m_transform[1] = y;
	m_vertices[0] = x - 0.5f; m_vertices[1] = y + 0.5f; m_vertices[2] = 0.1f; m_vertices[3] = 0.0f; m_vertices[4] = 1.0f;
	m_vertices[5] = x + 0.5f; m_vertices[6] = y + 0.5f; m_vertices[7] = 0.1f; m_vertices[8] = 1.0f; m_vertices[9] = 1.0f;
	m_vertices[10] = x + 0.5f; m_vertices[11] = y - 0.5f; m_vertices[12] = 0.1f; m_vertices[13] = 1.0f; m_vertices[14] = 0.0f;
	m_vertices[15] = x - 0.5f; m_vertices[16] = y - 0.5f; m_vertices[17] = 0.1f; m_vertices[18] = 0.0f; m_vertices[19] = 0.0f;
	glGenVertexArrays(1, &m_vao);
	ErrorGL(glBindVertexArray(m_vao));
	ErrorGL(glGenBuffers(1, &m_VB));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VB));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), m_vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

	ErrorGL(glBindVertexArray(0));
}

void Block::DrawBlock()
{
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_te));
	ErrorGL(glBindVertexArray(m_vao));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	
}

void Block::DeleteBlock()
{
	ErrorGL(glDeleteBuffers(1, &m_VB));
	ErrorGL(glDeleteVertexArrays(1, &m_vao));

}

void LoadMap(const char* Path, std::vector<StaticSquereHitbox>& hitbox, std::vector<Block>& blocks, unsigned int EOB, unsigned int grass)
{
	std::string line;
	std::ifstream map(Path);
	if (!map)
	{
		std::cout << "can not open map file" << Path << std::endl;
	}
	float y = 0.0f;
	while (std::getline(map, line))
	{
		float x = -56;
		for (int j = 0; j < line.length(); j++)
		{
			if (line[j] != ' ')
			{
				
				blocks.push_back(Block(grass, x, y, EOB));
				hitbox.push_back(StaticSquereHitbox(x, y));

			}
			x++;
		}
		y--;
	}

}
