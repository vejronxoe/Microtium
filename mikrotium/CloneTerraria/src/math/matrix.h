#pragma once
void ChangeCamera(float Xmin, float Xmax, float Ymin, float Ymax, float* matrix4x4f);
void CreateCamera(float Xmin, float Xmax, float Ymin, float Ymax, float* matrix4x4f);
void ChangeTransform(float x, float y, float* matrix4x4f);
void CreateTransform(float x, float y, float* matrix4x4f);
void ChangeScale(float x, float y, float* matrix4x4f);
void CreateScale(float x, float y, float* matrix4x4f);
void ChangeRotation(float a, float* matrix4x4f);
void CreateRotation(float a, float* matrix4x4f);
