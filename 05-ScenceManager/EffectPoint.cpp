#include "EffectPoint.h"

EffectPoint::~EffectPoint()
{
}

void EffectPoint::Render()
{
	if (state == EFFECT_STATE_7UP)
	{
		ani = EFFECT_ANI_7UP;
	}
	else
	{
		ani = 0;
	}

	animation_set->at(ani)->Render(x,y);
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
