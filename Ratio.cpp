#include "Ratio.h"

Ratio::Ratio()
{
	sum = 0;
	amount = 0;
}

void Ratio::addResult(bool isWinning)
{
	sum += isWinning;
	amount++;
}

float Ratio::getChance()
{
	if (amount) {
		return float(sum) / float(amount);
	}
	return 0.5;
}