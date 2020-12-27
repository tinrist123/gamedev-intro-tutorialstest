#include "Flower.h"


CFlower::CFlower(int x ,int y,int HeightPipe,int typeFlower) : Enemy()
{
	this->type = Type::FLOWER;
	this->start_y = y;
	this->start_x = x;
	this->HeightPipe = HeightPipe;
	this->typeFlower = typeFlower;
}

void CFlower::Render()
{
	if (this->typeFlower == FLOWER_TYPE_RED_FLOWER_FIRE)
	{
		if (isShooting)
		{
			if (ny == -1 && nx == 1) ani = FLOWER_ANI_UP_OPEN_MOUTH_RIGHT;
			else if (ny == -1 && nx == -1) ani = FLOWER_ANI_UP_OPEN_MOUTH_LEFT;
			else if ((ny == 1 || ny == 0) && nx == 1) ani = FLOWER_ANI_DOWN_OPEN_MOUTH_RIGHT;
			else if ((ny == 1 || ny == 0) && nx == -1) ani = FLOWER_ANI_DOWN_OPEN_MOUTH_LEFT;
		}
		else
		{
			if (nx == -1) ani = FLOWER_ANI_UP_MOVE_OPEN_MOUTH_LEFT;
			else if (nx == 1) ani = FLOWER_ANI_UP_MOVE_OPEN_MOUTH_RIGHT;
		}
	}
	else if (this->typeFlower == FLOWER_TYPE_GREEN_FLOWER_FIRE)
	{
		if (isShooting)
		{
			if (ny == -1 && nx == 1) ani =					FLOWER_GREEN_ANI_UP_OPEN_MOUTH_RIGHT;
			else if (ny == -1 && nx == -1) ani =			FLOWER_GREEN_ANI_UP_OPEN_MOUTH_LEFT;
			else if ((ny == 1 || ny == 0) && nx == 1) ani = FLOWER_GREEN_ANI_DOWN_OPEN_MOUTH_RIGHT;
			else if ((ny == 1 || ny == 0) && nx == -1) ani =FLOWER_GREEN_ANI_DOWN_OPEN_MOUTH_LEFT;
		}														   
		else													   
		{														   
			if (nx == -1) ani =								FLOWER_GREEN_ANI_UP_MOVE_OPEN_MOUTH_LEFT;
			else if (nx == 1) ani =							FLOWER_GREEN_ANI_UP_MOVE_OPEN_MOUTH_RIGHT;
		}
	}
	else if (this->typeFlower == FLOWER_TYPE_PIRANHA_FLOWER_FIRE)
	{
		ani = FLOWER_GREEN_PIRANHA_ANI_UNFIRE;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FLOWER_BBOX_WIDTH;
	b = y  + FLOWER_BBOX_HEIGHT;

	if (this->typeFlower == FLOWER_TYPE_GREEN_FLOWER_FIRE
		||
		this->typeFlower == FLOWER_TYPE_PIRANHA_FLOWER_FIRE
		)
	{
		t += BBOX_BIT / 2;
	}
}

void CFlower::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FLOWER_STATE_UP:
		isWaitingShooting = true;
		vy = -FLOWER_SHOOT_SPEED_X;
		break;
	case FLOWER_STATE_DOWN:
		isShooting = false;
		isWaitingShooting = false;
		delayBullet = 0;
		vy = 0;
		break;
	case FLOWER_STATE_AIM_TARGET:
		break;
	default:
		break;
	}
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);


	y += (y == start_y - BBOX_BIT * HeightPipe && state == FLOWER_STATE_UP) ? 0 : dy;
	vy += 0.0001 * dt;


	
	if (setInterval == 0)
	{
		setInterval = GetTickCount64();
		setInterval2 = 0;
		// -1 mean STATE DEFAULT
		if (state == FLOWER_STATE_DOWN || state == -1 || state == 3) SetState(FLOWER_STATE_UP);
	}
	else if (GetTickCount64() - setInterval > 3000)
	{
		if (state == FLOWER_STATE_UP)
		{
			SetState(FLOWER_STATE_DOWN);
		}
		if (setInterval2 == 0) setInterval2 = GetTickCount64();
		if (GetTickCount64() - setInterval2 > 2000)
		{
			setInterval = 0;
		}
		
	}

	if (y <= start_y - 16 * HeightPipe)
	{
		y = start_y - 16 * HeightPipe;
	}
	if (y >= start_y)
	{
		y = start_y;
	}
	
}