#include "Board.h"
#include "Pieces/Abstract/Piece.h"
#include <stdexcept>

Board::Board()
{
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

Piece* Board::getPiece(int x, int y) const
{
	return board[x][y].get();
}

void Board::setTurnColor(Color color)
{
	playerTurn = color;
}

void Board::incrementTurn()
{
	playerTurn = (playerTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
	turnCounter++;
}

void Board::movePiece(const Coords& c)
{
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
	// Checkmate
	if (isKingInCheck(playerColor) && !playerHasLegalMoves(playerColor))
	{
		return (playerColor == Color::BLACK) ? 1 /* White Wins */ : 2 /* Black Wins */;
	}
	// Stalemate
	else if (!isKingInCheck(playerColor) && !playerHasLegalMoves(playerColor))
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
