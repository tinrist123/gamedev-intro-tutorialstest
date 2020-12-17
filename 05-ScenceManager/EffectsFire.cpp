#include "EffectsFire.h"

EffectsFire::EffectsFire()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(8);
	SetAnimationSet(ani_set);

	this->startTimeAlive();
}

EffectsFire::~EffectsFire()
{
}

void EffectsFire::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void EffectsFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->checkDisApearEffect())
	{
		setObjDisappear();
	}
}



void EffectsFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
