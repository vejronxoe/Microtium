#include"DrawData.h"

unsigned int CreateDrawData(unsigned int eob ,float corTop, float corBottom, float corRight, float corLeft, float texTop, float texBottom, float texRight, float texLeft )
{
	float vertices[16];
	vertices[0] = corLeft; vertices[1] = corTop; vertices[2] = texLeft; vertices[3] = texTop;
	vertices[4] = corRight; vertices[5] = corTop; vertices[6] = texRight; vertices[7] = texTop;
	vertices[8] = corRight; vertices[9] = corBottom; vertices[10] = texRight; vertices[11] = texBottom;
	vertices[12] = corLeft; vertices[13] = corBottom;  vertices[14] = texLeft; vertices[15] = texBottom;
	unsigned int drawData;
	unsigned int vertexBuffer;

	ErrorGL(glGenVertexArrays(1, &drawData));
	ErrorGL(glBindVertexArray(drawData));
	ErrorGL(glGenBuffers(1, &vertexBuffer));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

	ErrorGL(glBindVertexArray(0));
	return drawData;


}

unsigned int CreateDrawData(unsigned int eob
	, float corTop
	, float corBottom
	, float corRight
	, float corLeft
	, unsigned int& VBO
	, float texTop
	, float texBottom
	, float texRight
	, float texLeft)
{
	float vertices[16];
	vertices[0] = corLeft; vertices[1] = corTop; vertices[2] = texLeft; vertices[3] = texTop;
	vertices[4] = corRight; vertices[5] = corTop; vertices[6] = texRight; vertices[7] = texTop;
	vertices[8] = corRight; vertices[9] = corBottom; vertices[10] = texRight; vertices[11] = texBottom;
	vertices[12] = corLeft; vertices[13] = corBottom;  vertices[14] = texLeft; vertices[15] = texBottom;
	unsigned int drawData;

	ErrorGL(glGenVertexArrays(1, &drawData));
	ErrorGL(glBindVertexArray(drawData));
	ErrorGL(glGenBuffers(1, &VBO));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

	ErrorGL(glBindVertexArray(0));
	return drawData;


}
