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
Editor::Editor(unsigned int eob)
{
	m_SelectBoxDD = CreateDrawData(eob,0.5f,-0.5f,0.5f,-0.5f,1,0,1.0f/4.0f,0);
	m_SelectBoxTex = CreateTextureRGBA("res/textures/SelectZone.png");
}
void DeleteBlocksInArea(std::vector<std::vector<Block>>& blocks
, int* vertices)
{
	for (int i = vertices[0]; i <= vertices[2]; i++)
	{
		for (int j = 0; j < blocks.at(i).size();j++)
		{
			if (blocks[i][j].m_Y < vertices[3])
			{
				break;
			}
			else if (blocks[i][j].m_Y <= vertices[1])
			{
				blocks[i].erase(blocks[i].begin() + j);
				j--;
			}

		}
	}
}
void Editor::Update(float deltaTime
	,char cursorState
	, unsigned int* BlocksTex
	, std::vector<std::vector<Block>>& blocks)
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
	
	m_Transform[0] = CameraHitboxX(m_Transform[0]);
	m_Transform[1] = CameraHitboxY(m_Transform[1]);
	
	
	
	
	if (cursorState == canNotDoIt)
	{
		int x = roundf(Input::XMousePos + m_Transform[0]);
		int y = roundf(Input::YMousePos + m_Transform[1]);


		switch (m_placingType)
		{
		case selectType:
			if (Input::LeftMousePress)
			{
				m_Selected = -1;
				m_Eraser = false;
				m_BoxSelected = false;
				m_FirstPointBox[0] = x;
				m_FirstPointBox[1] = y;
				m_SelectBoxSides[0] = x;
				m_SelectBoxSides[1] = y;
				m_SelectBoxSides[2] = x;
				m_SelectBoxSides[3] = y;
			}
			else if (Input::LeftMouseHold)
			{
				if (m_FirstPointBox[0] < x)
				{
					m_SelectBoxSides[0] = m_FirstPointBox[0];
					m_SelectBoxSides[2] = x;

				}
				else if (m_FirstPointBox[0] > x)
				{

					m_SelectBoxSides[2] = m_FirstPointBox[0];
					m_SelectBoxSides[0] = x;
				}
				else
				{
					m_SelectBoxSides[0] = x;
					m_SelectBoxSides[2] = x;
				}
				if (m_FirstPointBox[1] < y)
				{
					m_SelectBoxSides[1] = y;
					m_SelectBoxSides[3] = m_FirstPointBox[1];
				}
				else if (m_FirstPointBox[1] > y)
				{
					m_SelectBoxSides[1] = m_FirstPointBox[1];
					m_SelectBoxSides[3] = y;
				}
				else
				{
					m_SelectBoxSides[1] = y;
					m_SelectBoxSides[3] = y;
				}
			}
			else if (Input::LeftMouseRelease)
			{
				m_BoxSelected = true;
			}
			if (m_BoxSelected)
			{
				if (m_Eraser)
				{
					DeleteBlocksInArea(blocks, m_SelectBoxSides);
					m_Eraser = false;
				}
				else if(m_Selected < t_BlocksSize && m_Selected > -1)
				{
					DeleteBlocksInArea(blocks, m_SelectBoxSides);

					for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
					{
						for (int j = m_SelectBoxSides[3]; j <= m_SelectBoxSides[1];j++)
						{
							PushBlockInVec(blocks, GetItemIDByTexture(BlocksTex[m_Selected],BlocksTex), i, j, BlocksTex);

						}
					}
					m_Selected = -1;
				}
				else if (Input::CtrlHold && Input::XPress)
				{
					m_CopiedBlocks.assign(m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1, std::vector<int> {});
					for (int i = 0; i < m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1; i++)
					{
						m_CopiedBlocks[i].assign(m_SelectBoxSides[1] - m_SelectBoxSides[3] + 1,i_Nothing);
					}
					for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
					{
						for (int j = 0; j < blocks.at(i).size();j++)
						{
							if (blocks[i][j].m_Y < m_SelectBoxSides[3])
							{
								break;
							}
							else if (blocks[i][j].m_Y <= m_SelectBoxSides[1])
							{
								m_CopiedBlocks[i - m_SelectBoxSides[0]].at(abs(blocks[i][j].m_Y - m_SelectBoxSides[1])) = GetItemIDByTexture(blocks[i][j].m_te, BlocksTex);
							}

						}
					}
					DeleteBlocksInArea(blocks, m_SelectBoxSides);

				}
				else if (Input::CtrlHold && Input::CPress)
				{
					m_CopiedBlocks.assign(m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1, std::vector<int> {});
					for (int i = 0; i < m_SelectBoxSides[2] - m_SelectBoxSides[0] + 1; i++)
					{
						m_CopiedBlocks[i].assign(m_SelectBoxSides[1] - m_SelectBoxSides[3] + 1, i_Nothing);
					}
					for (int i = m_SelectBoxSides[0]; i <= m_SelectBoxSides[2]; i++)
					{
						for (int j = 0; j < blocks.at(i).size();j++)
						{
							if (blocks[i][j].m_Y < m_SelectBoxSides[3])
							{
								break;
							}
							else if (blocks[i][j].m_Y <= m_SelectBoxSides[1])
							{
								m_CopiedBlocks[i - m_SelectBoxSides[0]].at(abs(blocks[i][j].m_Y - m_SelectBoxSides[1])) = GetItemIDByTexture(blocks[i][j].m_te,BlocksTex);
							}

						}
					}
				}
				else if (Input::CtrlHold && Input::VPress)
				{
					int Vertices[4] = { m_FirstPointBox[0], m_FirstPointBox[1], m_FirstPointBox[0] + m_CopiedBlocks.size(), m_FirstPointBox[1] - m_CopiedBlocks[0].size()};
					DeleteBlocksInArea(blocks, Vertices);
					for (int i = 0 ; i < m_CopiedBlocks.size();i++)
					{
						for (int j = 0; j < m_CopiedBlocks[i].size();j++)
						{
							if (m_CopiedBlocks[i][j] != i_Nothing)
							{
								PushBlockInVec(blocks, m_CopiedBlocks[i][j], m_FirstPointBox[0] + i, m_FirstPointBox[1] - j, BlocksTex);

							}
						}
					}
				}
			}
			break;
		case brushType:
			m_BoxSelected = false;
			break;
		}
	}

}
void Editor::Draw(Shader& animSh
	, float* transform
	, float* scale)
{
	if (m_placingType == selectType)
	{
		animSh.Bind();
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SelectBoxTex));
		ErrorGL(glBindVertexArray(m_SelectBoxDD));
		ChangeScale(1, 1, scale);

		animSh.SetUniformMat4(animScale, scale);
		animSh.SetUniform1i(animLeangth, 4);
		int drawTransform[2] = { m_SelectBoxSides[0],m_SelectBoxSides[3] };
		int addx[4] = { 0,1,0,-1 };
		int addy[4] = { 1,0,-1, 0 };
		int sidelength[2] = { m_SelectBoxSides[2] - m_SelectBoxSides[0],m_SelectBoxSides[1] - m_SelectBoxSides[3] };
		for (int i = 0; i < 4; i++)
		{

			drawTransform[0] -= addx[i];
			drawTransform[1] -= addy[i];
			animSh.SetUniform1i(animNumber, i);
			for (int j = 0; j <= sidelength[abs(i % 2-1)]; j++)
			{
								
			
				drawTransform[0] += addx[i];
				drawTransform[1] += addy[i];
			
				ChangeTransform(drawTransform[0], drawTransform[1], transform);
				animSh.SetUniformMat4(animTransform, transform);
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			}
		}
	}
}

EditorHUD::EditorHUD(unsigned int eob
	, unsigned int backGroundTex)
{
	m_Icons[0] = CreateTextureRGBA("res/textures/PaintBrush.png");
	m_Icons[1] = CreateTextureRGBA("res/textures/SelectIcon.png");
	m_Icons[2] = CreateTextureRGBA("res/textures/RubberIcon.png");
	m_SlotTexs = CreateTextureRGBA("res/textures/inventorySlot.png");
	m_SelectZoneTex = CreateTextureRGBA("res/textures/SelectZone.png");
	Create(true,eob);
}
void EditorHUD::Create(unsigned int eob
	, bool first)
{
	if (!first)
	{
		ErrorGL(glDeleteBuffers(4, m_VBOs));
		ErrorGL(glDeleteBuffers(2 , m_EOBs));
		ErrorGL(glDeleteVertexArrays(4, m_DDs));
	}
	ErrorGL(glGenBuffers(2, m_EOBs));

	m_Scroll = 0;
	m_GapLength = DistanceOnUI(0.016f);
	m_SideLength = DistanceOnUI(0.11f);
	m_DDs[useSlotDD] = CreateDrawData(eob, 0.6f * m_SideLength, -0.6f * m_SideLength, 0.6f * m_SideLength, -0.6f * m_SideLength, m_VBOs[useSlotDD], 1, 0, 2*TEXSLOTDISTANCE, TEXSLOTDISTANCE);
	m_DDs[defaultSlotUV] = CreateDrawData(eob, 0.4f * m_SideLength, -0.4f * m_SideLength, 0.4f * m_SideLength, -0.4f * m_SideLength, m_VBOs[defaultSlotUV]);
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
	int cursorBehavior = canNotDoIt;
	
		int row = -1;
		if (Window::width - m_GapLength - m_SideLength <= Input::XRawMousePos && Window::width - m_GapLength >= Input::XRawMousePos)
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
			for (int i = 0; i < (t_BlocksSize + s_StructureSize) / 2; i++)
			{
				if (-i * (m_GapLength + m_SideLength) + m_GapLength + m_Scroll * (m_GapLength + m_SideLength) < Window::height - Input::YRawMousePos && m_GapLength + m_SideLength - i * (m_GapLength + m_SideLength) + m_Scroll * (m_GapLength + m_SideLength) > Window::height - Input::YRawMousePos)
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

			for (int i = 0; i < 3; i++)
			{
				if (m_GapLength + i * (m_GapLength + m_SideLength) < Input::YRawMousePos && (i + 1) * (m_GapLength + m_SideLength) > Input::YRawMousePos)
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
		if (!Input::LeftMouseHold)
		{
			if (Input::EPress)
			{
				editor.m_Eraser = !editor.m_Eraser;

			}
			if (Input::NumberPress[0])
			{
				editor.m_placingType = brushType;
			}
			else if (Input::NumberPress[1])
			{
				editor.m_placingType = selectType;
			}
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
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SlotTexs));
	ErrorGL(glBindVertexArray(m_DDs[leftHUDSlots]));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[1], GL_UNSIGNED_BYTE, 0));
	ChangeTransform(0,m_Scroll*(m_GapLength+ m_SideLength), transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_DDs[rightHUDSlots]));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_EOBSizes[0], GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindVertexArray(m_DDs[useSlotDD]));
	if (editor.m_Selected != -1)
	{
		ChangeTransform(Window::width - (m_GapLength + m_SideLength / 2.0f) + (editor.m_Selected / 2 - roundf(editor.m_Selected / 2.0f)) * (m_GapLength + m_SideLength), m_Scroll * (m_GapLength + m_SideLength) + (m_GapLength + m_SideLength / 2.0f) + -(editor.m_Selected / 2) * (m_GapLength + m_SideLength), transform);
		sh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
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
