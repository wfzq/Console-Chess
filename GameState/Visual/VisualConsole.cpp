#include "../Board/Board.h"
#include <iostream>

namespace visual
{
	static void draw(const Board* board);

	namespace
	{
		void drawLine()
		{
			std::cout << "     |-------------------------------|\n";
		}
		void drawFooter()
		{
			std::cout << "       a   b   c   d   e   f   g   h\n";
		}
	}

	static void draw(const Board* board)
	{
		/* Draw TOP to BOTTOM, LEFT to RIGHT */

		drawLine();

		for (int y = 7; y >= 0; --y)
		{
			// Draw column number
			std::cout << "  " << (y + 1) << "  |";

			for (int x = 0; x < 8; x++)
			{
				// Default char
				char result = ' ';

				// Get piece, if any
				Piece* current = board->getPiece(x, y);
				if (current != nullptr)
				{
					result = (char)current->getType();

					if (current->getColor() == Color::BLACK) {
						result = tolower(result);
					}
				}

				// Draw char
				std::cout << " " << result << " |";
			}
			std::cout << "\n";
			drawLine();
		}
		drawFooter();
		std::cout << "\n";
	}
}