#include"cursor.h"

#include"../Opengl/drawData.h"
#include"../Opengl/Texture.h"
#include"../glfw/input.h"
#include"../glfw/Window.h"
#include"../math/matrix.h"
#include"../NumberRender.h"
unsigned int CreateCursorDrawData
(unsigned int* CursorTextures
, unsigned int eob)
{
	unsigned int drawData = CreateDrawData(eob, 0, -28, 28, 0);
	CursorTextures[canNotDoIt] = CreateTextureRGBA("res/textures/canNotDo.png");
	CursorTextures[canClickOnIt] = CreateTextureRGBA("res/textures/canClickon.png");
	CursorTextures[canPickaxeIt] = CreateTextureRGBA("res/textures/canPickAxeIt.png");
	CursorTextures[canAxeIt] = CreateTextureRGBA("res/textures/canAxeIt.png");
	CursorTextures[canHammerIt] = CreateTextureRGBA("res/textures/canHammerIt.png");
	CursorTextures[canPutBin] = CreateTextureRGBA("res/textures/canPutItInBin.png");
	return drawData;
}
void DrawCursor(unsigned int* CursorTextures
	, unsigned int* structurteTextures
	, unsigned int* structurteDD
	, unsigned int cursorDrawData
	, unsigned int blockDrawData
	, Shader& basicSh
	, Shader& fontSh
	, float* transform
	, float* camera
	, float* scale
	, unsigned int fontDrawData
	, unsigned int numberTexture
	, Player& player
	, float* cameraCoordinates)
{
	basicSh.Bind();
	int x = std::roundf(cameraCoordinates[0] + Input::XMousePos);
	int y = std::roundf(cameraCoordinates[1] + Input::YMousePos);
	if (player.m_CursorOnPlaceableSpot)
	{
		ChangeTransform(x, y, transform);
		basicSh.SetUniformMat4(basicTransform, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_UseSlotTexture));
		ErrorGL(glBindVertexArray(blockDrawData));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	else if (player.m_CursorOnPlaceableForStructure)
	{
		ChangeTransform(x, y, transform);
		basicSh.SetUniformMat4( basicTransform, transform);
		basicSh.SetUniform1i(basicSize + ShadowLocation, -1);

		switch (player.m_PlayerSlots[0])
		{
		case i_Sapling:
			ErrorGL(glBindTexture(GL_TEXTURE_2D, structurteTextures[s_Sapling]));
			ErrorGL(glBindVertexArray(structurteDD[s_Sapling]));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

			break;
		}
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		basicSh.SetUniform1i(basicSize + ShadowLocation, 0);

	}
	ChangeTransform(Input::XRawMousePos, Window::height - Input::YRawMousePos, transform);
	basicSh.SetUniformMat4(basicTransform, transform);
	ChangeCamera(0, Window::width, 0, Window::height, camera);
	basicSh.SetUniformMat4(basicCamera, camera);

	bool canClickOn = player.m_AimingAtSlot;

	if (player.m_UseSlot != 0 && player.m_PlayerSlots[0] != i_Nothing)
	{
		if (player.m_PlayerSlots[0] >= i_WallDirt && player.m_PlayerSlots[0] <= i_WallIce)
		{
			basicSh.SetUniform1i( basicSize + ShadowLocation, 1);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_AllItemTextures[player.m_PlayerSlots[0]]));
			ErrorGL(glBindVertexArray(cursorDrawData));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			fontSh.Bind();
			ErrorGL(glBindVertexArray(fontDrawData));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
			float right = player.m_SlotVertices[2] + Input::XRawMousePos;
			float left = player.m_SlotVertices[0] + Input::XRawMousePos;
			drawNumber(Window::height - player.m_SlotVertices[3] - Input::YRawMousePos, left, right, player.m_AmountInSlots[0], fontDrawData, scale, transform, fontSh);
		}
		else
		{
			basicSh.SetUniform1i(basicSize + ShadowLocation, 0);
			ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_AllItemTextures[player.m_PlayerSlots[0]]));
			ErrorGL(glBindVertexArray(cursorDrawData));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
			fontSh.Bind();
			ErrorGL(glBindVertexArray(fontDrawData));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
			float right = player.m_SlotVertices[2] + Input::XRawMousePos;
			float left = player.m_SlotVertices[0] + Input::XRawMousePos;
			drawNumber(Window::height - player.m_SlotVertices[3] - Input::YRawMousePos, left, right, player.m_AmountInSlots[0], fontDrawData, scale, transform, fontSh);

		}
		return;
	}
	else if (canClickOn)
	{
		if (Input::CtrlHold)
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canPutBin]));
		}
		else
		{
			ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canClickOnIt]));
		}
	}
	else if (player.m_CursorOnMinableBlock)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canPickaxeIt]));

	}
	else if (player.m_CursorOnMinableWall)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canHammerIt]));
	}
	else if (player.m_CursorOnMinableWood)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canAxeIt]));
	}
	else
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canNotDoIt]));
	}
	ErrorGL(glBindVertexArray(cursorDrawData));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));


}	