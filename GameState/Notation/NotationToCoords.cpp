#include <string>
#include <algorithm>
#include <stdexcept>
#include "../Board/Board.h"

namespace notation
{
	static Coords toCoords(const Board* board, std::string notation);

	// Private
	namespace
	{
		bool isValidPromotionChar(char c) {
			return c == 'Q' || c == 'R' || c == 'B' || c == 'N';
		}

		bool isValidPieceChar(char c) {
			return c == 'Q' || c == 'R' || c == 'B' || c == 'N' || c == 'K';
		}
	}

	static Coords toCoords(const Board* board, std::string _notation)
	{
		std::string notation = _notation;

		// Check if string length is valid
		if (notation.length() < 2 || notation.length() > 8) {
			throw std::invalid_argument("Invalid notation string: " + _notation);
		}

		std::string notationUpper = notation;
		std::transform(notationUpper.begin(), notationUpper.end(), notationUpper.begin(), ::toupper);
		Color playerTurnColor = board->getTurnColor();

		// Castling
		if (notationUpper == "O-O") {
			int king = playerTurnColor == Color::BLACK ? 7 : 0;
			return Coords(4, king, 6, king);
		}
		else if (notationUpper == "O-O-O") {
			int king = playerTurnColor == Color::BLACK ? 7 : 0;
			return Coords(4, king, 2, king);
		}

		Coords c;
		char y = notation[notation.length() - 2];
		char z = notation[notation.length() - 1];
		int direction = Piece::getColorDirection(playerTurnColor);

		// Check(mate)
		if (z == '+' || z == '#')
		{
			// Remove last char, readjust string
			notation.pop_back();
			y = notation[notation.length() - 2];
			z = notation[notation.length() - 1];
		}

		// Promotion
		if (y == '=' && isValidPromotionChar(z) && notation.length() >= 4)
		{
			c.promotion = z;

			// Remove last 2 chars, readjust string
			notation.erase(notation.size() - 2);
			y = notation[notation.length() - 2];
			z = notation[notation.length() - 1];
		}

		// Check if endX and endY are valid
		y = std::toupper(y);
		if (y < 'A' || y > 'H' || z < '1' || z > '8') {
			throw std::invalid_argument("Invalid notation string: " + _notation);
		}

		// Assign destination
		int endX = y - 'A';
		int endY = z - '1';

		// If notation is 2 characters long, it's a pawn forward move
		if (notation.length() == 2)
		{
			int startX = endX;
			int startY = endY - direction;

			if (board->getPiece(startX, startY)->getType() == Type::PAWN) {
				return Coords(startX, startY, endX, endY);
			}
			else return Coords(startX, startY - direction, endX, endY);
		}

		// Remove endX/Y chars, readjust string
		notation.erase(notation.size() - 2);
		y = notation[notation.length() - 2];
		z = notation[notation.length() - 1];

		// Check for captures
		if (std::toupper(z) == 'X')
		{
			if (notation.length() == 1) {
				throw std::invalid_argument("Invalid notation string: " + _notation);
			}

			// Remove char, readjust string
			notation.pop_back();
			y = notation[notation.length() - 2];
			z = notation[notation.length() - 1];
		}

		int startX = -1, startY = -1;
		Type piece = Type::PAWN;

		// Check what to move
		if (isValidPieceChar(notation[0]))
		{
			piece = static_cast<Type>(notation[0]);
			notation.erase(0, 1);
		}

		// startX and Y available
		if (notation.length() == 2)
		{
			y = notation[notation.length() - 2];
			z = notation[notation.length() - 1];

			y = std::toupper(y);
			if (y < 'A' || y > 'H' || z < '1' || z > '8') {
				throw std::invalid_argument("Invalid notation string: " + _notation);
			}

			startX = y - 'A';
			startY = z - '1';

			if (board->getPiece(startX, startY)->getType() == piece) {
				return Coords(startX, startY, endX, endY);
			}
			else throw std::invalid_argument("Specified piece not found");
		}
		// One or both need board context
		else if (notation.length() <= 1)
		{
			if (notation.length() == 1)
			{
				z = std::toupper(notation[0]);

				if (z < '1' || z > '8') {
					startY = z;
				}

				if (y < 'A' || y > 'H') {
					startX = z;
				}
			}

			// Inefficient, check all squares
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					startX = startX == -1 ? i : startX;
					startY = startY == -1 ? j : startY;

					if (board->getPiece(startX, startY) != nullptr &&
						board->getPiece(startX, startY)->getType() == piece &&
						board->getPiece(startX, startY)->getColor() == playerTurnColor)
					{
						return Coords(startX, startY, endX, endY);
					}
				}
			}
		}
		// This shouldn't be reached
		throw std::invalid_argument("Invalid notation string: " + _notation);
	}
}