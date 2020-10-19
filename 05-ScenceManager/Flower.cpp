#include "Flower.h"
#include "Utils.h"


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
	CGameObject::SetState(state);


	switch (state)
	{
	case FLOWER_STATE_UP:
		isDelayedShooting = true;
		vy = -0.15f;
		break;
	case FLOWER_STATE_DOWN:
		vy = 0;
		break;
	default:
		break;
	}
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);


	x += dx;
	y += (y == 336.0f && state == FLOWER_STATE_UP) ? 0 : dy;
	vy += 0.0002f * dt;



	if (setInterval == 0)
	{
		setInterval = GetTickCount64();
		setInterval2 = 0;
		if (state == FLOWER_STATE_DOWN || state == -1) SetState(FLOWER_STATE_UP);
	}
	else if (GetTickCount64() - setInterval > 3000)
	{
		if (state == FLOWER_STATE_UP) SetState(FLOWER_STATE_DOWN);
		if (setInterval2 == 0) setInterval2 = GetTickCount64();
		if (GetTickCount64() - setInterval2 > 3000)
		{
			setInterval = 0;
		}
	}

	if (y <= 336.0f)
	{
		y = 336.0f;
	}
	if (y >= 384.0f)
	{
		y = 384.0f;
	}
	
}