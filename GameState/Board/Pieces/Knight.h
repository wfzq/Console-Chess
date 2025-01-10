#include "Abstract/Piece.h"

class Knight : public Piece
{
public:
	Knight(Color color) : Piece(color) {};
	Knight() = delete;
	~Knight() = default;

	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};