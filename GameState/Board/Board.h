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

	// Play
	int move(const Coords& c);

	// Manage
	void setTurnColor(Color color);
	void place(int startX, int startY, Type piece, Color color);
	Color getTurnColor() const;
	Piece* getPiece(int x, int y) const;
	int getEnPassantX() const;
private:
	void movePiece(const Coords& c);
	void incrementTurn();

	bool isMoveLegal(const Coords& c) const;
	bool isKingInCheck(Color playerColor) const;
	bool playerHasLegalMoves(Color playerColor) const;
	int isGameOver(Color playerColor) const;
	Coords findKing(Color playerColor) const;

	int fiftyMoveCounter = 0;
	int threeFoldCounter = 0;
	int turnCounter = 1;
	int enPassantX = -1; // Flag represents the x position
	Color playerTurn = Color::WHITE;
	std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;
};