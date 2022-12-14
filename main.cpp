#include <iostream>
#include <cstdlib>
#include "Field.h"
#include "Eurist.h"

using namespace std;

void firstMoveByPlayer(Field& field) {
	cout << "You're playing as CROSS. Make first move anywhere!\n" << field;
	size_t by, bx, y, x;
	bool success = false;
	do {
		cout << "Input: {board Y (0-2)} {board X (0-2)} {Y (0-2)} {X (0-2)}, from top-left to bottom-right.\n\n";
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
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n\n";
			cin >> y >> x;
			success = field.insert(y, x);
		} while (!success);
		if (field.adjudicate() == Cell::Null) {
			cout << field << "NULLS won this game. GG!\n";
			return;
		}

		cout << field << "CROSS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
		success = false;
		do {
			cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n\n";
			cin >> y >> x;
			success = field.insert(y, x);
		} while (!success);
		if (field.adjudicate() == Cell::Cross) {
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
		eurist.makeFirstMove(field, 9);
	}

	while (true) {
		if (playerMovesFirst) {
			cout << field << "NULLS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n\n";
			eurist.makeMove(field);
			if (field.adjudicate() == Cell::Null) {
				cout << field << "Eurist won this game with NULLS.\nThank you for playing!\n";
				return;
			}

			cout << field << "CROSS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
			success = false;
			do {
				cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n\n";
				cin >> y >> x;
				success = field.insert(y, x);
			} while (!success);
			if (field.adjudicate() == Cell::Cross) {
				cout << field << "Player won this game with CROSS. GG!\n";
				return;
			}
		}
		else {
			cout << field << "NULLS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n";
			success = false;
			do {
				cout << "Input: {Y (0-2)} {X (0-2)} from top-left.\n\n";
				cin >> y >> x;
				success = field.insert(y, x);
			} while (!success);
			if (field.adjudicate() == Cell::Null) {
				cout << field << "NULLS won this game. Well done!\n";
				return;
			}

			cout << field << "CROSS turn on " << field.getLastMove().y << ", " << field.getLastMove().x << " board!\n\n";
			eurist.makeMove(field);
			if (field.adjudicate() == Cell::Cross) {
				cout << field << "Eurist won this game with CROSS.\nThank you for trying!\n";
				return;
			}
		}
	}
}

int main(int argc, wchar_t* argv[]) {
	srand(time(0));

	Field field;
	Eurist eurist(20000);
	// playerVsPlayer(field);
	playerVsBot(field, eurist, false);
}