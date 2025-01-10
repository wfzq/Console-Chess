#include "King.h"
#include "../Board.h"

Type King::getType() const
{
	return Type::KING;
}

bool King::isValidMove(const Coords& c, const Board* board) const
{
	if (abs(c.startX - c.endX) <= 1 && abs(c.startY - c.endY) <= 1)
	{
		return isEndSquareValid(c, board);
	}
	return false;

	// TODO: Implement castling
}