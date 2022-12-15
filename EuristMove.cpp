#include "EuristMove.h"

EuristMove::EuristMove(size_t by_, size_t bx_, size_t y_, size_t x_, Ratio chance_)
{
	by = by_;
	bx = bx_;
	y = y_;
	x = x_;
	chance = chance_.getChance();
}

bool euristMoveCmp(EuristMove a, EuristMove b)
{
	/* if (a.chance == b.chance) {
		if (a.by == b.by) {
			if (a.bx == b.bx) {
				if (a.y == b.y) {
					return a.x < b.x;
				}
				return a.y < b.y;
			}
			return a.bx < b.bx;
		}
		return a.by < b.by;
	} */
	
	return a.chance > b.chance;
}