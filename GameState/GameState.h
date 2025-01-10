#include "Board/Board.h"

class GameState
{
public:
	GameState() = default;
	~GameState() = default;

	void start();
	void newGame();
	void getInput(); 

private:
	std::unique_ptr<Board> b;
};