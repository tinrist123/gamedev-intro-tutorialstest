#include "Flower.h"


void CFlower::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
void CFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y  + FLOWER_BBOX_HEIGHT;
}

void CFlower::SetState(int state)
{
	
}

void CFlower::Update(DWORD dt)
{
	CGameObject::Update(dt);

	if (setInterval == 0)
	{
		setInterval = GetTickCount64();
	}
}