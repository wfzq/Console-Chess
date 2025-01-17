#include "abstract/Piece.h"

class Queen : public Piece
{
public:
	Queen(Color color) : Piece(color) {};
	Queen() = delete;
	~Queen() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& c, const Board* board, int special = 0) const override;
};