#include "Font.h"

#include<iostream>
#include<fstream>

#include "../math/VectorOperation.h"
#include "../math/matrix.h"
#include "window.h"
#include "../libraries/json.hpp"
#include "../Opengl/ErrorSystem.h"
#define BEGINLETTER 32	

void LoadFont(std::vector<Letter>& letters
	, float& lineHeight
	, const char* fileName)
{
	std::ifstream f(fileName);
	nlohmann::json j = nlohmann::json::parse(f);
	float atlasWidth = j["atlas"]["width"];
	float atlasHeight = j["atlas"]["height"];
	float sizeDefalut = GetVisibleScale(60,120,Window::height,Window::width);
	float pixelDefalut = 16;
	auto glyphs = j["glyphs"];
	if('A' - BEGINLETTER < glyphs.size())
	{
		
		float pixelLeft = glyphs['A' - BEGINLETTER]["atlasBounds"]["left"];
		float pixelBottom = glyphs['A' - BEGINLETTER]["atlasBounds"]["bottom"];
		float pixelRight = glyphs['A' - BEGINLETTER]["atlasBounds"]["right"];
		float pixelTop = glyphs['A' - BEGINLETTER]["atlasBounds"]["top"];
		pixelDefalut = pixelTop - pixelBottom;

	}

	letters.resize(glyphs.size());
	lineHeight = sizeDefalut;
	for (int i = 0; i < glyphs.size(); ++i)
	{
		const auto& glyph = glyphs[i];

		if (glyph.contains("atlasBounds"))
		{
			float pixelLeft = glyph["atlasBounds"]["left"];
			float pixelBottom = glyph["atlasBounds"]["bottom"];
			float pixelRight = glyph["atlasBounds"]["right"];
			float pixelTop = glyph["atlasBounds"]["top"];
			letters[i].m_DefalutSize[0] = sizeDefalut * (pixelRight - pixelLeft) / pixelDefalut;
			letters[i].m_DefalutSize[1] = sizeDefalut * (pixelTop - pixelBottom) / pixelDefalut;
			if (lineHeight < letters[i].m_DefalutSize[1])
			{
				lineHeight = letters[i].m_DefalutSize[1];
			}
			letters[i].m_UVCoordinates[0] = pixelLeft / atlasWidth;
			letters[i].m_UVCoordinates[1] = 1.0f - (pixelBottom / atlasHeight);
			letters[i].m_UVCoordinates[2] = pixelRight / atlasWidth;
			letters[i].m_UVCoordinates[3] = 1.0f - (pixelTop / atlasHeight);   

		}
		else
		{
			
			letters[i].m_UVCoordinates[0] = -1;
			letters[i].m_UVCoordinates[1] = -1;
			letters[i].m_UVCoordinates[2] = -1;
			letters[i].m_UVCoordinates[3] = -1;
		}
	}
	
}
Format::Format(int LastLetterOfFormat
	, float sizeOfLetter
	, float red
	, float green
	, float blue
	, float alpha)
{
	m_Color[0] = red;
	m_Color[1] = green;
	m_Color[2] = blue;
	m_Color[3] = alpha;
	m_LastLetterOfFormat = LastLetterOfFormat;
	m_SizeOfLetter = sizeOfLetter;
}

Text::Text(std::string Letters
	, std::vector<Format> formats
	, std::vector<Letter>& Anscii
	, float lineHeight
	, char stablePoint
	, float x
	, float y)
{
	if (Letters.length() == 0)
	{
		assert(true);
	}

	m_Transform[0] = x;
	m_Transform[1] = y;
	m_TextVertices[0] = 0;
	m_TextVertices[1] = 0;
	m_TextVertices[2] = 0;
	m_TextVertices[3] = 0;
	for (int i = 0; i < formats.size(); i++)
	{
		formats.at(i).m_LastLetterOfFormat = Clamp(formats.at(i).m_LastLetterOfFormat, 0, Letters.length() - 1);
	}
	for (int i = 0; i < formats.size(); i++)
	{
		for (int j = 1; j < formats.size(); j++)
		{
			if (formats.at(j - 1).m_LastLetterOfFormat > formats.at(j).m_LastLetterOfFormat)
			{
				Format holder = formats[j - 1];
				formats[j - 1] = formats[j];
				formats[j] = holder;

			}
		}
	}
	std::vector<float> lineHeights;
	float biggestSizeOfLetter = 1;
	for (int i = 0; i < Letters.length(); i++)
	{
		float sizeOfLetter = 1;
		for (int j = 0; j < formats.size(); j++)
		{
			if (i <= formats.at(j).m_LastLetterOfFormat)
			{
				sizeOfLetter = formats[j].m_SizeOfLetter;
				break;

			}
		}
		if (sizeOfLetter > biggestSizeOfLetter)
		{
			biggestSizeOfLetter = sizeOfLetter;
		}
		if (Letters.at(i) != '\n')
		{
			
			if (Letters[i] != ' ')
			{
				m_TextVertices[2] += Anscii.at(Letters[i] - BEGINLETTER).m_DefalutSize[0] * sizeOfLetter;
			}
			else
			{
				m_TextVertices[2] += sizeOfLetter * lineHeight / 2;
			}
		}
		else
		{
			m_TextVertices[3] -= lineHeight * biggestSizeOfLetter;
			lineHeights.emplace(lineHeights.begin(), lineHeight * biggestSizeOfLetter);
			biggestSizeOfLetter = 1;

		}

	}
	m_TextVertices[3] -= lineHeight * biggestSizeOfLetter;
	lineHeights.emplace(lineHeights.begin(), lineHeight * biggestSizeOfLetter);


	switch (stablePoint)
	{
	case leftTop:
	case leftMiddle:
	case leftBottom:
		break;
	case middleTop:
	case middleMiddle:
	case middleBottom:
		m_TextVertices[0] -= m_TextVertices[2] / 2.0f;
		m_TextVertices[2] = m_TextVertices[2] / 2.0f;
		break;
	case rightTop:
	case rightMiddle:
	case rightBottom:
		m_TextVertices[0] -= m_TextVertices[2];
		m_TextVertices[2] = 0;
		break;
	default:
		assert(true);
		std::cout << "Error font.cpp wrong stable point" << std::endl;
		break;
	}
	switch (stablePoint)
	{
	case leftTop:
	case middleTop:
	case rightTop:
		break;
	case leftMiddle:
	case middleMiddle:
	case rightMiddle:
		m_TextVertices[1] -= m_TextVertices[3] / 2.0f;
		m_TextVertices[3] = m_TextVertices[3] / 2.0f;
		break;
	case leftBottom:
	case middleBottom:
	case rightBottom:
		m_TextVertices[1] -= m_TextVertices[3];
		m_TextVertices[3] = 0;
		break;
	default:
		assert(true);
		std::cout << "Error font.cpp wrong stable point" << std::endl;
		break;
	}

	std::vector<float> vertices;
	std::vector<unsigned char> order;
	float textVertices[2] =
	{ m_TextVertices[0]
	, m_TextVertices[1] - lineHeights[lineHeights.size() - 1] };
	lineHeights.pop_back();


	for (int i = 0; i < Letters.length(); i++)
	{
		float color[4] = { 1,1,1,1 };
		float sizeOfLetter = 1;

		for (int j = 0; j < formats.size(); j++)
		{
			if (i <= formats.at(j).m_LastLetterOfFormat)
			{
				sizeOfLetter = formats[j].m_SizeOfLetter;
				color[0] = formats[j].m_Color[0];
				color[1] = formats[j].m_Color[1];
				color[2] = formats[j].m_Color[2];
				color[3] = formats[j].m_Color[3];
				break;
			}
		}

		if (Letters.at(i) != '\n')
		{
			



			if (Letters[i] != ' ')
			{
				int n = vertices.size() / 8;

				vertices.resize(vertices.size() + 32);
				//1
				vertices[vertices.size() - 32] = textVertices[0];
				vertices[vertices.size() - 31] = textVertices[1] + Anscii.at(Letters[i] - BEGINLETTER).m_DefalutSize[1] * sizeOfLetter;

				vertices[vertices.size() - 30] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[0];
				vertices[vertices.size() - 29] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[3];

				vertices[vertices.size() - 28] = color[0];
				vertices[vertices.size() - 27] = color[1];
				vertices[vertices.size() - 26] = color[2];
				vertices[vertices.size() - 25] = color[3];
				//2							 
				vertices[vertices.size() - 24] = textVertices[0];
				vertices[vertices.size() - 23] = textVertices[1];

				vertices[vertices.size() - 22] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[0];
				vertices[vertices.size() - 21] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[1];

				vertices[vertices.size() - 20] = color[0];
				vertices[vertices.size() - 19] = color[1];
				vertices[vertices.size() - 18] = color[2];
				vertices[vertices.size() - 17] = color[3];
				//3							 
				vertices[vertices.size() - 16] = textVertices[0] + Anscii[Letters[i] - BEGINLETTER].m_DefalutSize[0] * sizeOfLetter;
				vertices[vertices.size() - 15] = textVertices[1];

				vertices[vertices.size() - 14] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[2];
				vertices[vertices.size() - 13] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[1];

				vertices[vertices.size() - 12] = color[0];
				vertices[vertices.size() - 11] = color[1];
				vertices[vertices.size() - 10] = color[2];
				vertices[vertices.size() - 9] = color[3];
				//4							
				vertices[vertices.size() - 8] = textVertices[0] + Anscii[Letters[i] - BEGINLETTER].m_DefalutSize[0] * sizeOfLetter;
				vertices[vertices.size() - 7] = textVertices[1] + Anscii[Letters[i] - BEGINLETTER].m_DefalutSize[1] * sizeOfLetter;

				vertices[vertices.size() - 6] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[2];
				vertices[vertices.size() - 5] = Anscii[Letters[i] - BEGINLETTER].m_UVCoordinates[3];

				vertices[vertices.size() - 4] = color[0];
				vertices[vertices.size() - 3] = color[1];
				vertices[vertices.size() - 2] = color[2];
				vertices[vertices.size() - 1] = color[3];




				order.resize(order.size() + 6);
				order[order.size() - 6] = n ;
				order[order.size() - 5] = n + 1;
				order[order.size() - 4] = n + 3;
				order[order.size() - 3] = n + 1;
				order[order.size() - 2] = n + 2;
				order[order.size() - 1] = n + 3;
				textVertices[0] += Anscii[Letters[i] - BEGINLETTER].m_DefalutSize[0] * sizeOfLetter;
			}
			else
			{
				textVertices[0] += sizeOfLetter * lineHeight / 2;
			}


		}
		else
		{
			textVertices[1] -= lineHeights[lineHeights.size() - 1];
			lineHeights.pop_back();
			textVertices[0] = m_TextVertices[0];
			biggestSizeOfLetter = 1;
		}

	}
	m_OrderSize = order.size();



	ErrorGL(glGenVertexArrays(1, &m_DrawData));
	ErrorGL(glBindVertexArray(m_DrawData));
	ErrorGL(glGenBuffers(1, &m_VBO));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));
	ErrorGL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(2));

	ErrorGL(glGenBuffers(1, &m_EOB));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EOB));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size(), order.data(), GL_STATIC_DRAW));


	ErrorGL(glBindVertexArray(0));
}
void Text::Draw(Shader& fontSh
	, float* transform
	, unsigned int fontTex)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	fontSh.SetUniformMat4(fontTransform, transform);	

	ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));
	ErrorGL(glBindVertexArray(m_DrawData));
	ErrorGL(glDrawElements(GL_TRIANGLES, m_OrderSize, GL_UNSIGNED_BYTE, 0));
}
void Text::deleteText()
{
	glDeleteBuffers(1, &m_EOB);
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_DrawData);
}
