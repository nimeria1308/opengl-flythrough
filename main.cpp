#include <iostream>

#include "Game.h"

using namespace std;

int main(int argc, char* args[])
{
	Game game(1280, 720);
	game.loop();

	return 0;
}
