#include <iostream>
#include "Gamestate/Board/Board.h"
#include "GameState/Notation/NotationToCoords.cpp"
#include "GameState/Visual/VisualConsole.cpp"

int main()
{
	Board ng;
	visual::draw(&ng);

	while (true)
	{
		Coords c;
		std::string s;
		std::cin >> s;

		try {
			c = notation::toCoords(&ng, s);

			std::cout << "sX = " << c.startX << "\n";
			std::cout << "sY = " << c.startY << "\n";
			std::cout << "eX = " << c.endX << "\n";
			std::cout << "eY = " << c.endY << "\n";
			std::cout << "\n~~~~~~~~~~~~~~~~~~~~~\n\n";
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n\n";
		}
	}
}