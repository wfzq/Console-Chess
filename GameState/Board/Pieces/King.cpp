#include "King.h"
#include "../Board.h"

Type King::getType() const
{
	return Type::KING;
}

bool King::isValidMove(const Coords& c, const Board* board, int special) const
{
	bool isWhite = this->getColor() == Color::WHITE;
	if (abs(c.startX - c.endX) <= 1 && abs(c.startY - c.endY) <= 1)
	{
		special = 1; // has moved
		return isEndSquareValid(c, board);
	}
	// Castle
	else if (abs(c.startX - c.endX) == 2 && c.endY == (isWhite ? 0 : 7))
	{
		Coords kingToRook = c;
		int rookX = (c.startX > c.endX ? 0 : 7);
		kingToRook.endX = rookX;

		if (!this->getIsMoved() && 
			!board->getPiece(rookX, c.endY)->getIsMoved() &&
			areIntermediateXSquaresEmpty(kingToRook, board)) 
		{
			special = 3;
			return true;
		}
	}
	return false;
}