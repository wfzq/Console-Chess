#include "abstract/Piece.h"

class Queen : public Piece
{
public:
	Queen(Color color) : Piece(color) {};
	Queen() = delete;
	~Queen() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};