
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
, std::vector<int>& stack
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
				bool edge = false;
				int rules[3] = { (lightMapSpace[0] == i) , -(lightMapSpace[1]== j), -(lightMapSpace[2] == i) };
				int table[2][4] = { {0,-1 + rules[0],0,1 + rules[2]}, {-1 ,0,1 + rules[1],0}};
				for(int l = 0 ;l < 4;l++ )
				{
				
					if(walls.at(i+table[0][l]).at(j + table[1][l]- Blocks::yMin) != t_Air)
					{
						edge =true;
						break;
					}
				}
				if(edge)
				{
					stack.emplace_back(i);
					stack.emplace_back(j - Blocks::yMin);
				}
				StaticLightMap.at(i).at(j - Blocks::yMin) = 1;
			}
		}
	}
    for (int i = lightMapSpace[0]; i <= lightMapSpace[2];i++)
	{
		for (int j = lightMapSpace[1]; j >= lightMapSpace[3];j--)
		{
			if(blocks.at(i).at(j- Blocks::yMin).m_Type == t_Torch)
			{
				bool edge = false;
				int rules[4] = { (0 == i) , -(Blocks::yMax-1 == j), -(Blocks::xMax-1 == i), (Blocks::yMin == j)};
				int table[2][4] = { {0,-1 + rules[0],0,1 + rules[2]}, {-1 + rules[3],0,1 + rules[1],0}};
				for(int l = 0 ;l < 4;l++ )
				{
					if(blocks.at(i+table[0][l]).at(j + table[1][l]- Blocks::yMin).m_Type != t_Torch)
					{
						edge =true;
						break;
					}
				}
				if(edge)
				{
					stack.emplace_back(i);
					stack.emplace_back(j - Blocks::yMin);
				}
				StaticLightMap.at(i).at(j - Blocks::yMin) = 1;
			}
			
        }
    }
	std::vector<int> lightStack;
	while(stack.size())
	{
	
		int y = stack.at(stack.size() - 1);
		stack.pop_back();
		int x = stack.at(stack.size() - 1);
		stack.pop_back();
		lightStack.emplace_back(x);
		lightStack.emplace_back(y);

		float emittingLight = StaticLightMap.at(x).at(y);
		while(lightStack.size())
		{
			int IndexY = lightStack.at(lightStack.size() - 1);
			lightStack.pop_back();
			int IndexX = lightStack.at(lightStack.size() - 1);
			lightStack.pop_back();

			int rules[4] = { (lightMapSpace[0] == IndexX) , -(lightMapSpace[1] - Blocks::yMin == IndexY), -(lightMapSpace[2] == IndexX), (lightMapSpace[3] - Blocks::yMin == IndexY) };
			int Table[2][4] = { {0,-1 + rules[0],0,1 + rules[2]}, {-1 + rules[3],0,1 + rules[1],0}};
			for(int l = 0; l < 4;l++)
			{
				int i = Table[0][l] + IndexX;
				int j = Table[1][l] + IndexY;
				float blockValue = 0;
				float dist[3] ={static_cast<float>(i - x),static_cast<float>(j - y),Pyt2D(static_cast<float>(i - x), static_cast<float>(j - y))};
    			int step[2] = {static_cast<int>((dist[0] == 0) ? 0 : dist[0]/std::abs(dist[0])),static_cast<int>((dist[1] == 0) ? 0 : dist[1]/std::abs(dist[1]))};
    			float deltaDist[2] = {(step[0] == 0) ? INFINITY : std::abs(dist[2]/dist[0]), (step[1] == 0) ? INFINITY : std::abs(dist[2] / dist[1])};					
    			float sideDist[2] = {deltaDist[0],deltaDist[1]};			
				int searchPos[2] ={x,y};
				float leanghtBefore = 0;
				while (i != searchPos[0] || j != searchPos[1])
				{
					if (sideDist[0] > sideDist[1])
					{
						blockValue += (sideDist[1] - leanghtBefore) *0.08f * (1 + (blocks.at(searchPos[0]).at(searchPos[1]).m_Behavior != b_Air && blocks.at(searchPos[0]).at(searchPos[1]).m_Behavior != b_Platform));
						leanghtBefore = sideDist[1];
						sideDist[1] += deltaDist[1];
						searchPos[1] += step[1];
					}
						else
					{
						blockValue += (sideDist[0] - leanghtBefore) * 0.08f * (1 + (blocks.at(searchPos[0]).at(searchPos[1]).m_Behavior != b_Air && blocks.at(searchPos[0]).at(searchPos[1]).m_Behavior != b_Platform));
						leanghtBefore = sideDist[0];
						sideDist[0] += deltaDist[0];
						searchPos[0] += step[0];
					}
				}
				float hold = emittingLight - blockValue;
				if (hold >	StaticLightMap.at(i).at(j))
				{
					lightStack.emplace_back(i);
					lightStack.emplace_back(j);
					StaticLightMap.at(i).at(j) = hold;
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
	std::vector<int> stack;
	for (int i = lightMapSpace[0]+1; i < lightMapSpace[2]; i++)
	{
		for (int j = lightMapSpace[3] + 1; j < lightMapSpace[1]; j++)
		{
			StaticLightMap.at(i).at(j-Blocks::yMin) = 0;
		}
	}
	for (int i = lightMapSpace[3]; i <= lightMapSpace[1];i++)
	{
		stack.emplace_back(lightMapSpace[0]);
		stack.emplace_back(i - Blocks::yMin);
		stack.emplace_back(lightMapSpace[2]);
		stack.emplace_back(i - Blocks::yMin);
	}
	for (int i = lightMapSpace[0]; i <= lightMapSpace[2]; i++)
	{
		stack.emplace_back(i);
		stack.emplace_back(lightMapSpace[1] - Blocks::yMin);
		stack.emplace_back(i);
		stack.emplace_back(lightMapSpace[3] - Blocks::yMin);
	}
	LightMapAlgorithm(lightMapSpace, stack, walls, blocks, StaticLightMap);

}

void CalculateLightMap(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap)
{
	for(int i = 0;i < StaticLightMap.size();i++)
	{
		for(int j = 0;j < StaticLightMap.at(i).size();j++)
		{
			StaticLightMap.at(i).at(j) = 0;
		}
	}
	int lightMapSpace[4] = {0,Blocks::yMax-1,Blocks::xMax-1,Blocks::yMin };
	std::vector<int> stack;
	LightMapAlgorithm(lightMapSpace, stack, walls, blocks, StaticLightMap);
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
		if(altTransform[0] > 1 && altTransform[0]  < width-2 && altTransform[1] > 1 && altTransform[1] < height-2)
		{

			int index[2] = {static_cast<int>(round(lightX)) - x, static_cast<int>((round(lightY)) - (y + Blocks::yMin)) };
			int maxLeanght = ceil(emittingLight/0.08f);
			int SpaceOfLight[4] = {index[0] - maxLeanght, index[1] +maxLeanght, index[0] + maxLeanght, index[1] - maxLeanght};
			SpaceOfLight[0] = Clamp(SpaceOfLight[0], 0, width);
			SpaceOfLight[1] = Clamp(SpaceOfLight[1], 0, height);
			SpaceOfLight[2] = Clamp(SpaceOfLight[2], 0, width);
			SpaceOfLight[3] = Clamp(SpaceOfLight[3], 0, height);

			for (int i = SpaceOfLight[0]; i < SpaceOfLight[2]; i++)
			{
				for (int j = SpaceOfLight[3]; j <  SpaceOfLight[1]; j++)
				{

					if ( Pyt2D(i- altTransform[0], j- altTransform[1]) >= maxLeanght)	
					{
						continue;
					}
					float blockValue = 0;
					float dist[3] ={i - altTransform[0],j - altTransform[1],Pyt2D(i - altTransform[0], j - altTransform[1])};
    				int step[2] = {static_cast<int>((dist[0] == 0) ? 0 : dist[0]/std::abs(dist[0])),static_cast<int>((dist[1] == 0) ? 0 : dist[1]/std::abs(dist[1]))};

    				float sideDist[2] = {};			
    				float deltaDist[2] = {(step[0] == 0) ? INFINITY : std::abs(dist[2]/dist[0]), (step[1] == 0) ? INFINITY : std::abs(dist[2] / dist[1])};					
					int searchPos[2] ={index[0],index[1]};
					if (step[0] == -1)
					{
						sideDist[0] = ((altTransform[0]+0.5f) - index[0] )*deltaDist[0];
					}
					else
					{
						sideDist[0] = (1+index[0]-(altTransform[0]+0.5f))*deltaDist[0];
					}
					if (step[1] == -1)
					{
						sideDist[1] = ((altTransform[1]+0.5f) - index[1] )*deltaDist[1];
					}
					else
					{
						sideDist[1] = (1+index[1]-(altTransform[1]+0.5f))*deltaDist[1];
					}
				
					float leanghtBefore = 0;
					while (i != searchPos[0] || j != searchPos[1])
					{
					
						if (sideDist[0] > sideDist[1])
						{
							blockValue += (sideDist[1] - leanghtBefore) *0.08f * (1 + (blocks.at(searchPos[0] + x).at(searchPos[1] + y).m_Behavior != b_Air && blocks.at(searchPos[0] + x).at(searchPos[1] + y).m_Behavior != b_Platform));
							leanghtBefore = sideDist[1];
							sideDist[1] += deltaDist[1];
							searchPos[1] += step[1];
						}
			 			else
						{
							blockValue += (sideDist[0] - leanghtBefore) * 0.08f * (1 + (blocks.at(searchPos[0] + x).at(searchPos[1] + y).m_Behavior != b_Air && blocks.at(searchPos[0] + x).at(searchPos[1] + y).m_Behavior != b_Platform));
							leanghtBefore = sideDist[0];
							sideDist[0] += deltaDist[0];
							searchPos[0] += step[0];
						}

					}
					int hold = 255*(emittingLight - blockValue);
					if (hold > data.at(width * j + i))
					{
						data.at(width * j + i) = hold;
					}
				
				
				}
			}
		}
	}
	
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data()));
}






