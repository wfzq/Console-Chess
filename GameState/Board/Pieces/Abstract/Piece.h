#pragma once
#include "Coords.h"
#include "PieceAttributes.h"

class Board;
class Piece
{
public:
	Piece(Color color);
	virtual ~Piece() = default;
	Piece() = delete;

	Color getColor() const;

	virtual Type getType() const = 0;
	virtual bool isValidMove(const Coords& move, const Board* board) const = 0;

	bool isEndSquareValid(const Coords& c, const Board* board) const;
	static int getColorDirection(const Color pieceColor);
	static int getMoveDirection(int start, int exit);
	static bool areIntermediateXSquaresEmpty(const Coords& c, const Board* board);
	static bool areIntermediateYSquaresEmpty(const Coords& c, const Board* board);
	static bool areIntermediateDiagonalSquaresEmpty(const Coords& c, const Board* board);
	static bool isStartExitOnSameDiagonal(const Coords& c);

protected:
	Color pieceColor;
};