#pragma once
#include <cstdlib>
#include "Field.h"
#include "Ratio.h"
#include "Coord.h"
#include "EuristMove.h"
// sorting eval output vector
#include <algorithm>
#include <vector>

class Eurist {
private:
	// How many random non-unique lines will be analyzed per move?
	size_t lines;
	// Line analysis multiplier in case of any board move
	float k;
	// Should it write debug information in std::cout?
	bool debug;

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
	void eval(Field& field, size_t by, size_t bx, std::vector <EuristMove>& euristMoves, size_t lines_, Cell side);
public:
	Eurist(size_t lines_, float k_, bool debug_ = false);
	~Eurist();
	
	std::vector <EuristMove> eval(Field& field);

	// Call this in case of bot playing cross hence making the first move
	// k is the multiplier for lines analyzed, there're 9 times more possibilities for the first move
	
	//void makeFirstMove(Field& field, size_t k = 1);
	
	// Call this in case of bot making any move but the first one
	
	//void makeMove(Field& field);
};