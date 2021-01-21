#include "Sympol_3.h"

Sympol_3::Sympol_3()
{
	this->type = Type::SYMPOL3;
}

Sympol_3::~Sympol_3()
{
}

void Sympol_3::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Sympol_3::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
