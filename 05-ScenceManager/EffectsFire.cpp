#include "EffectsFire.h"

EffectsFire::EffectsFire()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(8);
	SetAnimationSet(ani_set);

}

EffectsFire::~EffectsFire()
{
}

void EffectsFire::Render()
{
	animation_set->at(0)->Render(x, y);
}

void EffectsFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (animation_set->at(0)->IsRenderOver(300))
	{
		setObjDisappear();
	}
}

void EffectsFire::SetState(int state)
{
	if (state == EFFECTS_DEFLECT)
	{
		animation_set->at(0)->StartRenderAnimation();
	}
}

void EffectsFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r;
	t = b;
}
