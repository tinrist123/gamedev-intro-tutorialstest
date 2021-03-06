#include "QuestionBrick.h"
#include "Item.h"
#include "Utils.h"

QuestionBrick::QuestionBrick() : CBrick()
{
	this->type = Type::QUESTIONBRICK;
}

QuestionBrick::QuestionBrick(int start_x,int start_y,int width, int height,int ItemInside) : CBrick(width,height)
{
	this->ItemInside = ItemInside;
	this->width = width;
	this->height = height;
	this->start_x = start_x;
	this->start_y = start_y;

	this->type = Type::QUESTIONBRICK;
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
		if (y == start_y) return;
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
		ani = BRICK_ANI_EMPTY;
		this->subHealth();
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