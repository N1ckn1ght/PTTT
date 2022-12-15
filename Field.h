#pragma once
#include <iostream>
#include "Cell.h"
#include "Coord.h"

class Field
{
private:
	Cell**** ptr;

	// Which turn it is
	Cell turn;
	// Last move by (by, bx)
	Coord lastBoard;
	// Last move by (y, x)
	Coord lastMove;
public:
	Field();
	~Field();

	// This method has protection and will return false on failure or it will place CROSS or NULL accordingly to its Cell turn
	bool insert(const size_t by, const size_t bx, const size_t y, const size_t x);
	// This method can be called if (by, bx) are deterministic
	bool insert(const size_t y, const size_t x);
	// This method has no protection from illegal (any > 2) values
	Cell get(const size_t by, const size_t bx, const size_t y, const size_t x);
	// Will check lastBoard and determine winner by it, or draw by lastMove
	// Will return winner, or Cell::Any on draw, or Cell::Empty
	Cell adjudicate();

	Cell getTurn();
	Coord getLastMove();

	// This method has no protection from illegal (any > 2) values or moves; doesn't modify turn or lastMove
	void set(const size_t by, const size_t bx, const size_t y, const size_t x, const Cell elem);
	// Don't call this if you didn't use set previously
	void setTurn(Cell cell);
	// Don't call this if you didn't use set previously
	void setLastMove(Coord coord);

	friend std::ostream& operator << (std::ostream& out, const Field& field);
};