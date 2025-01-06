#include "Board.h"
#include "Pieces/Abstract/Piece.h"

Board::Board()
{
}

Board::~Board()
{
}

Piece* Board::getPiece(int x, int y) const
{
	return board[x][y].get();
}