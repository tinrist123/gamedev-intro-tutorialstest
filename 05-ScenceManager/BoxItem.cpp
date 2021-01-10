#include "BoxItem.h"

BoxItem::BoxItem()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(15);
	SetAnimationSet(ani_set);
}

void BoxItem::Render()
{
	animation_set->at(0)->Render(x, y);
}

void BoxItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
