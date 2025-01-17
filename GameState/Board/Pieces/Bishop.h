#include "abstract/Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Color color) : Piece(color) {};
	Bishop() = delete;
	~Bishop() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& c, const Board* board, int special = 0) const override;
};