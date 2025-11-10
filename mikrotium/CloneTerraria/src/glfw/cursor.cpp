#include"cursor.h"

#include"../Opengl/drawData.h"
#include"../Opengl/Texture.h"
#include"../glfw/input.h"
#include"../glfw/Window.h"
#include"../math/matrix.h"
unsigned int CreateCursorDrawData(unsigned int* CursorTextures, unsigned int eob)
{
	unsigned int drawData = CreateDrawData(eob, 0, -28, 28, 0);
	CursorTextures[canNotDoIt] = CreateTextureRGBA("res/textures/canNotDo.png");
	CursorTextures[canClickOnIt] = CreateTextureRGBA("res/textures/canClickon.png");
	CursorTextures[canPickaxeIt] = CreateTextureRGBA("res/textures/canPickAxeIt.png");
	CursorTextures[canAxeIt] = CreateTextureRGBA("res/textures/canAxeIt.png");
	CursorTextures[canHammerIt] = CreateTextureRGBA("res/textures/canHammerIt.png");
	return drawData;
}
void DrawCursor(unsigned int* CursorTextures, unsigned int cursorDrawData, unsigned int blockDrawData, Shader& basicSh, unsigned int transformLocation, float* transform, unsigned int cameraLocation, Player& player)
{
	basicSh.Bind();
	int x = std::roundf(player.m_Transform[0] + Input::XMousePos);
	int y = std::roundf(player.m_Transform[1] + Input::YMousePos);
	if (player.m_CursorOnPlaceableSpot)
	{
		ChangeTransform(x, y, transform);
		basicSh.SetUniformMat4(transformLocation, transform);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_UseSlotTexture));
		ErrorGL(glBindVertexArray(blockDrawData));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	ChangeTransform(Input::XRawMousePos, Window::height - Input::YRawMousePos, transform);
	basicSh.SetUniformMat4(transformLocation, transform);
	ChangeCamera(0, Window::width, 0, Window::height,player.m_Camera);
	basicSh.SetUniformMat4(cameraLocation, player.m_Camera);
	
	bool canClickOn = player.m_AimingAtSlot;
	if (player.m_UseSlot != 0 && player.m_PlayerSlots[0] != i_Nothing)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_AllItemTextures[player.m_PlayerSlots[0]]));
	}
	else if(canClickOn)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canClickOnIt]));
	}
	else if(player.m_CursorOnMinableBlock)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canPickaxeIt]));

	}
	else 
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, CursorTextures[canNotDoIt]));
	}
	
	ErrorGL(glBindVertexArray(cursorDrawData));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
