#include"matrix.h"

#include<math.h>

void CreateCamera(float Xmin, float Xmax, float Ymin, float Ymax, float* matrix4x4f)
{
	matrix4x4f[0] = 2.0f / ((Xmax - Xmin)); matrix4x4f[4] = 0.0f; matrix4x4f[8] = 0.0f; matrix4x4f[12] = -(Xmax + Xmin) / (Xmax - Xmin);
	matrix4x4f[1] = 0.0f; matrix4x4f[5] = 2.0f / ((Ymax - Ymin)); matrix4x4f[9] = 0.0f; matrix4x4f[13] = -(Ymax + Ymin) / (Ymax - Ymin);
	matrix4x4f[2] = 0.0f; matrix4x4f[6] = 0.0f;  matrix4x4f[10] = 1.0f; matrix4x4f[14] = 0.0f;
	matrix4x4f[3] = 0.0f; matrix4x4f[7] = 0.0f;  matrix4x4f[11] = 0.0f; matrix4x4f[15] = 1.0f;

}
void CreateTransform(float x, float y, float* matrix4x4f)
{
	matrix4x4f[0] = 1.0f; matrix4x4f[4] = 0.0f; matrix4x4f[8] = 0.0f; matrix4x4f[12] = x;
	matrix4x4f[1] = 0.0f; matrix4x4f[5] = 1.0f; matrix4x4f[9] = 0.0f; matrix4x4f[13] = y;
	matrix4x4f[2] = 0.0f; matrix4x4f[6] = 0.0f;  matrix4x4f[10] = 1.0f; matrix4x4f[14] = 0.0f;
	matrix4x4f[3] = 0.0f; matrix4x4f[7] = 0.0f;  matrix4x4f[11] = 0.0f; matrix4x4f[15] = 1.0f;
}
void ChangeTransform(float x, float y, float* matrix4x4f)
{
	matrix4x4f[12] = x;
	matrix4x4f[13] = y;
}
void ChangeCamera(float Xmin, float Xmax, float Ymin, float Ymax, float* matrix4x4f)
{
	matrix4x4f[0] = 2.0f / ((Xmax - Xmin)); matrix4x4f[12] = -(Xmax + Xmin) / (Xmax - Xmin);
	matrix4x4f[5] = 2.0f / ((Ymax - Ymin)); matrix4x4f[13] = -(Ymax + Ymin) / (Ymax - Ymin);
}
void CreateScale(float x, float y, float* matrix4x4f)
{
	matrix4x4f[0] = x; matrix4x4f[4] = 0.0f; matrix4x4f[8] = 0.0f; matrix4x4f[12] = 0.0f;
	matrix4x4f[1] = 0.0f; matrix4x4f[5] = y; matrix4x4f[9] = 0.0f; matrix4x4f[13] = 0.0f;
	matrix4x4f[2] = 0.0f; matrix4x4f[6] = 0.0f;  matrix4x4f[10] = 1.0f; matrix4x4f[14] = 0.0f;
	matrix4x4f[3] = 0.0f; matrix4x4f[7] = 0.0f;  matrix4x4f[11] = 0.0f; matrix4x4f[15] = 1.0f;
}
void ChangeScale(float x, float y, float* matrix4x4f)
{
	matrix4x4f[0] = x;
	matrix4x4f[5] = y;
}
void ChangeRotation(float a, float* matrix4x4f)
{
	double angle = a * PI / 180;
	matrix4x4f[0] = cos(angle); matrix4x4f[4] = -sin(angle);
	matrix4x4f[1] = sin(angle); matrix4x4f[5] = cos(angle);
}
void CreateRotation(float a, float* matrix4x4f)
{
	double angle = a * PI / 180;
	matrix4x4f[0] = cos(angle); matrix4x4f[4] = -sin(angle); matrix4x4f[8] = 0.0f; matrix4x4f[12] = 0.0f;
	matrix4x4f[1] = sin(angle); matrix4x4f[5] = cos(angle); matrix4x4f[9] = 0.0f; matrix4x4f[13] = 0.0f;
	matrix4x4f[2] = 0.0f; matrix4x4f[6] = 0.0f;  matrix4x4f[10] = 1.0f; matrix4x4f[14] = 0.0f;
	matrix4x4f[3] = 0.0f; matrix4x4f[7] = 0.0f;  matrix4x4f[11] = 0.0f; matrix4x4f[15] = 1.0f;
}
