#include <string>
#include <algorithm>
#include <stdexcept>
#include "../Board/Board.h"

namespace notation
{
	static Coords toCoords(const Board* board, std::string _notation);

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
		// Check if string length is valid
		if (_notation.length() < 2 || _notation.length() > 8)
			throw std::invalid_argument("Invalid notation string: " + _notation);

		std::string notation = _notation;
		char y = notation[notation.length() - 2];
		char z = notation[notation.length() - 1];

		// Check(mate)
		if (z == '+' || z == '#')
		{
			// Remove last char
			notation.pop_back();

			if (notation.length() < 2)
				throw std::invalid_argument("Invalid notation string: " + _notation);

			// Reasign chars
			y = notation[notation.length() - 2];
			z = notation[notation.length() - 1];
		}

		Color playerTurnColor = board->getTurnColor();

		{
			// Castling
			std::string notationUpper = notation;
			std::transform(notationUpper.begin(), notationUpper.end(), notationUpper.begin(), ::toupper);
			int king = playerTurnColor == Color::BLACK ? 7 : 0;

			if (notationUpper == "O-O")
				return Coords(4, king, 6, king);

			else if (notationUpper == "O-O-O")
				return Coords(4, king, 2, king);
		}

		// Promotion
		Coords c;
		if (y == '=' && isValidPromotionChar(z) && notation.length() >= 4)
		{
			c.promotion = z;

			// Erase last 2 chars, reasign
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
		int direction = Piece::getColorDirection(playerTurnColor);

		// If notation is 2 characters long, it's a pawn forward move
		if (notation.length() == 2)
		{
			int startX = endX;
			int startY = endY - direction;

			if (direction == 1 && startY < 1 || direction == -1 && startY > 6)
				throw std::invalid_argument("Invalid notation string: " + _notation);

			// Check for pawn 1 square behind
			Piece* pawn = board->getPiece(startX, startY);
			if (pawn != nullptr && pawn->getType() == Type::PAWN)
			{
				return Coords(startX, startY, endX, endY);
			}

			// Check for pawn 2 squares behind
			startY -= direction;
			pawn = board->getPiece(startX, startY);
			if (pawn != nullptr && pawn->getType() == Type::PAWN)
			{
				return Coords(startX, startY, endX, endY);
			}

			// Throw
			else throw std::invalid_argument("Specified piece not found");
		}

		// Cut string, readjust
		notation.erase(notation.size() - 2);
		z = notation[notation.length() - 1];

		// Check if capture
		if (std::toupper(z) == 'X')
		{
			if (notation.length() == 1)
				throw std::invalid_argument("Invalid notation string: " + _notation);

			// Remove char
			notation.pop_back();
		}

		int startX = -1, startY = -1;
		Type piece = Type::PAWN;

		// Check what to move
		if (isValidPieceChar(std::toupper(notation[0])))
		{
			// if 'b' determine if coord or bishop
			if (notation[0] == 'b' && 
				board->getPiece(1, endY - direction) != nullptr && 
				board->getPiece(1, endY - direction)->getType() == Type::PAWN) 
			{
				return Coords(1, endY - direction, endX, endY, c.promotion);
			}
			else 
			{
				piece = static_cast<Type>(std::toupper(notation[0]));
				notation.erase(0, 1);
			}
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

			Piece* p = board->getPiece(startX, startY);
			if (p != nullptr && p->getType() == piece) {
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

				if (z >= '1' && z <= '8') {
					startY = z - '1';
				}

				if (z >= 'A' && z <= 'H') {
					startX = z - 'A';
				}
			}

			// Inefficient, check all squares
			int dummy;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					int currentX = (startX == -1) ? i : startX;
					int currentY = (startY == -1) ? j : startY;

					if (board->getPiece(currentX, currentY) != nullptr &&
						board->getPiece(currentX, currentY)->getType() == piece &&
						board->getPiece(currentX, currentY)->getColor() == playerTurnColor && 
						board->getPiece(currentX, currentY)->isValidMove({currentX,currentY,endX,endY}, board, dummy)
)
					{
						return Coords(currentX, currentY, endX, endY);
					}
				}
			}
		}
		// This shouldn't be reached
		throw std::invalid_argument("no move found");
	}
}