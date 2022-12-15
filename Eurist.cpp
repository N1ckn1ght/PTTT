#include "Eurist.h"
#include <vector>

Eurist::Eurist(size_t lines_, float k_, bool debug_)
{
	lines = lines_;
	k = k_;
	debug = debug_;
	ratios = nullptr;
	count = nullptr;
	possible = nullptr;
	moves = 0;
}

Eurist::~Eurist()
{
	clear();
}

std::vector <EuristMove> Eurist::eval(Field& field)
{
	clear();
	init(field);

	Coord lastMove = field.getLastMove();
	std::vector <EuristMove> euristMoves;
	if (lastMove.y != -1) {
		eval(field, lastMove.y, lastMove.x, euristMoves, lines / count[lastMove.y][lastMove.x], field.getTurn());

		if (debug) {
			std::cout << "Eurist: I've evaluated " << count[lastMove.y][lastMove.x] << " legal moves, ";
			std::cout << lines / count[lastMove.y][lastMove.x] << " random lines for each;\n";
			std::cout << "Eurist: Total random non-unique moves made: " << moves << ".\n";
		}
	}
	else {
		size_t totalPossibleMoves = 0;
		for (size_t by = 0; by < 3; by++) {
			for (size_t bx = 0; bx < 3; bx++) {
				totalPossibleMoves += count[by][bx];
			}
		}
		for (size_t by = 0; by < 3; by++) {
			for (size_t bx = 0; bx < 3; bx++) {
				eval(field, by, bx, euristMoves, size_t(float(lines) * k / totalPossibleMoves), field.getTurn());
			}
		}

		if (debug) {
			std::cout << "Eurist: I've evaluated " << totalPossibleMoves << " legal moves, ";
			std::cout << size_t(float(lines) * k / totalPossibleMoves) << " random lines for each;\n";
			std::cout << "Eurist: Total random non-unique moves made: " << moves << ".\n";
		}
	}

	sort(euristMoves.begin(), euristMoves.end(), euristMoveCmp);
	return euristMoves;
}

void Eurist::eval(Field& field, size_t by, size_t bx, std::vector<EuristMove>& euristMoves, size_t lines_, Cell side)
{
	for (size_t i = 0; i < count[by][bx]; i++) {
		for (size_t j = 0; j < lines_; j++) {
			// Make field copy
			Field field_;
			for (size_t iby = 0; iby < 3; iby++) {
				for (size_t ibx = 0; ibx < 3; ibx++) {
					for (size_t iy = 0; iy < 3; iy++) {
						for (size_t ix = 0; ix < 3; ix++) {
							field_.set(iby, ibx, iy, ix, field.get(iby, ibx, iy, ix));
						}
					}
				}
			}
			field_.setTurn(field.getTurn());
			field_.setLastMove(field.getLastMove());

			// Make possible moves copy
			size_t** count_ = new size_t * [3];
			Coord*** possible_ = new Coord * *[3];
			for (size_t iby = 0; iby < 3; iby++) {
				count_[iby] = new size_t[3];
				possible_[iby] = new Coord * [3];
				for (size_t ibx = 0; ibx < 3; ibx++) {
					count_[iby][ibx] = count[iby][ibx];
					possible_[iby][ibx] = new Coord[9];
					for (size_t k = 0; k < count_[iby][ibx]; k++) {
						possible_[iby][ibx][k] = possible[iby][ibx][k];
					}
				}
			}

			// Make certain possible[by][bx][i] move and check for insta-winning
			moves++;
			// And also here's a patch in case of first move
			if (field.getLastMove().y == -1) {
				field_.insert(by, bx, possible[by][bx][i].y, possible[by][bx][i].x);
			}
			else {
				field_.insert(possible[by][bx][i].y, possible[by][bx][i].x);
				if (field_.adjudicate() == side) {
					ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(Result::Win);
					break;
				}
				std::swap(possible_[by][bx][i], possible_[by][bx][--count_[by][bx]]);
			}
			
			// Continue by playing random game until someone wins
			Cell winner = Cell::Empty;
			do {
				moves++;
				size_t cy = field_.getLastMove().y;
				size_t cx = field_.getLastMove().x;
				if (!count_[cy][cx]) {
					// situation, where there may be cells to place, just not in the [cy][cx]; considered a draw by now
					ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(Result::Draw);
					break;
				}
				// make random move
				size_t move = rand() % count_[cy][cx];
				field_.insert(possible_[cy][cx][move].y, possible_[cy][cx][move].x);
				std::swap(possible_[cy][cx][move], possible_[cy][cx][--count_[cy][cx]]);
				// while it isn't winning for someone
				winner = field_.adjudicate();
			} while (winner == Cell::Empty);

			// Add the outcome to probability of making possible[by][bx][i] move
			if (winner == side) {
				ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(Result::Win);
			}
			else if (winner == nextCell(side)) {
				ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(Result::Loss);
			}
			else {
				ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(Result::Draw);
			}
		}
	}

	for (size_t i = 0; i < count[by][bx]; i++) {
		euristMoves.push_back(EuristMove(by, bx, possible[by][bx][i].y, possible[by][bx][i].x, ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x]));
	}
}

void Eurist::clear()
{
	moves = 0;
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