#pragma once
#include"ErrorSystem.h"
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