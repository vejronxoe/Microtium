#include"XoeEngine.h"



int main()
{
	if (!(SetupEngine(640, 480, "TerrariaClone")))
		return -1;
	bool GameRunning = true;
	squere player(0, 0, 1, 1, "a", "res/textures/Fire.png", 0, "a", "res/shader/basic.txt");
	while (GameRunning)
	{
		ClearScrean();
		player.rendererObject();

	}
	EndProgram();
}