#include "Eurist.h"
#include <vector>

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

// This method is a copy-paste of makeMove() adapted to first move only;
// It's a test case. Usually the best one is just (1, 1, 1, 1).
// TODO: bring this mess of a code in order.
void Eurist::makeFirstMove(Field& field, size_t k)
{
	init(field);

	Cell side = field.getTurn();
	size_t fraction = lines / 81;

	for (size_t by = 0; by < 3; by++) {
		for (size_t bx = 0; bx < 3; bx++) {
			
			for (size_t i = 0; i < count[by][bx]; i++) {
				for (size_t j = 0; j < fraction; j++) {

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

					// Play this random game until someone wins
					moves++;
					field_.insert(by, bx, possible[by][bx][i].y, possible[by][bx][i].x);
					std::swap(possible_[by][bx][i], possible_[by][bx][--count_[by][bx]]);
					Cell winner = Cell::Empty;
					do {
						moves++;
						size_t cy = field_.getLastMove().y;
						size_t cx = field_.getLastMove().x;
						if (!count_[cy][cx]) {
							// it's a draw for now
							// situation, where there may be cells to place, just not in the [cy][cx]
							ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(0.5);
							break;
						}
						// make random move
						size_t move = rand() % count_[cy][cx];
						field_.insert(possible_[cy][cx][move].y, possible_[cy][cx][move].x);
						std::swap(possible_[cy][cx][move], possible_[cy][cx][--count_[cy][cx]]);
						// while it isn't winning for someone
						winner = field_.adjudicate(cy, cx);
					} while (winner == Cell::Empty);

					// Add the outcome to probability of making possible[by][bx][i] move
					if (winner != Cell::Empty) {
						ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(winner == side);
					}
				}
			}
		}
	}

	// Little debug info:
	std::cout << "Eurist: I've evaluated 81 legal moves, " << fraction << " random lines for each;\n";
	std::cout << "Eurist: Total random non-unique moves made: " << moves << ".\n";
	float maxc = -1.0;
	size_t maxi = 0, maxby = 0, maxbx = 0;
	for (size_t by = 0; by < 3; by++) {
		for (size_t bx = 0; bx < 3; bx++) {
			for (size_t i = 0; i < count[by][bx]; i++) {
				float cur = ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].getChance();
				if (cur > maxc) {
					maxc = cur;
					maxi = i;
					maxby = by;
					maxbx = bx;
				}
			}
		}
	}
	std::cout << "Eurist: My chance of winning now is: " << maxc << "\n";
	std::cout << "Eurist: I'll make move (" << maxby << ", " << maxbx << ", " << possible[maxby][maxbx][maxi].y << ", " << possible[maxby][maxbx][maxi].x << ").\n\n";
	field.insert(maxby, maxbx, possible[maxby][maxbx][maxi].y, possible[maxby][maxbx][maxi].x);
}

void Eurist::makeMove(Field& field)
{
	clear();
	init(field);

	size_t by = field.getLastMove().y;
	size_t bx = field.getLastMove().x;

	// TODO: make option to allow non-euristic calculations of certain depth
	// (at least check for depth=1 winning or losing possibilities)

	// This algorithm will try to win for the side of the current turn of the game
	Cell side = field.getTurn();
	size_t fraction = lines / count[by][bx];
	for (size_t i = 0; i < count[by][bx]; i++) {
		for (size_t j = 0; j < fraction; j++) {

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
			size_t** count_ = new size_t*[3];
			Coord*** possible_ = new Coord**[3];
			for (size_t iby = 0; iby < 3; iby++) {
				count_[iby] = new size_t[3];
				possible_[iby] = new Coord*[3];
				for (size_t ibx = 0; ibx < 3; ibx++) {
					count_[iby][ibx] = count[iby][ibx];
					possible_[iby][ibx] = new Coord[9];
					for (size_t k = 0; k < count_[iby][ibx]; k++) {
						possible_[iby][ibx][k] = possible[iby][ibx][k];
					}
				}
			}

			// Play this random game until someone wins
			moves++;
			field_.insert(possible[by][bx][i].y, possible[by][bx][i].x);

			if (field_.adjudicateFor(by, bx, side)) {
				// exception case of insta-winning
				ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(1);
				break;
			}
			std::swap(possible_[by][bx][i], possible_[by][bx][--count_[by][bx]]);
			Cell winner = Cell::Empty;
			do {
				moves++;
				size_t cy = field_.getLastMove().y;
				size_t cx = field_.getLastMove().x;
				if (!count_[cy][cx]) {
					// it's a draw for now
					// situation, where there may be cells to place, just not in the [cy][cx]
					ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(0.5);
					break;
				}
				// make random move
				size_t move = rand() % count_[cy][cx];
				field_.insert(possible_[cy][cx][move].y, possible_[cy][cx][move].x);
				std::swap(possible_[cy][cx][move], possible_[cy][cx][--count_[cy][cx]]);
				// while it isn't winning for someone
				winner = field_.adjudicate(cy, cx);
			} while (winner == Cell::Empty);

			// Add the outcome to probability of making possible[by][bx][i] move
			if (winner != Cell::Empty) {
				ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].addResult(winner == side);
			}
		}
	}

	// Little debug info:
	std::cout << "Eurist: I've evaluated " << count[by][bx] << " legal moves, " << fraction << " random lines for each;\n";
	std::cout << "Eurist: Total random non-unique moves made: " << moves << ".\n";
	float maxc = -1.0;
	size_t maxi = 0;
	for (size_t i = 0; i < count[by][bx]; i++) {
		float cur = ratios[by][bx][possible[by][bx][i].y][possible[by][bx][i].x].getChance();
		std::cout << "Eurist: Move (" << possible[by][bx][i].y << ", " << possible[by][bx][i].x << ")\tRatio: " << cur << "\n";
		if (cur > maxc) {
			maxc = cur;
			maxi = i;
		}
	}
	std::cout << "Eurist: I'll make move (" << possible[by][bx][maxi].y << ", " << possible[by][bx][maxi].x << ").\n\n";
	field.insert(possible[by][bx][maxi].y, possible[by][bx][maxi].x);
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