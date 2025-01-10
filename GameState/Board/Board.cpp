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
	place(0, 1, Type::KNIGHT, Color::WHITE);
	place(0, 2, Type::BISHOP, Color::WHITE);
	place(0, 3, Type::QUEEN, Color::WHITE);
	place(0, 4, Type::KING, Color::WHITE);
	place(0, 5, Type::BISHOP, Color::WHITE);
	place(0, 6, Type::KNIGHT, Color::WHITE);
	place(0, 7, Type::ROOK, Color::WHITE);
	for (int i = 0; i < 8; i++)
	{
		place(1, i, Type::PAWN, Color::WHITE);
	}

	place(7, 0, Type::ROOK, Color::BLACK);
	place(7, 1, Type::KNIGHT, Color::BLACK);
	place(7, 2, Type::BISHOP, Color::BLACK);
	place(7, 3, Type::QUEEN, Color::BLACK);
	place(7, 4, Type::KING, Color::BLACK);
	place(7, 5, Type::BISHOP, Color::BLACK);
	place(7, 6, Type::KNIGHT, Color::BLACK);
	place(7, 7, Type::ROOK, Color::BLACK);
	for (int i = 0; i < 8; i++)
	{
		place(6, i, Type::PAWN, Color::BLACK);
	}
}

Board::Board(const Board& b)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
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
	-1 - Invalid move
	 0 - Successful move
	 1 - Checkmate by White
	 2 - Checkmate by Black
	 3 - Stalemate
	 4 - Move results in draw by fifty-move rule
	 5 - Move results in draw by threefold repetition
	 6 - Move results in draw by insufficient material
 */
int Board::move(const Coords& c)
{
	if (isMoveLegal(c))
	{
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

int Board::getEnPassantX() const
{
	return enPassantX;
}

void Board::setTurnColor(Color color)
{
	playerTurn = color;
}

void Board::place(int startX, int startY, Type piece, Color color)
{
	switch (piece)
	{
	case Type::PAWN:
		this->board[startX][startY] = std::make_unique<Pawn>(color);
		break;
	case Type::ROOK:
		this->board[startX][startY] = std::make_unique<Rook>(color);
		break;
	case Type::KNIGHT:
		this->board[startX][startY] = std::make_unique<Knight>(color);
		break;
	case Type::BISHOP:
		this->board[startX][startY] = std::make_unique<Bishop>(color);
		break;
	case Type::QUEEN:
		this->board[startX][startY] = std::make_unique<Queen>(color);
		break;
	case Type::KING:
		this->board[startX][startY] = std::make_unique<King>(color);
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

void Board::movePiece(const Coords& c)
{
	// En passant
	enPassantX = -1;
	if (this->board[c.startX][c.startY]->getType() == Type::PAWN)
	{
		// Set flag
		if (c.startX == c.endX && abs(c.startY - c.endY) == 2) {
			enPassantX = c.startX;
		}
		
		// Remove En Passant-ed pawn
		if (abs(c.startX - c.endX) == 1 && this->board[c.endX][c.endY] == nullptr) {
			int offsetY = c.endY - Piece::getColorDirection(playerTurn);
			this->board[c.endX][offsetY] = nullptr;
		}
	}

	// Move piece
	this->board[c.endX][c.endY] = std::move(this->board[c.startX][c.startY]);
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
	if (piece->isValidMove(c, this))
	{
		// Check if the move wins the game
		if (end != nullptr && end->getType() == Type::KING) {
			return true;
		}

		// Check if it puts the player in check
		Board temp(*this);
		temp.movePiece(c);

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
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* piece = getPiece(i, j);
			if (piece != nullptr &&
				piece->getType() == Type::KING &&
				piece->getColor() == playerColor)
			{
				king.endX = i;
				king.endY = j;
				return king;
			}
		}
	}
	// Should never reach here
	throw std::invalid_argument("King not found");
}
