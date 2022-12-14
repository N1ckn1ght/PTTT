#include "Eurist.h"

Eurist::Eurist(size_t lines_)
{
	lines = lines_;
	ratios = nullptr;
	count = nullptr;
	possible = nullptr;
	moves = 0;
}

Eurist::~Eurist()
{
	clear();
}

Coord Eurist::makeFirstMove(Field& field)
{
	// always center? :D
	return false;
}

Coord Eurist::makeMove(Field& field)
{
	clear();
	init(field);

	size_t by = field.getLastMove().y;
	size_t bx = field.getLastMove().x;

	// TODO: make option to allow non-euristic calculations of certain depth
	// (at least check for depth=1 winning or losing possibilities)

	std::cout << "Eurist: lemme see... I'll move on (" << possible[by][bx][0].y << ", " << possible[by][bx][0].x << ")\n";
	field.insert(possible[by][bx][0].y, possible[by][bx][0].x);

	// make analysis here

	return false;
}

void Eurist::clear()
{
	if (ratios) {
		for (size_t by = 0; by < 3; by++) {
			for (size_t bx = 0; bx < 3; bx++) {
				for (size_t y = 0; y < 3; y++) {
					delete[] ratios[by][bx][y];
				}
				delete[] ratios[by][bx];
			}
			delete[] ratios[by];
		}
		delete[] ratios;
		ratios = nullptr;
	}
	if (count) {
		for (size_t by = 0; by < 3; by++) {
			delete[] count[by];
		}
		delete[] count;
		count = nullptr;
	}
	if (possible) {
		for (size_t by = 0; by < 3; by++) {
			for (size_t bx = 0; bx < 3; bx++) {
				delete[] possible[by][bx];
			}
			delete[] possible[by];
		}
		delete[] possible;
		possible = nullptr;
	}
}

void Eurist::init(Field& field)
{
	ratios = new Ratio***[3];
	count = new size_t*[3];
	possible = new Coord**[3];
	for (size_t by = 0; by < 3; by++) {
		ratios[by] = new Ratio**[3];
		count[by] = new size_t[3];
		possible[by] = new Coord*[3];
		for (size_t bx = 0; bx < 3; bx++) {
			ratios[by][bx] = new Ratio*[3];
			count[by][bx] = 0;
			possible[by][bx] = new Coord[9];
			for (size_t y = 0; y < 3; y++) {
				ratios[by][bx][y] = new Ratio[3];
				for (size_t x = 0; x < 3; x++) {
					ratios[by][bx][y][x] = Ratio();
					if (field.get(by, bx, y, x) == Cell::Empty) {
						possible[by][bx][count[by][bx]++] = Coord(y, x);
					}
				}
			}
		}
	}
}