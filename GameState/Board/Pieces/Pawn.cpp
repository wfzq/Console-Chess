#include "Pawn.h"
#include "../Board.h"

Type Pawn::getType() const
{
	return Type::PAWN;
}

bool Pawn::isValidMove(const Coords& c, const Board* board) const
{
	int direction = getColorDirection(pieceColor);

	// Move forward
	if (c.startX == c.endX)
	{
		// Move one square
		if (c.endY == c.startY + direction)
		{
			if (board->getPiece(c.endX, c.endY) == nullptr)
			{
				return true;
			}
		}
		// Move two squares
		else if (c.endY == c.startY + 2 * direction && c.startY == (pieceColor == Color::WHITE ? 1 : 6))
		{
			if (board->getPiece(c.endX, c.endY /*       */) == nullptr &&
				board->getPiece(c.endX, c.endY - direction) == nullptr)
			{
				return true;
			}
		}
	}
	// Capture
	else if (abs(c.startX - c.endX) == 1 && c.endY == c.startY + direction)
	{
		if (board->getPiece(c.endX, c.endY) != nullptr)
		{
			return pieceColor != board->getPiece(c.endX, c.endY)->getColor();
		}
		// En passant
		else
		{
			if (c.startY == 3.5 + 0.5 * direction) {
				if (board->getPiece(c.endX, c.endY - direction) != nullptr && 
					board->getPiece(c.endX, c.endY - direction)->getType() == Type::PAWN && 
					board->getPiece(c.endX, c.endY - direction)->getColor() != pieceColor && 
					board->getEnPassantX() == c.endX) {
					/* The flag, valid for a turn, is set by the Board object */
					return true;
				}
			}
		}
	}
	return false;
}