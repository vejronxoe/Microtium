#pragma once
#include "ErrorSystem.h"


class squere
{
private:
	float m_Points[16];
	unsigned int m_Order[6];

	unsigned int m_ImageID;
	unsigned int m_PointsID;
	unsigned int m_OrderID;
	unsigned int m_ProgramID;

	unsigned char* m_LocalBuffer;
	int m_Height;
	int m_Width;
	int m_BPP;
	  
public:
	float m_Scale[2];
	float m_Transform[2]; 
private:
	void CountPoints();
	void Bind() const;
	void Unbind() const;
public:
	squere(int TansformX, int TansformY, int ScaleX, int ScaleY, std::string pathImageEXE, std::string pathImage, int TexSlot, std::string pathshaderEXE, std::string pathshader);
	void rendererObject();
};