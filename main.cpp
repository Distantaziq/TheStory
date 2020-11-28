#include"CGameData.h"
#include"CGameState.h"

int main()
{
	COORD newSize = { 100,999 };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newSize);

	CGameState MainGame;
	//If there are save games, ask if we want to continue from save
	// {...}

	//If there are no save games, let's start from the beginning
	MainGame.RunGame();

}