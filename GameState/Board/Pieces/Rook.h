#include "Abstract/Piece.h"

class Rook : public Piece
{
public:
	Rook(Color color) : Piece(color) {};
	Rook() = delete;
	~Rook() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};