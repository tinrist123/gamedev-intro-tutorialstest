#include "EffectHitTail.h"

EffectHitTail::EffectHitTail()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(10);
	SetAnimationSet(ani_set);
	this->startTimeAlive();
}

EffectHitTail::~EffectHitTail()
{
}

void EffectHitTail::Render()
{
	animation_set->at(0)->Render(x, y, 255);
}

void EffectHitTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r = t = b;
}
