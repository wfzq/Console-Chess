#pragma once
#include "Pieces/Abstract/Piece.h"
#include <array>
#include <memory>

class Board
{
public:
	Board(); // Default New Game
	Board(const Board& b);
	~Board() = default;

	int move(const Coords& c);

	Piece* getPiece(int x, int y) const;
	void setTurnColor(Color color);

private:
	void incrementTurn();
	void movePiece(const Coords& c);
	bool isMoveLegal(const Coords& c) const;
	bool isKingInCheck(Color playerColor) const;
	bool playerHasLegalMoves(Color playerColor) const;
	int isGameOver(Color playerColor) const;
	Coords findKing(Color playerColor) const;

	int fiftyMoveCounter = 0;
	int threeFoldCounter = 0;
	int turnCounter = 1;
	Color playerTurn = Color::WHITE;
	std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;
};