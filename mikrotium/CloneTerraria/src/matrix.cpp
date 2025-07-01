#include"matrix.h"

void Camera(float Xmin, float Xmax, float Ymin, float Ymax, float* matrix)
{
	matrix[0] = 2.0f / ((Xmax - Xmin)); matrix[4] = 0.0f; matrix[8] = 0.0f; matrix[12] = -(Xmax + Xmin) / (Xmax - Xmin);
	matrix[1] = 0.0f; matrix[5] = 2.0f / ((Ymax - Ymin)); matrix[9] = 0.0f; matrix[13] = -(Ymax + Ymin) / (Ymax - Ymin);
	matrix[2] = 0.0f; matrix[6] = 0.0f;  matrix[10] = 1.0f; matrix[14] = 0.0f;
	matrix[3] = 0.0f; matrix[7] = 0.0f;  matrix[11] = 0.0f; matrix[15] = 1.0f;

}