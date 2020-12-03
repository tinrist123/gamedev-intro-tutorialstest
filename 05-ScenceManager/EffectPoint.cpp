#include "EffectPoint.h"

EffectPoint::~EffectPoint()
{
}

void EffectPoint::Render()
{
	animation_set->at(0)->Render(x,y);
}
void EffectPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
EffectPoint::EffectPoint()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(9);
	SetAnimationSet(ani_set);

	this->startTimeAlive();
	vy = -VY_SPEED;
}
