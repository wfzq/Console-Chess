#include "Abstract/Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Color color) : Piece(color) {};
	Pawn() = delete;
	~Pawn() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};