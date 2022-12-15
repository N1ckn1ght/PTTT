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
	if (by > 2 || bx > 2 || y > 2 || x > 2) {
		return false;
	}
	if (lastMove.y != -1 && by != lastMove.y && bx != lastMove.x) {
		return false;
	}
	ptr[by][bx][y][x] = turn;
	turn = nextCell(turn);
	lastBoard = Coord(by, bx);
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
	lastBoard = Coord(lastMove.y, lastMove.x);
	lastMove = Coord(y, x);
	return true;
}

Cell Field::get(size_t by, size_t bx, size_t y, size_t x)
{
	return ptr[by][bx][y][x];
}

Cell Field::adjudicate()
{
	if (lastMove.y == -1) {
		return Cell::Empty;
	}

	size_t by = lastBoard.y;
	size_t bx = lastBoard.x;

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

	size_t movesLeft = 9;
	by = lastMove.y;
	bx = lastMove.x;

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			if (ptr[by][bx][i][j] != Cell::Empty) {
				movesLeft--;
			}
		}
	}

	if (movesLeft) {
		return Cell::Empty;
	}
	return Cell::Any;
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
		out << " _ _ _   _ _ _   _ _ _";
		if (by == 0) {
			out << "  Y";
		}
		out << "\n";
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
				if (bx == 2) {
					out << y;
				} 
			}
			out << "\n";
		}
		out << " - - -   - - -   - - -\n";
	}
	out << " 0 1 2   0 1 2   0 1 2  X\n";
	return out;
}