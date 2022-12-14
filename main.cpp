#include <iostream>
#include "Field.h"
#include "Eurist.h"

using namespace std;

void firstMoveByPlayer(Field& field) {
	cout << "You're playing as CROSS. Make first move anywhere!\n" << field;
	size_t by, bx, y, x;
	bool success = false;
	do {
		cout << "Input: {board Y (0-2)} {board X (0-2)} {Y (0-2)} {X (0-2)}, from top-left to bottom-right.\n";
		cin >> by >> bx >> y >> x;
		success = field.insert(by, bx, y, x);
	} while (!success);
}

void playerVsPlayer(Field& field) {
	size_t y, x;
	bool success;

	firstMoveByPlayer(field);
	while (true) {
		cout << field << "NULLS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
		success = false;
		do {
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n";
			cin >> y >> x;
			success = field.insert(y, x);
		} while (!success);
		if (field.adjucate() == Cell::Null) {
			cout << field << "NULLS won this game. GG!\n";
			return;
		}

		cout << field << "CROSS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
		success = false;
		do {
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n";
			cin >> y >> x;
			success = field.insert(y, x);
		} while (!success);
		if (field.adjucate() == Cell::Cross) {
			cout << field << "CROSS won this game. GG!\n";
			return;
		}
	}
}

void playerVsBot(Field& field, Eurist& eurist, bool playerMovesFirst) {
	size_t y, x;
	bool success;

	if (playerMovesFirst) {
		firstMoveByPlayer(field);
	}
	else {
		// TODO: bot first move
	}

	while (true) {
		if (playerMovesFirst) {
			cout << field << "NULLS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
			eurist.makeMove(field);
			if (field.adjucate() == Cell::Null) {
				cout << field << "Eurist won this game with NULLS.\nThank you for playing!\n";
				return;
			}

			cout << field << "CROSS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
			success = false;
			do {
				cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n";
				cin >> y >> x;
				success = field.insert(y, x);
			} while (!success);
			if (field.adjucate() == Cell::Cross) {
				cout << field << "Player won this game with CROSS. GG!\n";
				return;
			}
		}
		else {
			// pass
		}
	}
}

int main(int argc, wchar_t* argv[]) {

	Field field;
	Eurist eurist(1000);
	// playerVsPlayer(field);
	playerVsBot(field, eurist, true);
}