#include "TripleBoxItem.h"

TripleBoxItem::TripleBoxItem()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(15);
	SetAnimationSet(ani_set);
}

void TripleBoxItem::Render()
{
	animation_set->at(1)->Render(x, y);
}

void TripleBoxItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}