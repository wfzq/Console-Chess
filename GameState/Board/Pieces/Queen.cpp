#include "Queen.h"
#include "../Board.h"

Type Queen::getType() const
{
	return Type::QUEEN;
}

bool Queen::isValidMove(const Coords& c, const Board* board, int &special) const
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
		else if (isStartExitOnSameDiagonal(c))
		{
			return areIntermediateDiagonalSquaresEmpty(c, board);
		}
	}
	return false;
}