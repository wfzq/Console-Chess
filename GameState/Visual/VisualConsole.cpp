#include "../Board/Board.h"
#include <iostream>

namespace visual
{
	static void draw(const Board* board, int wColor = 0, int bColor = 0);

	namespace
	{
		void colorChar(char c, int color = 0)
		{
			std::cout << "\033[" << color << "m" << c << "\033[0m";
		}
		void drawLine()
		{
			std::cout << "     |-------------------------------|\n";
		}

		void drawWhite(const Board* board, int wColor, int bColor)
		{
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
							// Draw black char
							std::cout << " ";
							colorChar(result, bColor);
							std::cout << " |";
						}
						else {
							// Draw white char
							std::cout << " ";
							colorChar(result, wColor);
							std::cout << " |";
						}
					}
					// Draw Space
					else {
						std::cout << "  " << " |";
					}
				}
				std::cout << "\n";
				drawLine();
			}
			std::cout << "       a   b   c   d   e   f   g   h\n";
			std::cout << "\n";
		}

		void drawBlack(const Board* board, int wColor, int bColor) {
			drawLine();

			for (int y = 0; y < 8; ++y)
			{
				// Draw column number
				std::cout << "  " << (y + 1) << "  |";

				for (int x = 7; x >= 0; --x)
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
							// Draw black char
							std::cout << " ";
							colorChar(result, bColor);
							std::cout << " |";
						}
						else {
							// Draw white char
							std::cout << " ";
							colorChar(result, wColor);
							std::cout << " |";
						}
					}
					// Draw Space
					else {
						std::cout << "  " << " |";
					}
				}
				std::cout << "\n";
				drawLine();
			}
			std::cout << "       h   g   f   e   d   c   b   a\n";
			std::cout << "\n";
		}
	}

	static void draw(const Board* board, int wColor, int bColor)
	{
		/* Draw TOP to BOTTOM, LEFT to RIGHT */

		if (board->getTurnColor() == Color::WHITE)
			drawWhite(board, wColor, bColor);
		else
			drawBlack(board, wColor, bColor);
	}
}