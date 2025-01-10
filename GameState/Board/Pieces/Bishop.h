#include "abstract/Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Color color) : Piece(color) {};
	Bishop() = delete;
	~Bishop() = default;
	
	Type getType() const override;
	bool isValidMove(const Coords& move, const Board* board) const override;
};