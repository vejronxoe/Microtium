#pragma once
#include"ErrorSystem.h"
#include<vector>
unsigned int CreateDrawData(unsigned int eob
	,float corTop
	, float corBottom
	, float corRight
	, float corLeft
	, float texTop = 1
	, float texBottom = 0
	, float texRight = 1
	, float texLeft = 0);
unsigned int CreateDrawData(unsigned int eob
	, float corTop
	, float corBottom
	, float corRight
	, float corLeft
	, unsigned int& VBO
	, float texTop = 1
	, float texBottom = 0
	, float texRight = 1
	, float texLeft = 0);
unsigned int CreateDrawData(unsigned int& EOB
	, unsigned int& VBO
	, unsigned int& sizeEOB
	, std::vector<uint16_t>& order
	, std::vector<float>& vertices);
unsigned int CreateDrawData(unsigned int& EOB
	, unsigned int& VBO
	, unsigned int& sizeEOB
	, std::vector<uint8_t>& order
	, std::vector<float>& vertices);