#include "FireBullet.h"
#include "Ground.h"




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
		vy = ny*BULLET_SPEED_Y;
		vx = nx*BULLET_SPEED_X;
		break;
	default:
		break;
	}
}

FireBullet::FireBullet()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(7);
	SetAnimationSet(ani_set);
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
}
