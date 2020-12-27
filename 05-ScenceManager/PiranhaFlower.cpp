#include "PiranhaFlower.h"

void PiranhaFlower::Render()
{
	if (state == FLOWER_STATE_UP)
	{
		ani = 1;
	}
}

void PiranhaFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
