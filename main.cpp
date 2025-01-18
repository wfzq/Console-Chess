#include <iostream>
#include "GameState/ConsoleChess.h"
#include "GameState/Notation/NotationToCoords.cpp"
#include "GameState/Visual/VisualConsole.cpp"

int main()
{
	ConsoleChess game;
	game.setPieceColor(0, 32);
	game.start();
}