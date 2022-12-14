#include <iostream>
#include <utility>
#include "Field.h"

using namespace std;

pair <size_t, size_t> firstMove(Field& field) {
	cout << "You're playing as CROSS. Make first move anywhere!\n" << field;
	size_t by, bx, y, x;
	bool success = false;
	do {
		cout << "Input: {board Y (0-2)} {board X (0-2)} {Y (0-2)} {X (0-2)}, from top-left to bottom-right.\n";
		cin >> by >> bx >> y >> x;
		success = field.insert(by, bx, y, x, Cell::Cross);
	} while (!success);
	return make_pair(y, x);
}

void playerVsPlayer(Field& field) {
	size_t y, x;
	bool success;

	pair <size_t, size_t> nextBoard = firstMove(field);
	while (true) {
		cout << field << "NULLS turn on " << nextBoard.first << ", " << nextBoard.second << " board!\n";
		success = false;
		do {
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n";
			cin >> y >> x;
			success = field.insert(nextBoard.first, nextBoard.second, y, x, Cell::Null);
		} while (!success);
		if (field.adjucateFor(nextBoard.first, nextBoard.second, Cell::Null)) {
			cout << field << "NULLS won this game. GG!\n";
			return;
		}

		nextBoard = make_pair(y, x);
		cout << field << "CROSS turn on " << nextBoard.first << ", " << nextBoard.second << " board!\n";
		success = false;
		do {
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n";
			cin >> y >> x;
			success = field.insert(nextBoard.first, nextBoard.second, y, x, Cell::Cross);
		} while (!success);
		if (field.adjucateFor(nextBoard.first, nextBoard.second, Cell::Cross)) {
			cout << field << "CROSS won this game. GG!\n";
			return;
		}

		nextBoard = make_pair(y, x);
	}
}

int main(int argc, wchar_t* argv[]) {

	Field field;
	playerVsPlayer(field);
}