#pragma once
#include<GL/glew.h>
#include<iostream>
#include<vector>

#include"ErrorSystem.h"

enum ShadowFragmentSh
{
	ShadowLocation = 0
	, ShadowSize
};
enum AnimationVertexSh
{
	animCamera = 0
	, animTransform
	, animScale
	, animNumber
	, animLeangth
	, animSize
};
enum BasicVertexSh
{
	basicCamera = 0
	, basicTransform
	, basicSize
};
enum FontVertexSh
{
	fontCamera = 0
	, fontTransform
	, fontScale
	, fontLetter
	, fontSize
};
enum HandVertexSh
{
	handCamera = 0
	, handTransform
	, handScale
	, handRotation
	, handBeginTransform
	, handSize
};
enum HUDVertexSh
{
	HUDCamera = 0
	, HUDTransform
	, HUDScale
	, HUDBasicLocation
	, HUDSize
};
enum TreeVertexSh
{
	treeCamera = 0
	, treeTransform
	, treeRotation
	, treeSize
};
enum advancedVertexSh
{
	advancedCamera = 0
	, advancedTransform
	, advancedScale
	, advancedRotation
	, advancedSize
};
enum backgroundVertexSh
{
	backgroundBlendFactor = 0 
};
enum particlesVertexSh
{
	particlesCamera = 0
	, particlesTransform
	, particlesScale
	, particlesRotation
	, particlesSize
};
enum particlesFragSh
{
	particlesColor = 0
	, particlesFragSize
};
enum structureVertexSh
{
	structureCamera = 0
	, structureTransform
	, structureScale
	, structureSize
};
class Shader
{
private:
	std::string fileShaderRead(std::string filePath);
	unsigned int CompileShader(unsigned int type, const char* source, const char* nameShader);
	void LinkBasicShader(unsigned int vertexShader, unsigned int fragmentShader);
public:
	unsigned int m_ID;
	std::vector<unsigned int> m_Locations;
	Shader(const char* VertexShaderSource, const char* FragmentShadersource);
	void DeleteShader();
	void GetUniformLocation(const char* name);
	void SetUniform4f(unsigned int location, float v0, float v1, float v2, float v3);
	void SetUniform1f(unsigned int location, float v);
	void SetUniform1i(unsigned int location, int v);
	void SetUniform1b(unsigned int location, bool v);
	void SetUniformMat4(unsigned int location, float* v);
	void Bind();
	void Unbind();
};

