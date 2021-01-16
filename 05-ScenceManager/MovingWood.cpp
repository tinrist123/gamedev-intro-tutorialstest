#include "MovingWood.h"

MovingWood::MovingWood()
{
	this->type = Type::MOVINGWOOD;
	this->category = Category::BRICK;
	this->SetState(MOVING_WOOD_STATE_MOVING_HORIZONTAL);
}

MovingWood::~MovingWood()
{
}

void MovingWood::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MOVING_WOOD_STATE_MOVING_HORIZONTAL:
		//vx = -MOVING_WOOD_SPEED_X;
		break;
	case MOVING_WOOD_STATE_FALLING:

		break;
	default:
		break;
	}
}

void MovingWood::Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj)
{
	CGameObject::Update(dt, staticObj);

	if (state == MOVING_WOOD_STATE_MOVING_HORIZONTAL)
	{
		if (x - ((CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth())) < 50)
		{
			vx = -MOVING_WOOD_SPEED_X;
		}
	}
	else if (state == MOVING_WOOD_STATE_FALLING)
	{
		vy += MARIO_GRAVITY * dt;
	}
	x += dx;
	y += dy;
}

void MovingWood::Render()
{
	animation_set->at(0)->Render(x, y);
}

void MovingWood::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + BBOX_BIT * 3;
	top = y;
	bottom = y + BBOX_BIT;

}
