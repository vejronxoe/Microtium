#include"NumberRender.h"


#include"Opengl/Texture.h"
#include"Opengl/DrawData.h"
#include"math/matrix.h"
#include"glfw/window.h"

#include<vector>

unsigned int CreateDrawDataNumbers(unsigned int eob
	, unsigned int& numberTexture
	, unsigned int& dotTex
	, unsigned int& dotDD)
{
	numberTexture = CreateTextureLinearRGBA("res/textures/Numbers.png");
	dotTex = CreateTextureLinearRGBA("res/textures/dot.png");
	dotDD = CreateDrawData(eob, 1, 0, 1, 0);
	return CreateDrawData(eob,1,0,1,0,1,0,0.1,0);
}
void drawNumber(float bottom
	, float right
	, float left
	, unsigned short int value
	, float* scale
	, float* transform
	, Shader Sh)
{ 
	if (value > 1)
	{
		
		int fourthDigit = (value / 1000);
		int thirdDigit = ((value / 100) - fourthDigit * 10);
		int secondDigit = ((value / 10) - fourthDigit * 100 - thirdDigit * 10);
		int firstDigit = value - fourthDigit * 1000 - thirdDigit * 100 - secondDigit * 10;

		float oneLeterSize = (right - left) / 4;
		ChangeScale(oneLeterSize, oneLeterSize, scale);
		Sh.SetUniformMat4(numberScale, scale);
	

		Sh.SetUniform1i(numberLetter, firstDigit);
		ChangeTransform(left + oneLeterSize * 3, bottom, transform);
		Sh.SetUniformMat4(numberTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (secondDigit || thirdDigit || fourthDigit)
		{
			Sh.SetUniform1i(numberLetter, secondDigit);
			ChangeTransform(left + oneLeterSize * 2, bottom, transform);
			Sh.SetUniformMat4(numberTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		if (thirdDigit || fourthDigit)
		{
			Sh.SetUniform1i(numberLetter, thirdDigit);
			ChangeTransform(left + oneLeterSize, bottom, transform);
			Sh.SetUniformMat4(numberTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		if (fourthDigit)
		{
			Sh.SetUniform1i(numberLetter, fourthDigit);
			ChangeTransform(left , bottom, transform);
			Sh.SetUniformMat4(numberTransform, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
	}
}
void drawTwoNumbersWithZero(float bottom
	, float right
	, float left
	, unsigned short int value
	, float* scale
	, float* transform
	, Shader Sh)
{

	int thirdDigit = ((value / 100));
	int secondDigit = ((value / 10) - thirdDigit * 10);
	int firstDigit = value - thirdDigit * 100 - secondDigit * 10;

	float oneLeterSize = (right - left) / 2;
	ChangeScale(oneLeterSize, oneLeterSize, scale);
	Sh.SetUniformMat4(numberScale, scale);
	
	if (secondDigit)
	{
		Sh.SetUniform1i(numberLetter, secondDigit);
		ChangeTransform(left, bottom, transform);
		Sh.SetUniformMat4(numberTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		Sh.SetUniform1i(numberLetter, firstDigit);
		ChangeTransform(left + oneLeterSize, bottom, transform);
		Sh.SetUniformMat4(numberTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	else
	{
		Sh.SetUniform1i(numberLetter, firstDigit);
		ChangeTransform(((right - left)/4.0f)+ left, bottom, transform);
		Sh.SetUniformMat4(numberTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

	}

}
void drawFloat(float bottom
	, float left
	, float value
	, unsigned int numberTex
	, unsigned int numberDD
	, unsigned int dotTex
	, unsigned int dotDD
	, float* scale
	, float* transform
	, Shader Sh)
{
	
	std::vector<int> digits;
	int i = 0;
	float holder = floorf(value / pow(10, i)) - floorf(value / pow(10, i + 1)) * 10;
	digits.push_back(holder);
	i++;
	holder = 1;
	while (0 < holder)
	{
		holder = floorf(value / pow(10, i)) - floorf(value / pow(10, i + 1)) * 10;
		digits.push_back(holder);
		
		holder = value - (holder+1) * pow(10,i);

		i++;
	}


	ChangeScale(10, 10, scale);
	Sh.SetUniformMat4(numberScale, scale);
	for (i = 0; i < digits.size(); i++)
	{
		Sh.SetUniform1i(numberLetter, digits.at(i));
		ChangeTransform(left + 10 * (digits.size()-1) - 10 * i, bottom, transform);
		Sh.SetUniformMat4(numberTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

	}
	
	ErrorGL(glBindVertexArray(dotDD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, dotTex));
	Sh.SetUniform1i(numberLetter, 0);
	ChangeTransform(left + 10 * digits.size()  , bottom, transform);
	Sh.SetUniformMat4(numberTransform, transform);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

	
	ErrorGL(glBindVertexArray(numberDD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTex));
	Sh.SetUniform1i(numberLetter, floorf((value - floorf(value)) * 10));
	ChangeTransform(left + 10 * (digits.size()+1), bottom, transform);
	Sh.SetUniformMat4(numberTransform, transform);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));


}