#include "PMaxSpeed.h"

PMaxSpeed::PMaxSpeed()
{
}

void PMaxSpeed::Render()
{
	animation_set->at(38)->Render(x, y, 255);
}
void PMaxSpeed::Update(float camX, float camY)
{
	x = camX;
	y = camY;
}

void PMaxSpeed::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
