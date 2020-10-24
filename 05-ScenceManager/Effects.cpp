#include "Effects.h"

void Effects::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Effects::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (animation_set->at(0)->IsRenderOver(400))
	{
		subHealth();
		isDisappeared = true;
	}
}

void Effects::SetState(int state)
{
	if (state == EFFECTS_DEFLECT)
	{
		animation_set->at(0)->StartRenderAnimation();
	}
}

void Effects::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r;
	t = b;
}
