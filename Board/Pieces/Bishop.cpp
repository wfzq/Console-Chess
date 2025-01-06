#include "Bishop.h"
#include "Board.h"

Type Bishop::getType() const
{
	return Type::BISHOP;
}

bool Bishop::isValidMove(const Coords& c, const Board* board) const
{
	if (isStartExitOnSameDiagonal(c))
	{
		return areIntermediateDiagonalSquaresEmpty(c, board) && isEndSquareValid(c, board);
	}
	return false;
}