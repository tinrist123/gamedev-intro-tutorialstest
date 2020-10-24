#include "QuestionBrick.h"
#include "Item.h"
#include "Utils.h"

QuestionBrick::QuestionBrick(int start_x,int start_y,int width, int height) : CBrick(width,height)
{
	this->width = width;
	this->height = height;
	this->start_x = start_x;
	this->start_y = start_y;
	ani = 0;
}

void QuestionBrick::Render()
{
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	 
	x += dx;
	y += dy;
	if (y >= start_y)
	{
		y = start_y;
		vy = 0;
	}
	else if (y < start_y)
	{
		vy += GRAVITY * dt;
	}
}

void QuestionBrick::SetState(int state)
{
	switch (state)
	{
	case QUESTION_BRICK_AVAILABLE:
		vy = -QUESTION_BRICK_SPEED_X;
		this->subHealth();
		ani = BRICK_ANI_EMPTY;
		break;
	}
}

void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH * width;
	b = y + BRICK_BBOX_HEIGHT * height;
	
}