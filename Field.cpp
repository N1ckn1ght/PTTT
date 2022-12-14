#include "Field.h"

Field::Field()
{
	ptr = new Cell***[3];
	for (size_t by = 0; by < 3; by++) {
		ptr[by] = new Cell**[3];
		for (size_t bx = 0; bx < 3; bx++) {
			ptr[by][bx] = new Cell*[3];
			for (size_t y = 0; y < 3; y++) {
				ptr[by][bx][y] = new Cell[3];
				for (size_t x = 0; x < 3; x++) {
					ptr[by][bx][y][x] = Cell::Empty;
				}
			}
		}
	}
}

bool Field::insert(size_t bx, size_t by, size_t x, size_t y, Cell elem)
{
	if (by > 2 or bx > 2 or y > 2 or x > 2) {
		return false;
	}
	if (ptr[bx][by][x][y] == Cell::Empty) {
		ptr[bx][by][x][y] = elem;
		return true;
	}
	return false;
}

Cell Field::adjucate()
{
	for (size_t by = 0; by < 3; by++) {
		for (size_t bx = 0; bx < 3; bx++) {
			Cell winner = adjucate(by, bx);
			if (winner != Cell::Empty) {
				return winner;
			}
		}
	}
	return Cell::Empty;
}

Cell Field::adjucate(size_t by, size_t bx)
{
	for (size_t i = 0; i < 3; i++) {
		if (ptr[by][bx][i][0] == ptr[by][bx][i][1] && ptr[by][bx][i][0] == ptr[by][bx][i][2]) {
			return ptr[by][bx][i][0];
		}
		if (ptr[by][bx][0][i] == ptr[by][bx][1][i] && ptr[by][bx][0][i] == ptr[by][bx][2][i]) {
			return ptr[by][bx][0][i];
		}
	}
	if (ptr[by][bx][0][0] == ptr[by][bx][1][1] && ptr[by][bx][0][0] == ptr[by][bx][2][2]) {
		return ptr[by][bx][0][0];
	}
	if (ptr[by][bx][0][2] == ptr[by][bx][1][1] && ptr[by][bx][0][2] == ptr[by][bx][2][0]) {
		return ptr[by][bx][0][2];
	}
	return Cell::Empty;
}

bool Field::adjucateFor(size_t by, size_t bx, Cell elem, size_t additionalMoves)
{
	for (size_t i = 0; i < 3; i++) {
		if ((ptr[by][bx][i][0] == elem) + (ptr[by][bx][i][1] == elem) + (ptr[by][bx][i][2] == elem) > 2 - additionalMoves) {
			return true;
		}
		if ((ptr[by][bx][0][i] == elem) + (ptr[by][bx][1][i] == elem) + (ptr[by][bx][2][i] == elem) > 2 - additionalMoves) {
			return true;
		}
	}
	if ((ptr[by][bx][0][0] == elem) + (ptr[by][bx][1][1] == elem) + (ptr[by][bx][2][2] == elem) > 2 - additionalMoves) {
		return true;
	}
	if ((ptr[by][bx][0][2] == elem) + (ptr[by][bx][1][1] == elem) + (ptr[by][bx][2][0] == elem) > 2 - additionalMoves) {
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, const Field& field)
{
	for (size_t by = 0; by < 3; by++) {
		out << " _ _ _    _ _ _    _ _ _\n";
		for (size_t y = 0; y < 3; y++) {
			for (size_t bx = 0; bx < 3; bx++) {
				out << "|";
				for (size_t x = 0; x < 3; x++) {
					switch (field.ptr[by][bx][y][x]) {
					case Cell::Cross:
						out << "X";
						break;
					case Cell::Null:
						out << "O";
						break;
					case Cell::Empty:
						out << " ";
						break;
					default:
						out << " ";
						break;
					}
					if (x < 2) {
						out << " ";
					}
				}
				out << "| ";
				if (bx < 2) {
					out << " ";
				}
			}
			out << "\n";
		}
		out << " - - -    - - -    - - -\n";
	}
	return out;
}