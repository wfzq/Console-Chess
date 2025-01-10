#include "Piece.h"
#include "../../Board.h"

Piece::Piece(Color color) : pieceColor{ color } {}

Color Piece::getColor() const { return pieceColor; }

bool Piece::isEndSquareValid(const Coords& c, const Board* board) const {
    return board->getPiece(c.endX, c.endY) == nullptr || pieceColor != board->getPiece(c.endX, c.endY)->getColor();
}

int Piece::getColorDirection(const Color pieceColor) {
    return pieceColor == Color::WHITE ? 1 : -1;
}

int Piece::getMoveDirection(int start, int exit) {
    return start < exit ? 1 : -1;
}

bool Piece::areIntermediateXSquaresEmpty(const Coords& c, const Board* board) {
    int direction = getMoveDirection(c.startX, c.endX);
    for (int i = c.startX + direction; i != c.endX; i += direction) {
        if (board->getPiece(i, c.startY) != nullptr) {
            return false;
        }
    }
    return true;
}

bool Piece::areIntermediateYSquaresEmpty(const Coords& c, const Board* board) {
    int direction = getMoveDirection(c.startY, c.endY);
    for (int i = c.startY + direction; i != c.endY; i += direction) {
        if (board->getPiece(c.startX, i) != nullptr) {
            return false;
        }
    }
    return true;
}

bool Piece::areIntermediateDiagonalSquaresEmpty(const Coords& c, const Board* board) {
    int directionX = getMoveDirection(c.startX, c.endX);
    int directionY = getMoveDirection(c.startY, c.endY);
    for (int i = c.startX + directionX, j = c.startY + directionY; i != c.endX; i += directionX, j += directionY) {
        if (board->getPiece(i, j) != nullptr) {
            return false;
        }
    }
    return true;
}

bool Piece::isStartExitOnSameDiagonal(const Coords& c) {
    return abs(c.startX - c.endX) == abs(c.startY - c.endY);
}
