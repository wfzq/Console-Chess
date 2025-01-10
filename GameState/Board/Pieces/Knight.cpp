#include "Knight.h"
#include "../Board.h"

Type Knight::getType() const
{
	return Type::KNIGHT;
}

bool Knight::isValidMove(const Coords& c, const Board* board) const
{
	if (isEndSquareValid(c, board))
	{
		if (abs(c.startX - c.endX) == 2 && abs(c.startY - c.endY) == 1)
		{
			return true;
		}
		else if (abs(c.startX - c.endX) == 1 && abs(c.startY - c.endY) == 2)
		{
			return true;
		}
	}
	return false;
}