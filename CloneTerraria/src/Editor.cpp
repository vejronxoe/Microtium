#include"Editor.h"
#include "glfw/input.h"
#include "Opengl/Texture.h"
#include "Opengl/drawData.h"
#include "glfw/UI.h"
#include "glfw/Window.h"
#include "math/VectorOperation.h"
#include "math/matrix.h"
#include "ItemList.h"


void Editor::Update(float deltaTime)
{ 
	float oldVelocity[2] = { m_Velocity[0], m_Velocity[1]};
	if (Input::AHold)
	{
		m_Velocity[0] -= deltaTime * 100;
	}

	if (Input::SHold)
	{
		m_Velocity[1] -= deltaTime * 100;

	}

	if (Input::DHold)
	{
		m_Velocity[0] += deltaTime * 100;

	}

	if (Input::WHold)
	{
		m_Velocity[1] += deltaTime * 100;

	}

	if (!(Input::WHold || Input::SHold))
	{
		m_Velocity[1] = 0;
		oldVelocity[1] = 0;
	}
	if (!(Input::AHold || Input::DHold))
	{
		m_Velocity[0] = 0;
		oldVelocity[0] = 0;
	}
	m_Transform[1] += oldVelocity[1] * deltaTime + 0.5f * (m_Velocity[1] - oldVelocity[1]) * deltaTime;
	m_Transform[0] += oldVelocity[0] * deltaTime + 0.5f * (m_Velocity[0] - oldVelocity[0]) * deltaTime;


}
EditorHUD::EditorHUD(unsigned int eob
	, unsigned int backGroundTex)
{
	m_Textures[0] = backGroundTex;
	m_Textures[1] = CreateTextureRGBA("res/textures/inventorySlot.png");
	Create(true,eob);
}
void EditorHUD::Create(unsigned int eob
	, bool first)
{
	if (!first)
	{
		ErrorGL(glDeleteBuffers(3, m_VBOs));
		ErrorGL(glDeleteVertexArrays(3, m_DDs));
	}
	m_Scroll = 0;
	m_GapLength = DistanceOnUI(0.02f);
	m_SideLength = DistanceOnUI(0.15f);
	m_DDs[0] = CreateDrawData(eob, 0.5f * m_SideLength, -0.5f * m_SideLength, 0.5f * m_SideLength, -0.5f * m_SideLength, m_VBOs[0],1,0, TEXSLOTDISTANCE,0);
	m_DDs[1] = CreateDrawData(eob, Window::height, 0, Window::width, Window::width - (2 * m_SideLength + 3 * m_GapLength), m_VBOs[1]);
	m_DDs[2] = CreateDrawData(eob, Window::height, 0, (m_SideLength + 2 * m_GapLength), 0, m_VBOs[2]);
}
void EditorHUD::Update(float deltaTime)
{
	if (Input::MouseWheel)
	{
		m_WantedScroll -= Input::MouseWheel;
		m_WantedScroll = Clamp(m_WantedScroll, 0, +  s_StructureSize);
	}

}
void EditorHUD::Draw(Shader& sh
,float* transform)
{
	sh.Bind();
	ChangeTransform(0,0,transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Textures[0]));
	ErrorGL(glBindVertexArray(m_DDs[1]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindVertexArray(m_DDs[2]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Textures[1]));
	ErrorGL(glBindVertexArray(m_DDs[0]));
	ChangeTransform(0.5f * m_SideLength + m_GapLength, Window::height - (0.5f * m_SideLength + m_GapLength), transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ChangeTransform(Window::width - (0.5f * m_SideLength + m_GapLength), 0.5f * m_SideLength + m_GapLength, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}
