#include "CarretSpeed.h"

CarretSpeed::CarretSpeed()
{
}

void CarretSpeed::Render()
{
	animation_set->at(36)->Render(x,y, 255);
}

void CarretSpeed::Update(float camX, float camY)
{
	x = camX;
	y = camY;
}

void CarretSpeed::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
