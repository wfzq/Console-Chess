#include "Board/Board.h"
#include <iostream>
#include <string>

class ConsoleChess
{
public:
	ConsoleChess();
	~ConsoleChess() = default;

	void start();
	int makeMove(std::string notation);
	void setPieceColor(int wColor, int bColor);
private:
	int whiteColor = 0;
	int blackColor = 0;
	bool gameOver = false;
	int outcome = 0;
	Board game;
};