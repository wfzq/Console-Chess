#pragma once
#include "Pieces/Abstract/Piece.h"
#include <array>
#include <memory>
#include <unordered_map>

// FIDE ruleset
class Board
{
public:
	enum class specialCodes : int
	{
		MOVE = 1,
		CAPTURE = 2,
		CASTLE = 3,
		PROMOTE = 4
	};

	Board() = default;
	Board(const Board& b);
	~Board() = default;

	// Play
	void newGame();
	int move(const Coords& c);

	// Manage
	void setTurnColor(Color color);
	void place(int startX, int startY, Type piece, Color color);
	Color getTurnColor() const;
	Piece* getPiece(int x, int y) const;
	Piece* getEnPassantPiece() const;
	int isGameOver(Color playerColor) const;
private:
	void movePiece(const Coords& c);
	void incrementTurn();
	void fiftyMoveCount(const Coords& c);
	void threeFoldCount(const Coords& c);
	void setFlags(const Coords& c);
	bool isMoveLegal(const Coords& c, int &special) const;
	bool isKingInCheck(Color playerColor) const;
	bool playerHasLegalMoves(Color playerColor) const;
	Coords findKing(Color playerColor) const;

	int fiftyMoveCounter = 0;
	int threeFoldCounter = 0;
	int turnCounter = 1;
	int special = 0;
	Piece* enPassantPiece = nullptr;
	Color playerTurn = Color::WHITE;
	std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;
};