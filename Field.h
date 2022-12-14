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
	// Last move by (y, x)
	Coord lastMove;
public:
	Field();
	~Field();

	// This method has protection and can be used at first move only
	bool insert(const size_t by, const size_t bx, const size_t y, const size_t x);
	// This method has protection and will return false on failure or it will place CROSS or NULL accordingly to its Cell turn
	bool insert(const size_t y, const size_t x);
	// This method has no protection from illegal (any > 2) values
	Cell get(const size_t by, const size_t bx, const size_t y, const size_t x);
	// Will call the next adjucate(by, bx) method until victor will be found
	Cell adjucate();
	// Will return victor of the 3x3 board or Cell::Empty
	Cell adjucate(const size_t by, const size_t bx);
	// With additionalMoves = 1 will return true if certain elem is just 1 move from victory
	bool adjucateFor(const size_t by, const size_t bx, const Cell elem, const size_t additionalMoves = 0);

	Cell getTurn();
	Coord getLastMove();

	Field makeCopy();
	// This method has no protection from illegal (any > 2) values or moves; doesn't modify turn or lastMove
	void set(const size_t by, const size_t bx, const size_t y, const size_t x, const Cell elem);
	// Don't call this if you didn't use set previously
	void setTurn(Cell cell);
	// Don't call this if you didn't use set previously
	void setLastMove(Coord coord);

	friend std::ostream& operator << (std::ostream& out, const Field& field);
};