#pragma once
#include "Field.h"
#include "Ratio.h"
#include "Coord.h"

class Eurist {
private:
	// How many random non-unique lines will be analyzed per move?
	size_t lines;

	// getChance() will return probability of winning on this move from 0 to 1
	Ratio**** ratios;
	// auxiliary array for Coord*** possible that contains amount of moves
	size_t** count;
	// which moves are possible, basically contains array of Coord(y, x) on certain(by, bx) board
	Coord*** possible;
	// General amount of moves has been made during the analysis
	size_t moves;

	void clear();
	void init(Field& field);
public:
	Eurist(size_t lines_);
	~Eurist();
	
	// Call this in case of bot playing cross hence making the first move
	Coord makeFirstMove(Field& field);
	// Call this in case of bot making any move but the first one
	Coord makeMove(Field& field);
};