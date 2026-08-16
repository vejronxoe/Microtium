#pragma once
#include<glad/glad.h>
#include<iostream>
#include<vector>

#include"opengl/ErrorSystem.h"

enum uniformSh
{
	ShadowLocation = 0
	, ShadowSize
	, animCamera = 0
	, animTransform
	, animScale
	, animNumber
	, animLeangth
	, animSize
	, basicCamera = 0
	, basicTransform
	, basicSize
	, numberCamera = 0
	, numberTransform
	, numberScale
	, numberLetter
	, numberSize
	, handCamera = 0
	, handTransform
	, handScale
	, handRotation
	, handBeginTransform
	, handSize
	, HUDCamera = 0
	, HUDTransform
	, HUDScale
	, HUDBasicLocation
	, HUDSize
	, CrownCamera = 0
	, CrownTransform
	, CrownRotation
	, CrownSize
	, advancedCamera = 0
	, advancedTransform
	, advancedScale
	, advancedRotation
	, advancedSize
	, backgroundBlendFactor = 0 
	, particlesCamera = 0
	, particlesTransform
	, particlesScale
	, particlesRotation
	, particlesSize
	, particlesColor = 0
	, particlesFragSize
	, structureShadow = 0
	, structureLookAt
	, structureSize
	, HUDShadow = 0
	, HUDCraftingY
	, fragHUDSize
	, fontCamera = 0
	, fontTransform
	, fontSize
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

