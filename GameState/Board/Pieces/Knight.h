#include "Abstract/Piece.h"

class Knight : public Piece
{
public:
	Knight(Color color) : Piece(color) {};
	Knight() = delete;
	~Knight() = default;

	Type getType() const override;
	bool isValidMove(const Coords& c, const Board* board, int &special) const override;
};