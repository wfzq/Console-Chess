#include "Abstract/Piece.h"

class King : public Piece
{
public:
	King(Color color) : Piece(color) {};
	King() = delete;
	~King() = default;

	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};