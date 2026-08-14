#pragma once
#include"opengl/Shader.h"
class Background
{
public:
	unsigned int m_DD[3];
	unsigned int m_Tex[5];
	Background(unsigned int eob, Shader backgroundSh);
	void DrawBackground(Shader backgroundSh, Shader basicSh, float* transform, float* cameraTransform);

};