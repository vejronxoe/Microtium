#include"NumberRender.h"


#include"Opengl/Texture.h"
#include"Opengl/DrawData.h"
#include"math/matrix.h"
#include"glfw/window.h"


unsigned int CreateDrawDataNumbers(unsigned int eob, unsigned int& numberTexture)
{
	numberTexture = CreateTextureRGBA("res/textures/Numbers.png");
	return CreateDrawData(eob,1,0,1,0,0,1,0,0.1f);
}
void drawNumber(float bottom, float right, float left, unsigned short int value, unsigned int NumberDrawData,unsigned int numberLocation, unsigned int transformLocation, unsigned int scaleLocation, float* scale, float* transform, Shader Sh)
{ 
	if (value > 1)
	{
		
		int fourthDigit = (value / 1000);
		int thirdDigit = ((value / 100) - fourthDigit * 10);
		int secondDigit = ((value / 10) - fourthDigit * 100 - thirdDigit * 10);
		int firstDigit = value - fourthDigit * 1000 - thirdDigit * 100 - secondDigit * 10;

		float oneLeterSize = (right - left) / 4.0f;
		ChangeScale(oneLeterSize, oneLeterSize, scale);
		Sh.SetUniformMat4(scaleLocation, scale);
		bottom -= oneLeterSize;

		Sh.SetUniform1i(numberLocation, firstDigit);
		ChangeTransform(left, bottom, transform);
		Sh.SetUniformMat4(transformLocation, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		if (secondDigit || thirdDigit || fourthDigit)
		{
			Sh.SetUniform1i(numberLocation, secondDigit);
			ChangeTransform(left + oneLeterSize, bottom, transform);
			Sh.SetUniformMat4(transformLocation, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		if (thirdDigit || fourthDigit)
		{
			Sh.SetUniform1i(numberLocation, thirdDigit);
			ChangeTransform(left + oneLeterSize * 2, bottom, transform);
			Sh.SetUniformMat4(transformLocation, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		if (fourthDigit)
		{
			Sh.SetUniform1i(numberLocation, fourthDigit);
			ChangeTransform(left + oneLeterSize * 3, bottom, transform);
			Sh.SetUniformMat4(transformLocation, transform);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
	}
}