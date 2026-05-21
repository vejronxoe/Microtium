#include"BlocksAndWalls.h"

#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<string>

#include"Opengl/ErrorSystem.h"
#include"Opengl/Texture.h"
#include"math/matrix.h"
#include"ItemList.h"
#include"glfw/Window.h"
#include"math/VectorOperation.h"

namespace Blocks
{
	int xMax;
	int xMin;
	int yMax;
	int yMin;
};

unsigned char getBehaviorByType(unsigned char blocksType)
{
	blocksType = Clamp(blocksType,0,t_BlocksSize-1);
	unsigned char behaviours[t_BlocksSize];
	for (int i = 0; i < t_BlocksSize; i++)
	{
		behaviours[i] = b_BasicSolid;
	}
	behaviours[t_Ice] = b_Slippery;
	behaviours[t_Asphalt] = b_Asphalt;
	behaviours[t_Platform] = b_Platform;
	behaviours[t_DoorBlock] = b_Indestructible;
	behaviours[t_Sand] = b_Sand;

	return behaviours[blocksType];
	

}
unsigned char getTypeByItem(unsigned char item)
{
	item = Clamp(item, 0, i_ItemSize-1);
	int types[i_ItemSize] = {t_Dirt};
	for (int i = 0; i < t_BlocksSize; i++)
	{
		types[i] = t_Dirt;
	}
	types[i_Ice] = t_Ice;
	types[i_Asphalt] = t_Asphalt;
	types[i_Platform] = t_Platform;
	types[i_ForestPlank] = t_ForestPlank;
	types[i_Sand] = t_Sand;
	types[i_WallIce] = t_Ice;
	return types[item];
}
unsigned char GetWallItemBytype(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char WallIDs[t_BlocksSize] = {i_WallDirt};
	for (int i = 0; i < t_BlocksSize; i++)
	{
		WallIDs[i] = i_WallDirt;
	}
	WallIDs[t_Ice] = i_WallIce;
	return WallIDs[blocksType];

}
unsigned char GetBlockItemByType(unsigned char blocksType)
{
	blocksType = Clamp(blocksType, 0, t_BlocksSize - 1);
	unsigned char blocksIDs[t_BlocksSize] = { i_Dirt };
	for (int i = 0; i < t_BlocksSize; i++)
	{
		blocksIDs[i] = i_Dirt;
	}
	blocksIDs[t_Ice] = i_Ice;
	blocksIDs[t_Asphalt] = i_Asphalt;
	blocksIDs[t_Platform] = i_Platform;
	blocksIDs[t_ForestPlank] = i_ForestPlank;
	blocksIDs[t_Sand] = i_Sand;
	return blocksIDs[blocksType];
}


DamagedBlock::DamagedBlock(int x
	, int y
	, char HP)
	:m_Transform{ x, y }, m_HP(HP)
{
}
void DamagedBlock::DrawDamage(Shader& basicShader
	, float* transform
	, unsigned int* texture)
{

	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	basicShader.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texture[m_HP - 1]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}



Block::Block(unsigned char blockType
	, int y)
{
	blockType = Clamp(blockType, 0, t_BlocksSize -1);
	m_Y = y;
	m_Type = blockType;
	m_Behavior = getBehaviorByType(blockType);
	unsigned char hardness[t_BlocksSize] = { 15 };
	hardness[t_Asphalt] = 35;
	hardness[t_Platform] = 20;
	hardness[t_ForestPlank] = 20;
	m_Hardness = hardness[blockType];
}
Wall::Wall(unsigned char wallType
	, int y)
{
	m_Type = wallType;
	m_Y = y;
	unsigned char hardness[t_BlocksSize];
	for (int i = 0; i < t_BlocksSize; i++)
	{
		hardness[i] = 15;
	}
	hardness[t_Dirt] = 20;
	m_Hardness = hardness[wallType];
}
void Block::Draw(Shader& basicSh
	, unsigned int* blockTex
	, int x
	, float* transform)
{
	ChangeTransform(x, m_Y, transform);
	basicSh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[m_Type]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void Wall::Draw(Shader& shadowSh
	, unsigned int* blockTex
	, int x
	, float* transform)
{
	ChangeTransform(x, m_Y, transform);
	shadowSh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, blockTex[m_Type]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void CreateAllBlockTextures(unsigned int* IDs)
{
	IDs[t_TopGrass] = CreateTextureRepeatRGBA("res/textures/topGrassBlock.png");
	IDs[t_LeftGrass] = CreateTextureRepeatRGBA("res/textures/leftGrassBlock.png");
	IDs[t_DownGrass] = CreateTextureRepeatRGBA("res/textures/downGrassBlock.png");
	IDs[t_RightGrass] = CreateTextureRepeatRGBA("res/textures/rightGrassBlock.png");
	IDs[t_TopDownGrass] = CreateTextureRepeatRGBA("res/textures/topDownGrassBlock.png");
	IDs[t_LeftRightGrass] = CreateTextureRepeatRGBA("res/textures/leftRightGrassBlock.png");
	IDs[t_TopLeftGrass] = CreateTextureRepeatRGBA("res/textures/topLeftGrassBlock.png");
	IDs[t_DownLeftGrass] = CreateTextureRepeatRGBA("res/textures/downLeftGrassBlock.png");
	IDs[t_DownRightGrass] = CreateTextureRepeatRGBA("res/textures/downRightGrassBlock.png");
	IDs[t_TopRightGrass] = CreateTextureRepeatRGBA("res/textures/topRightGrassBlock.png");
	IDs[t_MissingTopGrass] = CreateTextureRepeatRGBA("res/textures/missingTopGrassBlock.png");
	IDs[t_MissingLeftGrass] = CreateTextureRepeatRGBA("res/textures/missingLeftGrassBlock.png");
	IDs[t_MissingDownGrass] = CreateTextureRepeatRGBA("res/textures/missingDownGrassBlock.png");
	IDs[t_MissingRightGrass] = CreateTextureRepeatRGBA("res/textures/missingRightGrassBlock.png");
	IDs[t_FullGrass] = CreateTextureRepeatRGBA("res/textures/fullGrassBlock.png");
	IDs[t_Dirt] = CreateTextureRepeatRGBA("res/textures/dirtBlock.png");
	IDs[t_Ice] = CreateTextureRepeatRGBA("res/textures/ice.png");
	IDs[t_Asphalt] = CreateTextureRepeatRGBA("res/textures/Asphalt.png");
	IDs[t_Platform] = CreateTextureRepeatRGBA("res/textures/platform.png");
	IDs[t_Platform] = CreateTextureRepeatRGBA("res/textures/platform.png");
	IDs[t_ForestPlank] = CreateTextureRepeatRGBA("res/textures/forestPlank.png");
	IDs[t_Sand] = CreateTextureRepeatRGBA("res/textures/sand.png");
	IDs[t_DoorBlock] = CreateTextureRepeatRGBA("res/textures/red.png");
}

void createWall(int x
	, int y
	, unsigned short int wallType
	, std::vector<int>& chunksToRebuildWalls
	, std::vector<std::vector<Wall>>& walls)
{
	int indexToPlace;
	for (indexToPlace = 0; indexToPlace < walls.at(x).size(); indexToPlace++)
	{
		if (walls.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuildWalls.size(); i++)
	{
		if (chunksToRebuildWalls[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuildWalls.emplace_back(chunkIndex);
	}
	walls.at(x).emplace(walls.at(x).begin() + indexToPlace, wallType, y);
}

void DestroyWall(std::vector<std::vector<Wall>>& Walls
	, std::vector<int>& chunksToRebuildWalls
	, int x
	, int y)
{
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuildWalls.size(); i++)
	{
		if (chunksToRebuildWalls[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuildWalls.emplace_back(chunkIndex);
	}
	int index;
	if (FindWall(Walls, x, y, index))
	{
		Walls.at(x).erase(Walls.at(x).begin()+index);
	}

}

void CreateBlock(int x
	, int y
	, unsigned short int blockType
	, std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX)
{
	
	int	indexOfTheWall; 
	
	if (blockType == t_Sand)
	{
		bool notThere = true;
		for (int i = 0; i < isThereSandOnX.size();i++)
		{
			if (x == isThereSandOnX[i])
			{
				notThere = false;
				break;
			}
		}
		if (notThere)
		{
			isThereSandOnX.emplace_back(x);
		}
	}
	int indexToPlace;
	for (indexToPlace = 0; indexToPlace < blocks.at(x).size(); indexToPlace++)
	{
		if (blocks.at(x).at(indexToPlace).m_Y < y)
		{
			break;
		}
	}
	blocks.at(x).emplace(blocks.at(x).begin() + indexToPlace, blockType,y);
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuild.size(); i++)
	{
		if (chunksToRebuild[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere) 
	{
		chunksToRebuild.emplace_back(chunkIndex);
	}
}
void DestroyBlock(std::vector<int>& chunksToRebuild
	, std::vector<std::vector<Block>>& blocks
	, std::vector<int>& isThereSandOnX
	, int x
	, int y)
{
	int index; 

	if (FindBlock(blocks, x, y, index))
	{
		if (blocks.at(x).at(index).m_Behavior == b_Sand)
		{
		
			bool isThereSand = false;
			for (int i = 0; i < blocks.at(x).size(); i++)
			{
				if (blocks.at(x).at(i).m_Behavior == b_Sand && blocks.at(x).at(i).m_Y != y)
				{
					isThereSand = true;
				}
			}
			if (!isThereSand)
			{
				for (int i = 0; i < isThereSandOnX.size();i++)
				{
					if (isThereSandOnX.at(i) == x)
					{
						isThereSandOnX.erase(isThereSandOnX.begin() + i);
						break;
					}
				}
			}
		}
		blocks.at(x).erase(blocks.at(x).begin() + index);
	}
	bool alreadyThere = false;
	int chunkIndex = FindChunk(x, y);
	for (int i = 0; i < chunksToRebuild.size(); i++)
	{
		if (chunksToRebuild[i] == chunkIndex)
		{
			alreadyThere = true;
			break;
		}
	}
	if (!alreadyThere)
	{
		chunksToRebuild.emplace_back(chunkIndex);
	}
}

bool FindBlock(std::vector<std::vector<Block>>& blocks
	, int x
	, int y
	, int& index)
{
	index = -1;
	for (int i = 0; i < blocks.at(x).size(); i++)
	{
		if (blocks.at(x).at(i).m_Y == y)
		{
			index = i;
			return true;
		}
		if (blocks.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return false;
}
bool FindBlock(std::vector<std::vector<Block>>& blocks
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < blocks.at(j).size(); i++)
		{
			if (blocks.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (blocks.at(j).at(i).m_Y <= vertices[1])
			{
				return true;
			}

		}
	}
	return false;
}
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int x
	, int y
	, int& index)
{
	index = -1;
	for (int i = 0; i < Walls.at(x).size(); i++)
	{
		if (Walls.at(x).at(i).m_Y == y)
		{
			index = i;
			return true;
		}
		if (Walls.at(x).at(i).m_Y < y)
		{
			break;
		}
	}
	return false;
}
bool FindWall(std::vector<std::vector<Wall>>& Walls
	, int* vertices)
{
	for (int j = vertices[0]; j <= vertices[2]; j++)
	{
		for (int i = 0; i < Walls.at(j).size(); i++)
		{
			if (Walls.at(j).at(i).m_Y < vertices[3])
			{
				break;
			}
			if (Walls.at(j).at(i).m_Y <= vertices[1])
			{
				return true;
			}

		}
	}
	return false;
}
int FindChunk(int x ,int y)
{
	return x / 20 + ((y-Blocks::yMin) / 20) * 54;
}
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks)
{
	std::vector<int> chunksToRebuild;

	for (int i = 0; i < 2322; i++)
	{
		chunksToRebuild.emplace_back(i);
		
	}
	CreateChunks(chunksToRebuild, chunks, blocks);
}
void CreateChunks(std::vector<ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls)
{
	std::vector<int> chunksToRebuild;
	for (int i = 0; i < 2322; i++)
	{
		chunksToRebuild.emplace_back(i);
	}
	CreateChunks(chunksToRebuild, chunks, walls);
}

void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Block>>& blocks)
{
	for (int l = 0;l < chunksToRebuild.size();l++)
	{
		int i = chunksToRebuild[l];
	
		bool map[t_BlocksSize - 1][20][20] = {};
		int transform[2] = { (i - (i / 54) * 54) * 20,((i / 54) * 20) + Blocks::yMin };
		for (int k = transform[0];k < transform[0] + 20;k++)
		{
			for (int j = 0; j < blocks.at(k).size();j++)
			{

				if (transform[1] > blocks.at(k).at(j).m_Y)
				{
					break;
				}
				else if (transform[1] + 20 > blocks.at(k).at(j).m_Y)
				{
					unsigned char blockType = blocks.at(k).at(j).m_Type;
					if (blockType < t_BlocksSize - 1 && blockType >= 0)
					{
						map[blockType][k - transform[0]][blocks.at(k).at(j).m_Y - transform[1]] = true;
					}
				}

			}
		}
	

		for (int j = 0; j < t_BlocksSize - 1;j++)
		{
			std::vector<float> vertices;
			std::vector<unsigned short> order;
			float blockVertices[4];
			int uvVertices[4];
			for (int k = 0; k < 20;k++)
			{
				for (int o = 0;o < 20; o++)
				{
					if (map[j][k][o])
					{
						blockVertices[0] = transform[0] - 0.5f + k;
						blockVertices[1] = transform[1] + 0.5f + o;
						blockVertices[2] = transform[0] + 0.5f + k;
						blockVertices[3] = transform[1] - 0.5f + o;
						uvVertices[0] = 0;
						uvVertices[1] = 1;
						uvVertices[2] = 1;
						uvVertices[3] = 0;
						o++;
						while (map[j][k][o] && o < 20)
						{
							uvVertices[1]++;
							blockVertices[1]++;
							o++;
						}
						int goToLeft = 1;
						bool canGo = true;
						for (int p = o - uvVertices[1]; p < o; p++)
						{
							canGo = canGo && map[j][k + goToLeft][p];
						}
						while (canGo && goToLeft + k < 20)
						{
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								map[j][k + goToLeft][p] = false;
							}
							uvVertices[2]++;
							blockVertices[2]++;
							goToLeft++;
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								canGo = canGo && map[j][k + goToLeft][p];
							}

						}

						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[1]);
						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[1]);
					}
				}
			}
		
			if (vertices.size() != 0)
			{
				order.resize((vertices.size() / 16) * 6);

				for (int k = 0; k < vertices.size() / 16; k++)
				{
					order.emplace_back(0 + k * 4);
					order.emplace_back(1 + k * 4);
					order.emplace_back(2 + k * 4);
					order.emplace_back(0 + k * 4);
					order.emplace_back(3 + k * 4);
					order.emplace_back(2 + k * 4);
				}
			}
			if (chunks.at(i).m_VBO[j])
			{
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glDeleteVertexArrays(1, &chunks.at(i).m_VA[j]));
			}
			chunks.at(i).m_VA[j] = 0;
			chunks.at(i).m_VBO[j] = 0;
			chunks.at(i).m_EOB[j] = 0;
			chunks.at(i).m_EOBNumber[j] = 0;
			if (vertices.size())
			{
				ErrorGL(glGenVertexArrays(1, &chunks.at(i).m_VA[j]));
				ErrorGL(glBindVertexArray(chunks.at(i).m_VA[j]));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunks.at(i).m_EOB[j]));
				ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(unsigned short), order.data(), GL_STATIC_DRAW));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, chunks.at(i).m_VBO[j]));
				ErrorGL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));

				ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
				ErrorGL(glEnableVertexAttribArray(0));
				ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
				ErrorGL(glEnableVertexAttribArray(1));


				ErrorGL(glBindVertexArray(0));
				chunks.at(i).m_EOBNumber[j] = order.size();
			}
		}
	}
	chunksToRebuild.clear();

}
void CreateChunks(std::vector<int>& chunksToRebuild
	, std::vector < ChunkDD>& chunks
	, std::vector<std::vector<Wall>>& walls)
{
	for (int l = 0;l < chunksToRebuild.size();l++)
	{
		int i = chunksToRebuild[l];
		bool map[t_BlocksSize - 1][20][20] = {};
		int transform[2] = { (i - (i / 54) * 54) * 20,((i / 54) * 20) + Blocks::yMin };
		for (int k = transform[0];k < transform[0] + 20;k++)
		{
			for (int j = 0; j < walls.at(k).size();j++)
			{

				if (transform[1] > walls.at(k).at(j).m_Y)
				{
					break;
				}
				else if (transform[1] + 20 > walls.at(k).at(j).m_Y)
				{
					unsigned char blockType = walls.at(k).at(j).m_Type;
					if (blockType < t_BlocksSize - 1 && blockType >= 0)
					{
						map[blockType][k - transform[0]][walls.at(k).at(j).m_Y - transform[1]] = true;
					}
				}

			}
		}
		for (int j = 0; j < t_BlocksSize - 1;j++)
		{
			std::vector<float> vertices;
			std::vector<unsigned short> order;
			float blockVertices[4];
			int uvVertices[4];
			for (int k = 0; k < 20;k++)
			{
				for (int o = 0;o < 20; o++)
				{
					if (map[j][k][o])
					{
						blockVertices[0] = transform[0] - 0.5f + k;
						blockVertices[1] = transform[1] + 0.5f + o;
						blockVertices[2] = transform[0] + 0.5f + k;
						blockVertices[3] = transform[1] - 0.5f + o;
						uvVertices[0] = 0;
						uvVertices[1] = 1;
						uvVertices[2] = 1;
						uvVertices[3] = 0;
						o++;
						while (map[j][k][o] && o < 20)
						{
							uvVertices[1]++;
							blockVertices[1]++;
							o++;
						}
						int goToLeft = 1;
						bool canGo = true;
						for (int p = o - uvVertices[1]; p < o; p++)
						{
							canGo = canGo && map[j][k + goToLeft][p];
						}
						while (canGo && goToLeft + k < 20)
						{
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								map[j][k + goToLeft][p] = false;
							}
							uvVertices[2]++;
							blockVertices[2]++;
							goToLeft++;
							for (int p = o - uvVertices[1]; p < o; p++)
							{
								canGo = canGo && map[j][k + goToLeft][p];
							}

						}

						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[3]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[3]);
						vertices.emplace_back(blockVertices[2]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[2]);
						vertices.emplace_back(uvVertices[1]);
						vertices.emplace_back(blockVertices[0]);
						vertices.emplace_back(blockVertices[1]);
						vertices.emplace_back(uvVertices[0]);
						vertices.emplace_back(uvVertices[1]);
					}
				}
			}
				
			if (vertices.size() != 0)
			{
				order.resize((vertices.size() / 16) * 6);

				for (int k = 0; k < vertices.size() / 16; k++)
				{
					order.emplace_back(0 + k * 4);
					order.emplace_back(1 + k * 4);
					order.emplace_back(2 + k * 4);
					order.emplace_back(0 + k * 4);
					order.emplace_back(3 + k * 4);
					order.emplace_back(2 + k * 4);
				}
			}
			if (chunks.at(i).m_VBO[j])
			{
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glDeleteBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glDeleteVertexArrays(1, &chunks.at(i).m_VA[j]));
			}
			chunks.at(i).m_VA[j] = 0;
			chunks.at(i).m_VBO[j] = 0;
			chunks.at(i).m_EOB[j] = 0;
			chunks.at(i).m_EOBNumber[j] = 0;
			if (vertices.size())
			{
				ErrorGL(glGenVertexArrays(1, &chunks.at(i).m_VA[j]));
				ErrorGL(glBindVertexArray(chunks.at(i).m_VA[j]));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_EOB[j]));
				ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunks.at(i).m_EOB[j]));
				ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, order.size() * sizeof(unsigned short), order.data(), GL_STATIC_DRAW));
				ErrorGL(glGenBuffers(1, &chunks.at(i).m_VBO[j]));
				ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, chunks.at(i).m_VBO[j]));
				ErrorGL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));

				ErrorGL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
				ErrorGL(glEnableVertexAttribArray(0));
				ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
				ErrorGL(glEnableVertexAttribArray(1));


				ErrorGL(glBindVertexArray(0));
				chunks.at(i).m_EOBNumber[j] = order.size();
			}
		}
	}
	chunksToRebuild.clear();

}
void DrawChunks(Shader& shadowSh
	, unsigned int* textures
	, float* trasform
	, float* cameraTransform
	, std::vector<ChunkDD>& chunksBlock
	, std::vector<ChunkDD>& chunksWall)
{
	if (Window::height)
	{
		shadowSh.Bind();
		ChangeTransform(0, 0, trasform);
		shadowSh.SetUniformMat4(basicTransform, trasform);
		shadowSh.SetUniform1i(basicSize + ShadowLocation, 1);
		float cameraVertices[4] = { floorf(cameraTransform[0] - Window::halfWidthOfGameTransform)
			, ceilf(cameraTransform[1] + Window::halfHeightOfGameTransform)
			, ceilf(cameraTransform[0] + Window::halfWidthOfGameTransform)
			, floorf(cameraTransform[1] - Window::halfHeightOfGameTransform) };
		float edgeChunks[4] = { FindChunk(cameraVertices[0] ,cameraVertices[3])
			, FindChunk(cameraVertices[2], cameraVertices[3])
			, FindChunk(cameraVertices[2], cameraVertices[1])
			, FindChunk(cameraVertices[0], cameraVertices[1]) };
		float deltaChunks[2] = { edgeChunks[1] - edgeChunks[0] ,(edgeChunks[3] - edgeChunks[0]) / 54 };

		for (int i = 0; i <= deltaChunks[1];i++)
		{
			for (int j = 0; j <= deltaChunks[0]; j++)
			{
				int chunkIndex = edgeChunks[0] + j + i * 54;

				for (int k = 0; k < t_BlocksSize - 1;k++)
				{
					if (chunksWall[chunkIndex].m_VA[k])
					{
						ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[k]));
						ErrorGL(glBindVertexArray(chunksWall[chunkIndex].m_VA[k]));
						ErrorGL(glDrawElements(GL_TRIANGLES, chunksWall[chunkIndex].m_EOBNumber[k], GL_UNSIGNED_SHORT, 0));
					}
				}
			}
		}
		shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);

		for (int i = 0; i <= deltaChunks[1];i++)
		{
			for (int j = 0; j <= deltaChunks[0]; j++)
			{
				int chunkIndex = edgeChunks[0] + j + i * 54;

				for (int k = 0; k < t_BlocksSize - 1;k++)
				{
					if (chunksBlock[chunkIndex].m_VA[k])
					{
						ErrorGL(glBindTexture(GL_TEXTURE_2D, textures[k]));
						ErrorGL(glBindVertexArray(chunksBlock[chunkIndex].m_VA[k]));
						ErrorGL(glDrawElements(GL_TRIANGLES, chunksBlock[chunkIndex].m_EOBNumber[k], GL_UNSIGNED_SHORT, 0));
					}
				}
			}
		}
	}

}