#include"flora.h"

#include "Opengl/ErrorSystem.h"
#include "math/matrix.h"
#include "glfw/Window.h"
#define TIMETOGROW 10

void tree::drawTree(Shader& sh
	, unsigned int transformLocation
	, unsigned int rotateLocation
	, float* cameraCoordinate
	, float* transform
	, float* rotation)
{
	if (ceilf(cameraCoordinate[0] + Window::halfWidthOfGameTransform) >= m_Transform[0] && floorf(cameraCoordinate[0] - Window::halfWidthOfGameTransform) <= m_Transform[0])
	{
		if (floorf(cameraCoordinate[1] - Window::halfHeightOfGameTransform) <= m_Transform[1] && ceilf(cameraCoordinate[1] + Window::halfHeightOfGameTransform) >= m_Transform[1])
		{
			if (m_Rotation != 0)
			{
				ChangeRotation(m_Rotation, rotation);
				sh.SetUniformMat4(rotateLocation, rotation);
			}
			ErrorGL(glBindVertexArray(m_DrawData));
			ChangeTransform(m_Transform[0], m_Transform[1], transform);
			sh.SetUniformMat4(transformLocation, transform);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, m_texture));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			if (m_Rotation != 0)
			{
				ChangeRotation(0, rotation);
				sh.SetUniformMat4(rotateLocation, rotation);
			}
		}
	}
}
tree::tree(unsigned int texture
	, unsigned int drawData
	, unsigned short int itemDrop
	, unsigned char hardness
	, char partOfTree
	, int x
	, int y
	, float rotation)
	: m_Transform{ x, y }
	, m_PartOfTree(partOfTree)
	, m_ItemDrop(itemDrop)
	, m_Hardness(hardness)
	, m_texture(texture)
	, m_DrawData(drawData)
	, m_Rotation(rotation)
{}
damagedWood::damagedWood(int x
	, int y
	, int rotation
	, char HP)
:m_Transform{x,y}, m_Rotation(rotation), m_HP(HP)
{}

void damagedWood::DrawCut(Shader& sh
	, unsigned int transformLocation
	, unsigned int rotationLocation
	, float* rotation
	, float* transform
	, unsigned int* texture)
{

	if (m_Rotation != 0)
	{
		ChangeRotation(m_Rotation, rotation);
		sh.SetUniformMat4(rotationLocation, rotation);
	}
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(transformLocation, transform);
	int index = m_HP / 3;
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[index]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	if (m_Rotation != 0)
	{
		ChangeRotation(0, rotation);
		sh.SetUniformMat4(rotationLocation, rotation);
	}
}


seedling::seedling(char type
	, int x
	, int y
	, unsigned int* structuresTextures)
	:m_Transform{x, y}
	,m_Timer(0)
	,m_Type(type)
	,m_Texture(structuresTextures[type])
{}
void seedling::everyFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, std::vector<seedling>& seedling
	, std::vector<tree>& trees)
{
	m_Timer += deltaTime;
	if (m_Timer > TIMETOGROW)
	{
		int leangth = (rand() % 16) + 6;
		bool inBlock = false;
		for (int j = m_Transform[0] - 4; j < m_Transform[0] + 5; j++)
		{
			for (int i = 0; i < blocks.at(j).size(); i++)
			{
				if (blocks.at(j).at(i).m_Transform[1] >= m_Transform[1] && blocks.at(j).at(i).m_Transform[1] <= m_Transform[1] + leangth)
				{
					inBlock = true;
					break;
				}
			}
		}
		for (int i = 0; i < trees.size(); i++)
		{
			if (trees.at(i).m_Transform[1] >= m_Transform[1] && trees.at(i).m_Transform[1] <= m_Transform[1] + leangth)
			{
				inBlock = true;
				break;
			}
		}
		if (inBlock)
		{
			m_Timer = 0;
		}
		else
		{
			int branchsMaxCount = floor(leangth / 3);
			unsigned int leftBranchs = rand() % branchsMaxCount;
			unsigned int rightBranchs = rand() % branchsMaxCount;
			std::vector<int> leftHeight;
			std::vector<int> rightHeight;
			for (int i = 0; i < leftBranchs; i++)
			{
			
			}
		}
	}
}
void seedling::drawSeedling(Shader sh
	, unsigned int transformLocation
	, float* Transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], Transform);
	sh.SetUniformMat4(transformLocation, Transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}