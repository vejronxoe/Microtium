#include"Editor.h"
#include "glfw/input.h"
#include "Opengl/Texture.h"
#include "Opengl/drawData.h"
#include "glfw/UI.h"
#include "glfw/Window.h"
#include "math/VectorOperation.h"
#include "math/matrix.h"
#include "ItemList.h"
#include "glfw/cursor.h"

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
	m_Icons[0] = CreateTextureRGBA("res/textures/PaintBrush.png");
	m_Icons[1] = CreateTextureRGBA("res/textures/SelectIcon.png");
	m_Icons[2] = CreateTextureRGBA("res/textures/RubberIcon.png");
	m_Textures[0] = backGroundTex;
	m_Textures[1] = CreateTextureRGBA("res/textures/inventorySlot.png");
	Create(true,eob);
}
void EditorHUD::Create(unsigned int eob
	, bool first)
{
	if (!first)
	{
		ErrorGL(glDeleteBuffers(6, m_VBOs));
		ErrorGL(glDeleteBuffers(2 , m_EOBs));
		ErrorGL(glDeleteVertexArrays(6, m_DDs));
	}
	ErrorGL(glGenBuffers(2, m_EOBs));

	m_Scroll = 0;
	m_GapLength = DistanceOnUI(0.016f);
	m_SideLength = DistanceOnUI(0.11f);
	m_DDs[useSlotDD] = CreateDrawData(eob, 0.6f * m_SideLength, -0.6f * m_SideLength, 0.6f * m_SideLength, -0.6f * m_SideLength, m_VBOs[useSlotDD], 1, 0, 2*TEXSLOTDISTANCE, TEXSLOTDISTANCE);
	m_DDs[defaultSlotUV] = CreateDrawData(eob, 0.4f * m_SideLength, -0.4f * m_SideLength, 0.4f * m_SideLength, -0.4f * m_SideLength, m_VBOs[defaultSlotUV]);
	m_DDs[rightBackground] = CreateDrawData(eob, Window::height, 0, Window::width, Window::width - (2 * m_SideLength + 3 * m_GapLength), m_VBOs[rightBackground]);
	m_DDs[leftBackground] = CreateDrawData(eob, Window::height, 0, (m_SideLength + 2 * m_GapLength), 0, m_VBOs[leftBackground]);
	std::vector<float> Vertices;
	std::vector<unsigned char> order;
	Vertices.reserve((t_BlocksSize + s_StructureSize) * 16);
	int placebleObjectsNumber = floorf((t_BlocksSize + s_StructureSize) / 2.0f);
	int j;
	for (j = 0; j <placebleObjectsNumber; j++)
	{
		for (int i = 0;i < 2;i++)
		{
			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(0);

			Vertices.emplace_back(Window::width - (m_GapLength + i * (m_GapLength + m_SideLength)));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(TEXSLOTDISTANCE);
			Vertices.emplace_back(1);

			Vertices.emplace_back(Window::width - (i + 1) * (m_GapLength + m_SideLength));
			Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
			Vertices.emplace_back(0);
			Vertices.emplace_back(1);

		}
	}
	if (j*2 != (t_BlocksSize + s_StructureSize))
	{
		Vertices.emplace_back(Window::width - 2 * (m_GapLength + m_SideLength));
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - (m_GapLength + (m_GapLength + m_SideLength)));
		Vertices.emplace_back(m_GapLength - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(0);

		Vertices.emplace_back(Window::width - (m_GapLength + (m_GapLength + m_SideLength)));
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(1);

		Vertices.emplace_back(Window::width - 2 * (m_GapLength + m_SideLength));
		Vertices.emplace_back((m_GapLength + m_SideLength) - j * (m_GapLength + m_SideLength));
		Vertices.emplace_back(0);
		Vertices.emplace_back(1);
	}
	order.resize((Vertices.size() / 16)*6);

	for (int i = 0; i < Vertices.size()/16; i++)
	{
		order.emplace_back(0 + i * 4);
		order.emplace_back(1 + i * 4);
		order.emplace_back(2 + i * 4);
		order.emplace_back(0 + i * 4);
		order.emplace_back(3 + i * 4);
		order.emplace_back(2 + i * 4);
	}
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[0]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[rightHUDSlots]));
	ErrorGL(glBindVertexArray(m_DDs[rightHUDSlots]));
	ErrorGL(glGenBuffers(1, &m_VBOs[rightHUDSlots]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[rightHUDSlots]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[0]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[0] = order.size();
	order.clear();
	Vertices.clear();
	
	for (int i = 0; i < 3;i++)
	{

		Vertices.emplace_back(m_GapLength);
		Vertices.emplace_back(Window::height - (m_GapLength + i * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(0);
		Vertices.emplace_back(0);

		Vertices.emplace_back(m_GapLength + m_SideLength);
		Vertices.emplace_back(Window::height - (m_GapLength + i * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(0);

		Vertices.emplace_back(m_GapLength + m_SideLength);
		Vertices.emplace_back(Window::height - ((i+1) * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(TEXSLOTDISTANCE);
		Vertices.emplace_back(1);

		Vertices.emplace_back(m_GapLength);
		Vertices.emplace_back(Window::height - ((i + 1) * (m_GapLength + m_SideLength)));
		Vertices.emplace_back(0);
		Vertices.emplace_back(1);
	}




	order.resize((Vertices.size()/16)*6);
	for (int i = 0; i<Vertices.size() / 16; i++)
	{
		order.emplace_back(0 + i*4);
		order.emplace_back(1 + i*4);
		order.emplace_back(2 + i*4);
		order.emplace_back(0 + i*4);
		order.emplace_back(3 + i*4);
		order.emplace_back(2 + i*4);
	}

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[1]));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));
	ErrorGL(glGenVertexArrays(1, &m_DDs[leftHUDSlots]));
	ErrorGL(glBindVertexArray(m_DDs[leftHUDSlots]));
	ErrorGL(glGenBuffers(1, &m_VBOs[leftHUDSlots]));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[leftHUDSlots]));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOBs[1]));

	ErrorGL(glBindVertexArray(0));
	m_EOBSizes[1] = order.size();


}
int EditorHUD::Update(float deltaTime
	,Editor& editor)
{
	int row = -1;
	int cursorBehavior = canNotDoIt;
	if (Window::width - m_GapLength - m_SideLength<= Input::XRawMousePos && Window::width - m_GapLength >= Input::XRawMousePos)
	{
		row = 0;
	}
	
	if (Window::width - 2 * (m_GapLength + m_SideLength) <= Input::XRawMousePos && Window::width - 2 * m_GapLength - m_SideLength >= Input::XRawMousePos)
	{
		row = 1;
	}
	
	if (m_GapLength <= Input::XRawMousePos && m_GapLength + m_SideLength >= Input::XRawMousePos)
	{
		row = 2;
	}


	switch (row)
	{
	case 0:
	case 1:
	{
		for (int i = 0; i < (t_BlocksSize + s_StructureSize)/2; i++)
		{
			if (-i*(m_GapLength+m_SideLength) + m_GapLength + m_Scroll* (m_GapLength + m_SideLength) < Window::height - Input::YRawMousePos && m_GapLength + m_SideLength -i * (m_GapLength + m_SideLength) + m_Scroll * (m_GapLength + m_SideLength) > Window::height - Input::YRawMousePos)
			{
				if (Input::LeftMousePress)
				{
					editor.m_Selected = i * 2 + row;
				}
				cursorBehavior = canClickOnIt;
				break;
			}
		}
		break;
	}
	case 2:
	{
	
		for (int i = 0; i < 3 ; i++)
		{
			if ( m_GapLength + i * (m_GapLength + m_SideLength) < Input::YRawMousePos && (i+1) * (m_GapLength + m_SideLength) > Input::YRawMousePos)
			{
				if (Input::LeftMousePress)
				{
					switch (i)
					{
					case 0:
						editor.m_placingType = brushType;
						break;
					case 1:
						editor.m_placingType = selectType;
						break;
					case 2:
						editor.m_Eraser = !editor.m_Eraser;
						break;
					}
				}
				cursorBehavior = canClickOnIt;
				break;
			}
		}
		
		break;
	}
	}
	if (Input::EPress)
	{
		editor.m_Eraser = !editor.m_Eraser;

	}
	if (Input::MouseWheel)
	{
		m_WantedScroll -= Input::MouseWheel;
		m_WantedScroll = Clamp(m_WantedScroll, 0, (t_BlocksSize +  s_StructureSize)/2.0f -1);
	}
	if (m_WantedScroll != m_Scroll)
	{
		float diff = (m_WantedScroll - m_Scroll);
		m_Scroll += (diff + 1 * diff / abs(diff)) * deltaTime;
		float newDiff = (m_WantedScroll - m_Scroll);
		if (newDiff != 0)
		{
			if (diff / abs(diff) != newDiff / abs(newDiff))
			{
				m_Scroll = m_WantedScroll;
			}
		}
	}
	return cursorBehavior;
}
void EditorHUD::Draw(Shader& sh
	, Editor editor
	, unsigned int* itemsTex
	, unsigned int * blockTex
	, float* transform)
{

	sh.Bind();
	ChangeTransform(0, 0, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Textures[0]));
	ErrorGL(glBindVertexArray(m_DDs[rightBackground]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindVertexArray(m_DDs[leftBackground]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Textures[1]));
	ErrorGL(glBindVertexArray(m_DDs[leftHUDSlots]));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[1], GL_UNSIGNED_BYTE, 0));
	ChangeTransform(0,m_Scroll*(m_GapLength+ m_SideLength), transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_DDs[rightHUDSlots]));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[0], GL_UNSIGNED_BYTE, 0));
	ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (editor.m_Selected/2 - roundf(editor.m_Selected/2.0f))*(m_GapLength+m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(editor.m_Selected / 2) * (m_GapLength + m_SideLength), transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_DDs[useSlotDD]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ChangeTransform(  m_SideLength / 2.0f + m_GapLength, Window::height - (m_SideLength/2.0f + m_GapLength + editor.m_placingType * (m_GapLength + m_SideLength)), transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	if (editor.m_Eraser)
	{
		ChangeTransform(m_SideLength / 2.0f + m_GapLength, Window::height - (m_SideLength / 2.0f + m_GapLength + 2 * (m_GapLength + m_SideLength)), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	ErrorGL(glBindVertexArray(m_DDs[defaultSlotUV]));

	for (int i = 0; i < 3; i++)
	{
		ChangeTransform(m_SideLength / 2.0f + m_GapLength, Window::height - (m_SideLength / 2.0f + m_GapLength + i * (m_GapLength + m_SideLength)), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Icons[i]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	for (int i = 0; i < t_BlocksSize; i++)
	{
		ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (i / 2 - roundf(i / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(i / 2) * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[i]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	for (int i = 0; i < s_StructureSize; i++)
	{
		int index = t_BlocksSize + i;
		ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (index / 2 - roundf(index / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(index / 2) * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, itemsTex[GetItemIDByStructure(i)]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
}