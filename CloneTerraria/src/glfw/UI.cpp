#include"glfw/UI.h"

#include<string>

#include"glfw/Window.h"
#include"glfw/input.h"
#include"math/VectorOperation.h"
#include"math/matrix.h"
#include"ItemList.h"
#include"opengl/DrawData.h"
#include"opengl/ErrorSystem.h"

void UITranslatorToPixels(float leftCoordinates
	, float downCoordinates
	, float rightCoordinates
	, float upCoordinates
	, float* vertices4fOutput
	, int stablePoint)
{
	float Scale = 0;
	float sizeDifference[2] = {};
	if (Window::height > Window::width)
	{
		Scale = Window::width;
		sizeDifference[0] = 0;
		sizeDifference[1] = (Window::height - Window::width) / 2.0f;
	}
	else
	{
		Scale = Window::height;
		sizeDifference[0] = (Window::width - Window::height) / 2.0f;
		sizeDifference[1] = 0;
	}

	vertices4fOutput[0] = leftCoordinates * Scale;
	vertices4fOutput[1] = upCoordinates * Scale;
	vertices4fOutput[2] = rightCoordinates * Scale;
	vertices4fOutput[3] = downCoordinates * Scale;

	switch (stablePoint)
	{
	case leftTop:
	case leftMiddle:
	case leftBottom:
		break;
	case middleTop:
	case middleMiddle:
	case middleBottom:
		vertices4fOutput[0] += sizeDifference[0];
		vertices4fOutput[2] += sizeDifference[0];
		break;
	case rightTop:
	case rightMiddle:
	case rightBottom:
		vertices4fOutput[0] += sizeDifference[0] * 2;
		vertices4fOutput[2] += sizeDifference[0] * 2;

		break;
	default:

		std::cout << "Error vectorOperation.cpp wrong stable point" << std::endl;
		break;
	}
	switch (stablePoint)
	{
	case leftTop:
	case middleTop:
	case rightTop:
		vertices4fOutput[1] += sizeDifference[1] * 2;
		vertices4fOutput[3] += sizeDifference[1] * 2;
		break;
	case leftMiddle:
	case middleMiddle:
	case rightMiddle:
		vertices4fOutput[1] += sizeDifference[1];
		vertices4fOutput[3] += sizeDifference[1];
		break;
	case leftBottom:
	case middleBottom:
	case rightBottom:
		break;
	default:
		std::cout << "Error vectorOperation.cpp wrong stable point" << std::endl;
		break;
	}

}
float DistanceOnUI(float distance)
{
	float Scale = 0;
	if (Window::height > Window::width)
	{
		Scale = Window::width;

	}
	else
	{
		Scale = Window::height;
	}
	return distance * Scale;
}
Slider::Slider(unsigned int SliderTex
	, unsigned int trailTex
	, unsigned int sliderDD
	, unsigned int eob
	, int stablePoint
	, float left
	, float down
	, float right
	, float top)
{
	CreateSlider(SliderTex, trailTex, sliderDD, eob, stablePoint, left, down, right, top);
}
void Slider::CreateSlider(unsigned int SliderTex
	, unsigned int trailTex
	, unsigned int sliderDD
	, unsigned int eob
	, int stablePoint
	, float left
	, float down
	, float right
	, float top)
{
	if (m_SliderDD != -1)
	{
		Delete();
	}
	UITranslatorToPixels(left, down, right, top, m_Vertices, stablePoint);
	m_SliderDD = sliderDD;
	m_TrailDD = CreateDrawData(eob ,m_Vertices[1], m_Vertices[3], m_Vertices[2], m_Vertices[0], m_TrailVBO);
	m_SliderTex = SliderTex;
	m_TrailTex = trailTex;
}
 float Slider::Update(float value
	 , bool& howerOver)
{
	 howerOver = false;

	 if (!m_IsActive && IsInArea(m_Vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
	 {

		 if (Input::LeftMousePress)
		 {
			 m_IsActive = true;

		 }
		 howerOver = true;
	 }
	if (Input::LeftMouseRelease)
	{
		m_IsActive = false;
	}
	if (m_IsActive)
	{
		howerOver = true;
		value = Clamp(((Input::XRawMousePos - m_Vertices[0]) / (m_Vertices[2] - m_Vertices[0])),0,1);


	}
	return value;
}
void Slider::Draw(Shader sh
	, float value
	, float* transform)
{
	float sliderX = Clamp(value* (m_Vertices[2] - m_Vertices[0]) + m_Vertices[0], m_Vertices[0], m_Vertices[2]);

	ChangeTransform( 0, 0, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_TrailDD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_TrailTex));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	ChangeTransform(sliderX, (m_Vertices[1] + m_Vertices[3]) / 2.0f, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_SliderDD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_SliderTex));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}
void Slider::Delete()
{
	
	glDeleteVertexArrays(1, &m_TrailDD);
	glDeleteBuffers(1, &m_TrailVBO);
}


CheckBox::CheckBox(unsigned int* tex
	, unsigned int eob
	, int stablePoint
	, float left
	, float down
	, float right
	, float top)
{
	Create(tex, eob, stablePoint, left, down, right, top);
}
void CheckBox::Create(unsigned int* tex
	, unsigned int eob
	, int stablePoint
	, float left
	, float down
	, float right
	, float top)
{
	if (m_DD != -1)
	{
		Delete();
	}
	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	UITranslatorToPixels(left, down, right, top, m_Vertices, stablePoint);
	m_DD = CreateDrawData(eob, m_Vertices[1], m_Vertices[3], m_Vertices[2], m_Vertices[0], m_VBO);
}
bool CheckBox::Update(bool isActive
	, bool& value)
{
	if (IsInArea(m_Vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
	{
		value = (!value && isActive) || (value && !isActive);
		return true;
	}
	return false;
}
void CheckBox::Draw(Shader sh
	, bool value
	, float* transform)
{
	ChangeTransform(0, 0, transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindVertexArray(m_DD));
	if (value)
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[1]));
	}
	else
	{
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[0]));
	}
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void CheckBox::Delete()
{
	glDeleteVertexArrays(1, &m_DD);
	glDeleteBuffers(1, &m_VBO);
}
void ChangeScreenSize( int newWidth, int newHeight)
{
	if (!Window::fullScreen)
	{
		Window::windowWidth = newWidth;
		Window::windowHeight = newHeight;
	}	
	Window::width = newWidth;
	Window::height = newHeight;
	glViewport(0, 0, Window::width, Window::height);
	float blockSize = DistanceOnUI(BlockSize);
	Window::halfHeightOfGameTransform = (Window::height / blockSize )/ 2.0f;
	Window::halfWidthOfGameTransform = (Window::width / blockSize )/ 2.0f;
	Window::FontSize = DistanceOnUI(1.0f / 100.0f);

	
}
