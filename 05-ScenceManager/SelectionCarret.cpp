#include "SelectionCarret.h"

SelectionCarret::SelectionCarret(CMario* mario)
{
	this->mario = mario;
}

SelectionCarret::~SelectionCarret()
{
}

void SelectionCarret::Render()
{
	animation_set->at(0)->Render(x, y);
}

void SelectionCarret::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}

void SelectionCarret::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	mario->isWalking = false;
	if (start_x == 0)
		start_x = x;
	if (start_y == 0)
		start_y = y;
	if (mario->GetState() == MARIO_SELECTION_MAP_STATE_MOVE_TOP)
	{
		
		SetPosition(80, 145);
	}
	else if (mario->GetState() == MARIO_SELECTION_MAP_STATE_MOVE_BOTTOM)
	{
		SetPosition(80, 160);
	}
	CGameObject::Update(dt, coObjects);
}

void SelectionCarret::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	default:
		break;
	}

}
