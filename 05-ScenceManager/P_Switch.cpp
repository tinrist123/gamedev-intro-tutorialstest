#include "P_Switch.h"

P_Switch::P_Switch(float posX, float posY)
{
	this->start_x = posX;
	this->start_y = posY;

	this->type = Type::P_SWITCH;
}

P_Switch::P_Switch()
{
}

P_Switch::~P_Switch()
{
}

void P_Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	if (y <= start_y - 16)
	{
		y = start_y - 16;
		//this->isTouchable = true;
	}
	y += dy;
	x += dx;
}

void P_Switch::Render()
{
	if (!isActived)
	{
		animation_set->at(PSWITCH_ANI_UP)->Render(x, y, 255);
	}
	else {
		animation_set->at(PSWITCH_ANI_CRUSHED)->Render(x, y, 255);
	}
	RenderBoundingBox();
}

void P_Switch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + BBOX_BIT;
	if (!isActived)
	{
		top = y;
		bottom = y + BBOX_BIT;
	}
	else
	{
		top = y + BBOX_BIT / 2;
		bottom = y + BBOX_BIT ;
	}
}
