#include "Board.h"
#include "Pieces/Abstract/Piece.h"
#include <stdexcept>
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Knight.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"

Board::Board()
{
	place(0, 0, Type::ROOK, Color::WHITE);
	place(1, 0, Type::KNIGHT, Color::WHITE);
	place(2, 0, Type::BISHOP, Color::WHITE);
	place(3, 0, Type::QUEEN, Color::WHITE);
	place(4, 0, Type::KING, Color::WHITE);
	place(5, 0, Type::BISHOP, Color::WHITE);
	place(6, 0, Type::KNIGHT, Color::WHITE);
	place(7, 0, Type::ROOK, Color::WHITE);
	for (int i = 0; i < 8; i++)
	{
		place(i, 1, Type::PAWN, Color::WHITE);
	}

	place(0, 7, Type::ROOK, Color::BLACK);
	place(1, 7, Type::KNIGHT, Color::BLACK);
	place(2, 7, Type::BISHOP, Color::BLACK);
	place(3, 7, Type::QUEEN, Color::BLACK);
	place(4, 7, Type::KING, Color::BLACK);
	place(5, 7, Type::BISHOP, Color::BLACK);
	place(6, 7, Type::KNIGHT, Color::BLACK);
	place(7, 7, Type::ROOK, Color::BLACK);
	for (int i = 0; i < 8; i++)
	{
		place(i, 6, Type::PAWN, Color::BLACK);
	}
}

Board::Board(const Board& b)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Shallow copy
			board[i][j] = b.board[i][j];
		}
	}
	playerTurn = b.playerTurn;
	fiftyMoveCounter = b.fiftyMoveCounter;
	threeFoldCounter = b.threeFoldCounter;
	turnCounter = b.turnCounter;
}

/*
	Return values:
	-2 - No king
	-1 - Illegal move
	 0 - Successful move
	 1 - Checkmate by White
	 2 - Checkmate by Black
	 3 - Stalemate (Draw)
	 4 - Draw by fifty-move rule
	 5 - Draw by threefold repetition
	 6 - Draw by insufficient material
 */
int Board::move(const Coords& c)
{
	if (isMoveLegal(c))
	{
		setFlags(c);
		movePiece(c);
		incrementTurn();

		return isGameOver(playerTurn);
	}
	// Invalid move
	else return -1;
}

Color Board::getTurnColor() const
{
	return playerTurn;
}

Piece* Board::getPiece(int x, int y) const
{
	return board[x][y].get();
}

Piece* Board::getEnPassantPiece() const
{
	return enPassantPiece;
}

void Board::setTurnColor(Color color)
{
	playerTurn = color;
}

void Board::place(int x, int y, Type piece, Color color)
{
	switch (piece)
	{
	case Type::PAWN:
		this->board[x][y] = std::make_unique<Pawn>(color);
		break;
	case Type::ROOK:
		this->board[x][y] = std::make_unique<Rook>(color);
		break;
	case Type::KNIGHT:
		this->board[x][y] = std::make_unique<Knight>(color);
		break;
	case Type::BISHOP:
		this->board[x][y] = std::make_unique<Bishop>(color);
		break;
	case Type::QUEEN:
		this->board[x][y] = std::make_unique<Queen>(color);
		break;
	case Type::KING:
		this->board[x][y] = std::make_unique<King>(color);
		break;
	default:
		throw std::invalid_argument("Invalid piece type");
	}
}

void Board::incrementTurn()
{
	playerTurn = (playerTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
	turnCounter++;
}

void Board::fiftyMoveCount(const Coords& c)
{
	Piece* start = getPiece(c.startX, c.startY);
	Piece* exit = getPiece(c.endX, c.endY);

	if (start->getType() == Type::PAWN ||
		exit != nullptr)
	{
		fiftyMoveCounter = 0;
	}
	else fiftyMoveCounter++;
}

void Board::threeFoldCount(const Coords& c)
{
	// Piece* start = getPiece(c.startX, c.startY);
	// Piece* exit = getPiece(c.endX, c.endY);
	// 
	// if (fiftyMoveCounter == 0)
	// {
	// 	threeFoldCounter = 0;
	// 	// threeFoldArray.clear();
	// }
	// // Start of turn
	// if (playerTurn == Color::WHITE)
	// {
	// }
}

void Board::movePiece(const Coords& c)
{
	this->board[c.endX][c.endY] = std::move(this->board[c.startX][c.startY]);
}

void Board::setFlags(const Coords& c)
{
	// Expire enemy en passant flags
	enPassantPiece->setIsMoved(false);
	enPassantPiece = nullptr;

	// Set own flags
	switch (special) {
	case (int)specialCodes::MOVE:
	{
		// Set en passant flag
		if (getPiece(c.startX, c.startY)->getType() == Type::PAWN) {
			enPassantPiece = getPiece(c.startX, c.startY);
		}
		// flag king or rook has moved
		else {
			getPiece(c.startX, c.startY)->setIsMoved(true);
		}
		break;
	}
	case (int)specialCodes::CAPTURE:
	{
		// Remove en passant pawn
		this->board[c.endX][c.startY] = nullptr;
		break;
	}
	case (int)specialCodes::CASTLE:
	{
		// Move rook
		int rookStartX = (c.startX > c.endX ? 0 : 7);
		int rookEndX = rookStartX == 0 ? c.endX + 1 : c.endX - 1;

		movePiece({ rookStartX, c.startY, rookEndX, c.startY });
		break;
	}
	case (int)specialCodes::PROMOTE:
	{
		// Replace pawn
		place(c.endX, c.endY, (Type)c.promotion, playerTurn);
		break;
	}
	default:
		// Do nothing
		break;
	}

	special = 0; // Reset once read

	fiftyMoveCount(c);
	threeFoldCount(c);
}

bool Board::isMoveLegal(const Coords& c) const
{
	Piece* piece = getPiece(c.startX, c.startY);
	Piece* end = getPiece(c.endX, c.endY);

	// Check if piece is valid
	if (piece == nullptr) {
		return false;
	}
	if (piece->getColor() != playerTurn) {
		return false;
	}

	// Check if it can move to the end
	if (piece->isValidMove(c, this, special))
	{
		// Taking enemy king overrides the normal rules
		if (end != nullptr && end->getType() == Type::KING) {
			return true;
		}

		Board temp(*this);
		temp.movePiece(c);

		// Check if it puts your king in check
		return !temp.isKingInCheck(playerTurn);
	}

	return false;
}

bool Board::isKingInCheck(Color playerColor) const
{
	Coords toKing = findKing(playerColor);
	Color enemyColor = (playerColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

	Board temp(*this);
	temp.setTurnColor(enemyColor);

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			Piece* piece = getPiece(x, y);

			if (piece != nullptr &&
				piece->getColor() == enemyColor)
			{
				toKing.startX = x;
				toKing.startY = y;
				if (piece->isValidMove(toKing, &temp)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::playerHasLegalMoves(Color playerColor) const
{
	Coords c;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (getPiece(x, y) != nullptr &&
				getPiece(x, y)->getColor() == playerColor)
			{
				c.startY = y;
				c.startX = x;

				for (int yd = 0; yd < 8; yd++)
				{
					for (int xd = 0; xd < 8; xd++)
					{
						c.endX = xd;
						c.endY = yd;

						if (isMoveLegal(c))
							return true;
					}
				}
			}
		}
	}
	return false;
}

int Board::isGameOver(Color playerColor) const
{
	try {
		bool canMove = playerHasLegalMoves(playerColor);
		bool kingInCheckBool = isKingInCheck(playerColor);

		// Checkmate
		if (kingInCheckBool && !canMove)
		{
			return (playerColor == Color::BLACK) ? 1 /* White Wins */ : 2 /* Black Wins */;
		}
		// Stalemate
		else if (!kingInCheckBool && !canMove)
		{
			return 3;
		}
		else if (fiftyMoveCounter >= 100)
		{
			return 4;
		}
		else if (threeFoldCounter >= 3)
		{
			return 5;
		}

		// Game continues
		return 0;
	}
	catch (const std::exception& e) {
		return -2; // No king found
	}

}

Coords Board::findKing(Color playerColor) const
{
	Coords king;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			Piece* piece = getPiece(x, y);
			if (piece != nullptr &&
				piece->getType() == Type::KING &&
				piece->getColor() == playerColor)
			{
				king.endX = x;
				king.endY = y;
				return king;
			}
		}
	}
	// Should never reach here
	throw std::invalid_argument("King not found");
}
