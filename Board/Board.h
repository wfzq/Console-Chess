#pragma once
#include "Pieces/Abstract/Piece.h"
#include <array>
#include <memory>

class Board
{
public:
	Board();
	~Board();

	Piece* getPiece(int x, int y) const;

private:
	std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;
};