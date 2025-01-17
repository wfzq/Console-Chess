#include "Rook.h"
#include "../Board.h"

Type Rook::getType() const
{
	return Type::ROOK;
}

bool Rook::isValidMove(const Coords& c, const Board* board, int special) const
{
	if (isEndSquareValid(c, board))
	{
		if (c.startY == c.endY)
		{
			if (areIntermediateXSquaresEmpty(c, board))
			{
				special = 1; // has moved
				return true;
			}
		}
		else if (c.startX == c.endX)
		{
			if (areIntermediateYSquaresEmpty(c, board))
			{
				special = 1; // has moved
				return true;
			}
		}
		return false;
	}
	return false;
}
