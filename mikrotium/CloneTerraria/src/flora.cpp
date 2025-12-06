#include"flora.h"

#include"Opengl/ErrorSystem.h"
#include"math/matrix.h"
#include"glfw/Window.h"


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