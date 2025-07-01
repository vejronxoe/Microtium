#include"matrix.h"

float* Camera(float Xmin, float Xmax, float Ymin, float Ymax)
{
	float divisorX = 1 / ((Xmax - Xmin) * 0.5f);
	float divisorY = 1 / ((Ymax - Ymin) * 0.5f);
	float matrix[16] = {
		divisorX, 0.0f, 0.0f,  -(Xmin / divisorX) - 1.0f,
		0.0f, divisorY, 0.0f, -(Ymin / divisorY) - 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return matrix;
}