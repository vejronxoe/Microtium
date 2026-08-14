#pragma once
#include"opengl/Shader.h"
struct Slider
{
	unsigned int m_SliderDD = -1;
	unsigned int m_TrailDD = -1;
	unsigned int m_TrailVBO = -1;
	unsigned int m_SliderTex = -1;
	unsigned int m_TrailTex = -1;
	float m_Vertices[4] = {};
	bool m_IsActive = false;
	Slider() = default;
	Slider(unsigned int SliderTex
		, unsigned int trailTex
		, unsigned int sliderDD
		, unsigned int eob
		, int stablePoint
		, float left
		, float down
		, float right
		, float top);
	void CreateSlider(unsigned int SliderTex
		, unsigned int trailTex
		, unsigned int sliderDD
		, unsigned int eob
		, int stablePoint
		, float left
		, float down
		, float right
		, float top);
	float Update(float value
		, bool& howerOver);
	void Draw(Shader sh
		, float value
		, float* transform);
	void Delete();
};
struct CheckBox
{
	unsigned int m_DD = -1;
	unsigned int m_VBO = -1;
	unsigned int m_Tex[2] = {};
	float m_Vertices[4] = {};
	CheckBox() = default;
	CheckBox(unsigned int* tex
		, unsigned int eob
		, int stablePoint
		, float left
		, float down
		, float right
		, float top);
	void Create(unsigned int* tex
		, unsigned int eob
		, int stablePoint
		, float left
		, float down
		, float right
		, float top);
	bool Update(bool isActive
		, bool& value);
	void Draw(Shader sh
		, bool value
		, float* transform);
	void Delete();
};

void UITranslatorToPixels(float leftCoordinates
	, float downCoordinates
	, float rightCoordinates
	, float upCoordinates
	, float* vertices4fOutput
	, int stablePoint);
void ChangeScreenSize(int newWidth, int newHeight);

float DistanceOnUI(float distance);
