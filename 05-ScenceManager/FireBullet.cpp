#include "FireBullet.h"




void FireBullet::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}
void FireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isBoundingBox)
	{
		l = r;
		t - b;
	}
	else
	{
		l = x;
		t = y;
		r = x + 8;
		b = y + 8;
	}
	
}

void FireBullet::SetState(int state)
{
	CGameObject::SetState(state);


	switch (state)
	{
	case 1:
		vy = 0.015*ny;
		vx = 0.015*nx;
		break;
	case 2:
		vy = 0;
		break;
	default:
		break;
	}
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
}
