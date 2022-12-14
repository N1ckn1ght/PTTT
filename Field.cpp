#include "Field.h"

Field::Field()
{
	turn = Cell::Cross;
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

Field::~Field()
{
	for (size_t by = 0; by < 3; by++) {
		for (size_t bx = 0; bx < 3; bx++) {
			for (size_t y = 0; y < 3; y++) {
				delete[] ptr[by][bx][y];
			}
			delete[] ptr[by][bx];
		}
		delete[] ptr[by];
	}
	delete[] ptr;
}

bool Field::insert(const size_t by, const size_t bx, const size_t y, const size_t x)
{
	if (by > 2 || bx > 2 || y > 2 || x > 2 || lastMove.y != -1) {
		return false;
	}
	ptr[by][bx][y][x] = turn;
	turn = nextCell(turn);
	lastMove = Coord(y, x);
	return true;
}

bool Field::insert(const size_t y, const size_t x)
{
	if (y > 2 || x > 2 || ptr[lastMove.y][lastMove.x][y][x] != Cell::Empty) {
		return false;
	}
	ptr[lastMove.y][lastMove.x][y][x] = turn;
	turn = nextCell(turn);
	lastMove = Coord(y, x);
	return true;
}

Cell Field::get(size_t by, size_t bx, size_t y, size_t x)
{
	return ptr[by][bx][y][x];
}

Cell Field::adjudicate()
{
	for (size_t by = 0; by < 3; by++) {
		for (size_t bx = 0; bx < 3; bx++) {
			Cell winner = adjudicate(by, bx);
			if (winner != Cell::Empty) {
				return winner;
			}
		}
	}
	return Cell::Empty;
}

Cell Field::adjudicate(size_t by, size_t bx)
{
	for (size_t i = 0; i < 3; i++) {
		if (ptr[by][bx][i][0] != Cell::Empty && ptr[by][bx][i][0] == ptr[by][bx][i][1] && ptr[by][bx][i][0] == ptr[by][bx][i][2]) {
			return ptr[by][bx][i][0];
		}
		if (ptr[by][bx][0][i] != Cell::Empty && ptr[by][bx][0][i] == ptr[by][bx][1][i] && ptr[by][bx][0][i] == ptr[by][bx][2][i]) {
			return ptr[by][bx][0][i];
		}
	}
	if (ptr[by][bx][0][0] != Cell::Empty && ptr[by][bx][0][0] == ptr[by][bx][1][1] && ptr[by][bx][0][0] == ptr[by][bx][2][2]) {
		return ptr[by][bx][0][0];
	}
	if (ptr[by][bx][0][2] != Cell::Empty && ptr[by][bx][0][2] == ptr[by][bx][1][1] && ptr[by][bx][0][2] == ptr[by][bx][2][0]) {
		return ptr[by][bx][0][2];
	}
	return Cell::Empty;
}

bool Field::adjudicateFor(size_t by, size_t bx, Cell elem, size_t additionalMoves)
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

Cell Field::getTurn()
{
	return turn;
}

Coord Field::getLastMove()
{
	return lastMove;
}

void Field::set(const size_t by, const size_t bx, const size_t y, const size_t x, const Cell elem)
{
	ptr[by][bx][y][x] = elem;
}

void Field::setTurn(Cell cell)
{
	turn = cell;
}

void Field::setLastMove(Coord coord)
{
	lastMove = coord;
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