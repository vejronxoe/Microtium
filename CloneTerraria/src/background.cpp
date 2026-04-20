#include"background.h"
#include"Opengl/DrawData.h"
#include"Opengl/Texture.h"
#include"math/matrix.h"
#include"BlocksAndWalls.h"
#define SlowForBackground 2.0f
Background::Background(unsigned int eob , Shader backgroundSh)
{
	backgroundSh.Bind();
	ErrorGL(glUniform1i(glGetUniformLocation(backgroundSh.m_ID, "texture1"), 0));
	ErrorGL(glUniform1i(glGetUniformLocation(backgroundSh.m_ID, "texture2"), 1));
	m_Tex[0] = CreateTextureRepeatRGBA("res/textures/BackgroundForest.png");
	m_Tex[1] = CreateTextureRepeatRGBA("res/textures/BackgroundUnderground.png");
	m_Tex[2] = CreateTextureRepeatRGBA("res/textures/BackgroundSky.png");
	m_Tex[3] = CreateTextureRepeatRGBA("res/textures/BackgroundSand.png");
	m_Tex[4] = CreateTextureRepeatRGBA("res/textures/BackgroundSnow.png");
	m_DD[0] = CreateDrawData(eob, SlowForBackground * Blocks::yMax/12.0f, 0, SlowForBackground * Blocks::xMax, - SlowForBackground * Blocks::xMax / 24.0f, 0.999f, 0, 25, 0);
	m_DD[1] = CreateDrawData(eob, SlowForBackground * Blocks::yMax, SlowForBackground * Blocks::yMax / 12.0f - 1, SlowForBackground * Blocks::xMax, -SlowForBackground * Blocks::xMax / 24.0f , 16, 0, 25, 0);
	m_DD[2] = CreateDrawData(eob, 0, SlowForBackground * Blocks::yMin, SlowForBackground * Blocks::xMax, -SlowForBackground * Blocks::xMax / 24.0f, 24, 0, 25, 0);
}
void Background::DrawBackground(Shader backgroundSh, Shader basicSh, float* transform, float* cameraTransform)
{	
	basicSh.Bind();
	ChangeTransform(cameraTransform[0] / SlowForBackground, cameraTransform[1] / SlowForBackground, transform);
	basicSh.SetUniformMat4(basicTransform, transform);
	glBindTexture(GL_TEXTURE_2D, m_Tex[1]);
	ErrorGL(glBindVertexArray(m_DD[2]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ErrorGL(glBindVertexArray(m_DD[1]));
	glBindTexture(GL_TEXTURE_2D, m_Tex[2]);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	
	
	backgroundSh.Bind();
	ChangeTransform(cameraTransform[0] / SlowForBackground, cameraTransform[1] / SlowForBackground, transform);
	backgroundSh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_DD[0]));
	float blendFactor = (abs(cameraTransform[0] - Blocks::xMax / 2.0f) - 180) / 20.0f;
	if (blendFactor > 1)
	{
		blendFactor = 1;
	}
	else if (blendFactor < 0)
	{
		blendFactor = 0;
	}
	backgroundSh.SetUniform1f(basicSize + backgroundBlendFactor, blendFactor);
	if (cameraTransform[0] - Blocks::xMax / 2.0f > 0)
	{
		ErrorGL(glActiveTexture(GL_TEXTURE1));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[3]));

	}
	else
	{
		ErrorGL(glActiveTexture(GL_TEXTURE1));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[4]));
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Tex[0]);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}