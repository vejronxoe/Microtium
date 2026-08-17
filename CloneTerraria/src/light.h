#include"BlocksAndWalls.h"
#include"EnemyAndProjectile.h"

void CalculateLightMap(int chunkNumber
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap);
void CalculateLightMap(std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& walls
	, std::vector<std::vector<float>>& StaticLightMap);
void CreateLightMap(std::vector<std::vector<float>>& StaticLightMap
	, std::vector<std::vector<Block>>& blocks
	, std::vector<Projectile>& projectiles
	, float* playerTransform
	, unsigned int playerHand 
	, int x
	, int y
	, int width
	, int height
	, unsigned int &iD);