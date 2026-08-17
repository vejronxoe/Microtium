
#include"light.h"


#include<fstream>
#include<sstream>
#include<string>
#include<cmath>

#include"ItemList.h"
#include"opengl/ErrorSystem.h"
#include"opengl/Texture.h"
#include"math/matrix.h"
#include"ItemList.h"
#include"glfw/Window.h"
#include"math/VectorOperation.h"

void LightMapAlgorithm(int *lightMapSpace
, std::vector<int>& Stack
, std::vector<std::vector<uint8_t>>& walls
, std::vector<std::vector<Block>>& blocks
, std::vector<std::vector<float>> &StaticLightMap)
{
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		for (int j = lightMapSpace[1]; j >= lightMapSpace[3];j--)
		{
			if (j < 0)
			{
				break;
			}
			if (walls.at(i).at(j-Blocks::yMin) == t_Air)
			{
				StaticLightMap.at(i).at(j - Blocks::yMin) = 1;
				Stack.emplace_back(i);
				Stack.emplace_back(j - Blocks::yMin);
			}
		}
	}
    for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		for (int j = lightMapSpace[1]; j >= lightMapSpace[3];j--)
		{
			if(blocks.at(i).at(j- Blocks::yMin).m_Type == t_Torch)
			{
				StaticLightMap.at(i).at(j - Blocks::yMin) = 1;
				Stack.emplace_back(i);
				Stack.emplace_back(j - Blocks::yMin);
			}
			
        }
    }
	while (Stack.size() != 0)
	{
		int IndexY = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		int IndexX = Stack.at(Stack.size() - 1);
		Stack.pop_back();
		float baseLight = StaticLightMap.at(IndexX).at(IndexY);

		int rules[4] = { (lightMapSpace[0] == IndexX) , -(lightMapSpace[1] - Blocks::yMin == IndexY), -(lightMapSpace[2] == IndexX), (lightMapSpace[3] - Blocks::yMin == IndexY) };
		{
			int Table[2][4] = { {0,-1 + rules[0],0,1 + rules[2]}, {-1 + rules[3],0,1 + rules[1],0}};
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];
				int lightBlock = 2;
				if (blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Air 
                || blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Platform)
				{
					lightBlock = 1;
				}
				float hold = baseLight - 0.08f * lightBlock;
				if (hold > StaticLightMap.at(CheckingX).at(CheckingY))
				{
					StaticLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}

		{
			int Table[2][4] = { {-1 + rules[0],-1 + rules[0],1 + rules[2],1 + rules[2]}, {-1 + rules[3],1 +rules[1],1 + rules[1] ,-1 + rules[3]}};
			for (int i = 0; i < 4;i++)
			{
				int CheckingX = IndexX + Table[0][i];
				int CheckingY = IndexY + Table[1][i];
				int lightBlock = 2;
				if (blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Air
                || blocks.at(CheckingX).at(CheckingY - Blocks::xMin).m_Behavior == b_Platform)
				{
					lightBlock = 1;
				}
				float hold = baseLight - 0.08f * 1.4142f * lightBlock;
				if (hold > StaticLightMap.at(CheckingX).at(CheckingY))
				{
					StaticLightMap.at(CheckingX).at(CheckingY) = hold;
					Stack.emplace_back(CheckingX);
					Stack.emplace_back(CheckingY);
				}
			}

		}
	}
}
void CalculateLightMap(int chunkNumber
, std::vector<std::vector<Block>>& blocks
, std::vector<std::vector<uint8_t>>& walls
, std::vector<std::vector<float>>& StaticLightMap)
{
	int lightMapSpace[4] = { (chunkNumber - (chunkNumber / 54) * 54) * 20-14,((chunkNumber / 54) * 20) + Blocks::yMin + 34,(chunkNumber - (chunkNumber / 54) * 54) * 20+34,((chunkNumber / 54) * 20) + Blocks::yMin-14 };
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			if (lightMapSpace[i] >= Blocks::xMax)
			{
				lightMapSpace[i] = Blocks::xMax-1;
			}
			else if (lightMapSpace[i] < Blocks::xMin)
			{
				lightMapSpace[i] = Blocks::xMin;
			}
		}
		else
		{
			if (lightMapSpace[i] >= Blocks::yMax)
			{
				lightMapSpace[i] = Blocks::yMax-1;
			}
			else if (lightMapSpace[i] < Blocks::yMin)
			{
				lightMapSpace[i] = Blocks::yMin;
			}
		}
	}
	int lightMapHeight = lightMapSpace[1] - lightMapSpace[3];
	int lightMapWidth = lightMapSpace[2] - lightMapSpace[0];
	std::vector<int> Stack;
	for (int i = lightMapSpace[0]+1; i < lightMapSpace[2]; i++)
	{
		for (int j = lightMapSpace[3] + 1; j < lightMapSpace[1]; j++)
		{
			StaticLightMap.at(i).at(j-Blocks::yMin) = 0;
		}
	}
	for (int i = lightMapSpace[3]; i <= lightMapSpace[1];i++)
	{
		Stack.emplace_back(lightMapSpace[0]);
		Stack.emplace_back(i - Blocks::yMin);
		Stack.emplace_back(lightMapSpace[2]);
		Stack.emplace_back(i - Blocks::yMin);
	}
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2]; i++)
	{
		Stack.emplace_back(i);
		Stack.emplace_back(lightMapSpace[1] - Blocks::yMin);
		Stack.emplace_back(i);
		Stack.emplace_back(lightMapSpace[3] - Blocks::yMin);
	}
	LightMapAlgorithm(lightMapSpace, Stack, walls, blocks, StaticLightMap);

}

void CalculateLightMap(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap)
{
	int lightMapSpace[4] = {0,Blocks::yMax-1,Blocks::xMax-1,Blocks::yMin };
	std::vector<int> Stack;
	LightMapAlgorithm(lightMapSpace, Stack, walls, blocks, StaticLightMap);
}

void CreateLightMap(std::vector<std::vector<float>>& StaticLightMap
	, std::vector<std::vector<Block>>& blocks
	, std::vector<Projectile>& projectiles
	, float* playerTransform
	, unsigned int playerHand 
	, int x
	, int y
	, int width
	, int height
	, unsigned int &iD)
{	
	y -= Blocks::yMin;
	std::vector<uint8_t> data;
	data.assign(width * height,0);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i <  width; i++)
		{
		
			data.at(width*j + i) = ((uint8_t)(StaticLightMap.at(x + i).at(y + j)*255));

		}
	}

	std::vector<float> dynamicStack;
	if(playerHand == i_Torch)
    {
	    dynamicStack.emplace_back(playerTransform[0]);
	    dynamicStack.emplace_back(playerTransform[1]);
	    dynamicStack.emplace_back(1);
    }
	
	for(int i = 0; i < projectiles.size(); i++)
    {
        if(projectiles.at(i).m_ProjectileType == p_FireArrow
        || projectiles.at(i).m_ProjectileType == p_FireCannonBall
        || projectiles.at(i).m_ProjectileType == p_FireBall)
        {
            dynamicStack.emplace_back(projectiles.at(i).m_Transform[0]);
            dynamicStack.emplace_back(projectiles.at(i).m_Transform[1]);
            dynamicStack.emplace_back(0.3f);
        }
    }
	
	while (dynamicStack.size() != 0)
	{
		float emittingLight = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightY = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightX = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();

		float altTransform[2] = {lightX - x, lightY - (y + Blocks::yMin) }; 

		int index[2] = {static_cast<int>(round(lightX)) - x, static_cast<int>(round(lightY) - (y + Blocks::yMin)) };
		int maxLeanght = ceil(emittingLight/0.08f)+1;
		int SpaceOfLight[4] = {index[0] - maxLeanght, index[1] +maxLeanght, index[0] + maxLeanght, index[1] - maxLeanght};
		SpaceOfLight[0] = Clamp(SpaceOfLight[0], 0, width);
		SpaceOfLight[1] = Clamp(SpaceOfLight[1], 0, height);
		SpaceOfLight[2] = Clamp(SpaceOfLight[2], 0, width);
		SpaceOfLight[3] = Clamp(SpaceOfLight[3], 0, height);
		

		for (int i = SpaceOfLight[0]; i < SpaceOfLight[2]; i++)
		{
			for (int j = SpaceOfLight[3]; j <  SpaceOfLight[1]; j++)
			{
			
				int blockValue = 1;
				int searchIndex[2] = { i,j };
				float searchAdd[2] = {};
				if (searchIndex[1] != index[1] && searchIndex[0] != index[0])
				{
					
					float searchScale[2] = {};
					float searchLeanghts[2] = {};
					float leanght[3] = { altTransform[0] - searchIndex[0],altTransform[1] -searchIndex[1],Pyt2D(searchIndex[0] - altTransform[0], searchIndex[1] - altTransform[1])};
					searchAdd[0] = (leanght[0]) / std::abs(leanght[0]);
					searchAdd[1] = (leanght[1]) / std::abs(leanght[1]);
					searchScale[0] = Pyt2D( 1 ,leanght[1] / leanght[0]);
					searchScale[1] = Pyt2D(1, leanght[0] / leanght[1]);
					searchLeanghts[0] = searchScale[0];
					searchLeanghts[1] = searchScale[1];

					
					while (leanght[2] >= searchLeanghts[0] || leanght[2] >= searchLeanghts[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air && blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Platform)
						{
							blockValue = 2;
							break;
						}
						if (searchLeanghts[0] > searchLeanghts[1])
						{
							searchLeanghts[1] += searchScale[1];
							searchIndex[1] += searchAdd[1];
						}
						else
						{
							searchLeanghts[0] += searchScale[0];
							searchIndex[0] += searchAdd[0];
						}
					}
				}
				else if (searchIndex[0] != index[0])
				{
					searchAdd[0] = (index[0] - searchIndex[0]) / abs(index[0] - searchIndex[0]);
					while (searchIndex[0] != index[0])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air && blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Platform)
						{
							blockValue = 2;
							break;
						}
						searchIndex[0] += searchAdd[0];
					}
				}
				else if (searchIndex[1] != index[1])
				{
					searchAdd[1] = (index[1] - searchIndex[1]) / abs(index[1] - searchIndex[1]);
					while (searchIndex[1] != index[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air && blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Platform)
						{
							blockValue = 2;
							break;
						}
						searchIndex[1] += searchAdd[1];
					}
				}
				float hold = emittingLight - 0.08f * blockValue * Pyt2D(altTransform[0]- i, altTransform[1]-j);
				if (hold * 255 > data.at(width * j + i))
				{
					data.at(width * j + i) = hold * 255;
				}
			}
		}



	}
	
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data()));
}







/*
{
	
	y -= Blocks::yMin;
	std::vector<uint8_t> data;
	data.assign(width * height,0);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i <  width; i++)
		{
		
			data.at(width*j + i) = ((uint8_t)(StaticLightMap.at(x + i).at(y + j)*255));

		}
	}

	std::vector<float> dynamicStack;
	if(playerHand == i_Torch)
    {
        dynamicStack.emplace_back(1);
        dynamicStack.emplace_back(playerTransform[1]);
        dynamicStack.emplace_back(playerTransform[0]);
    }
	
	
	
	while (dynamicStack.size() != 0)
	{
		float emittingLight = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightY = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();
		float lightX = dynamicStack.at(dynamicStack.size() - 1);
		dynamicStack.pop_back();

		float altTransform[2] = {lightX - x, lightY - (y + Blocks::yMin) }; 

		int index[2] = {static_cast<int>(round(lightX)) - x, static_cast<int>(round(lightY) - (y + Blocks::yMin)) };
		int maxLeanght = ceil(emittingLight/0.08f)+1;
		int SpaceOfLight[4] = {index[0] - maxLeanght, index[1] +maxLeanght, index[0] + maxLeanght, index[1] - maxLeanght};
		SpaceOfLight[0] = Clamp(SpaceOfLight[0], 0, width);
		SpaceOfLight[1] = Clamp(SpaceOfLight[1], 0, height);
		SpaceOfLight[2] = Clamp(SpaceOfLight[2], 0, width);
		SpaceOfLight[3] = Clamp(SpaceOfLight[3], 0, height);
		

		for (int i = SpaceOfLight[0]; i < SpaceOfLight[2]; i++)
		{
			for (int j = SpaceOfLight[3]; j <  SpaceOfLight[1]; j++)
			{
			
				int blockValue = 1;
				int searchIndex[2] = { i,j };
				float searchAdd[2] = {};
				if (searchIndex[1] != index[1] && searchIndex[0] != index[0])
				{
					
					float searchScale[2] = {};
					float searchLeanghts[2] = {};
					float leanght[3] = { altTransform[0] - searchIndex[0],altTransform[1] -searchIndex[1],Pyt2D(searchIndex[0] - altTransform[0], searchIndex[1] - altTransform[1])};
					searchAdd[0] = (leanght[0]) / std::abs(leanght[0]);
					searchAdd[1] = (leanght[1]) / std::abs(leanght[1]);
					searchScale[0] = Pyt2D( 1 ,leanght[1] / leanght[0]);
					searchScale[1] = Pyt2D(1, leanght[0] / leanght[1]);
					searchLeanghts[0] = searchScale[0];
					searchLeanghts[1] = searchScale[1];

					
					while (leanght[2] >= searchLeanghts[0] || leanght[2] >= searchLeanghts[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air)
						{
							blockValue = 2;
							break;
						}
						if (searchLeanghts[0] > searchLeanghts[1])
						{
							searchLeanghts[1] += searchScale[1];
							searchIndex[1] += searchAdd[1];
						}
						else
						{
							searchLeanghts[0] += searchScale[0];
							searchIndex[0] += searchAdd[0];
						}
					}
				}
				else if (searchIndex[0] != index[0])
				{
					searchAdd[0] = (index[0] - searchIndex[0]) / abs(index[0] - searchIndex[0]);
					while (searchIndex[0] != index[0])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air)
						{
							blockValue = 2;
							break;
						}
						searchIndex[0] += searchAdd[0];
					}
				}
				else if (searchIndex[1] != index[1])
				{
					searchAdd[1] = (index[1] - searchIndex[1]) / abs(index[1] - searchIndex[1]);
					while (searchIndex[1] != index[1])
					{
						if (blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Air && blocks.at(searchIndex[0] + x).at(searchIndex[1] + y).m_Type != b_Platform)
						{
							blockValue = 2;
							break;
						}
						searchIndex[1] += searchAdd[1];
					}
				}
				float hold = emittingLight - 0.08f * blockValue * Pyt2D(altTransform[0]- i, altTransform[1]-j);
				if (hold * 255 > data.at(width * j + i))
				{
					data.at(width * j + i) = hold * 255;
				}
			}
		}



	}
	
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data()));
}*/