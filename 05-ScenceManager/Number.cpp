#include "Number.h"

Number::Number()
{
}

void Number::Render()
{
	animation_set->at(ani + 1)->Render(x, y, 255);
}

void Number::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}

Number::~Number()
{

}
