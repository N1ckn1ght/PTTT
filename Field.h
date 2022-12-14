#pragma once
#include <iostream>
#include "Cell.cpp"
class Field
{
private:
	Cell**** ptr;
public:
	Field();
	bool insert(size_t bx, size_t by, size_t x, size_t y, Cell elem);

	Cell adjucate();
	Cell adjucate(size_t by, size_t bx);
	bool adjucateFor(size_t by, size_t bx, Cell elem, size_t additionalMoves = 0);

	friend std::ostream& operator << (std::ostream& out, const Field& field);
};