#include "Rook.h"
#include "../Board.h"

Type Rook::getType() const
{
	return Type::ROOK;
}

bool Rook::isValidMove(const Coords& c, const Board* board) const
{
	if (isEndSquareValid(c, board))
	{
		if (c.startY == c.endY)
		{
			return areIntermediateXSquaresEmpty(c, board);
		}
		else if (c.startX == c.endX)
		{
			return areIntermediateYSquaresEmpty(c, board);
		}
	}
	return false;
}
