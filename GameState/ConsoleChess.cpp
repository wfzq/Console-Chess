#include "ConsoleChess.h"
#include "Notation/NotationToCoords.cpp"
#include "Visual/VisualConsole.cpp"

ConsoleChess::ConsoleChess()
{
	game.newGame();
}

void ConsoleChess::start()
{
	visual::draw(&game, whiteColor, blackColor);

	if (gameOver) {
		if (outcome == 1) {
			std::cout << "White Wins!\n";
		}
		else if (outcome == 2) {
			std::cout << "Black Wins!\n";
		}
		else if (outcome > 2) {
			std::cout << "Stalemate!\n";
		}
		return;
	}

	std::string input;
	int moveResult;
	while (true)
	{
		std::cout << ">> ";
		std::getline(std::cin, input);

		try
		{
			Coords gameMove = notation::toCoords(&game, input);
			moveResult = game.move(gameMove);

			if (moveResult < 0)
				throw std::invalid_argument("");
			else if (moveResult == 1) {
				visual::draw(&game, whiteColor, blackColor);
				std::cout << "White Wins!\n";
				break;
			}
			else if (moveResult == 2) {
				visual::draw(&game, whiteColor, blackColor);
				std::cout << "Black Wins!\n";
				break;
			}
			else if (moveResult > 2) {
				visual::draw(&game, whiteColor, blackColor);
				std::cout << "Stalemate!\n";
				break;
			}
		}
		catch (const std::exception&)
		{
			std::cout << "Invalid move!\n";
		}

		visual::draw(&game, whiteColor, blackColor);
	}
}

int ConsoleChess::makeMove(std::string notation)
{
	if (gameOver) return -1;

	try
	{
		Coords gameMove = notation::toCoords(&game, notation);
		int result = game.move(gameMove);

		if (result == 1) {
			gameOver = true;
			outcome = 1; // White wins
		}
		else if (result == 2) {
			gameOver = true;
			outcome = 2; // Black wins
		}
		else if (result > 2) {
			gameOver = true;
			outcome = 3; // Stalemate
		}

		return result;
	}
	catch (const std::exception&) {
		return -1;
	}
}

void ConsoleChess::setPieceColor(int wColor, int bColor)
{
	whiteColor = wColor;
	blackColor = bColor;
}
