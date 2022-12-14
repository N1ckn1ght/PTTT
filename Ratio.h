#pragma once

class Ratio {
private:
	size_t sum;
	size_t amount;
public:
	Ratio();

	void addResult(const bool isWinning);
	float getChance();
};