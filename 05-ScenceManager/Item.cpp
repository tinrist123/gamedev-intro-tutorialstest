#include "Item.h"
#include "Utils.h"

Item::Item(int width, int height, int typeitem, int x, int y)
{
	this->width = width;
	this->height = height;
	this->typeItem = typeitem;
	this->start_x = x;
	this->start_y = y;
}

void Item::Render()
{
	if (true)
	{
		int ani = 0;
		if (this->typeItem == 1) ani = 1;
		else ani = 2;
		animation_set->at(ani)->Render(x, y);
	}
	RenderBoundingBox();
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	x += dx;
	y += dy;
	if (y >= start_y)
	{
		y = start_y;
	}
	else if (y < start_y)
	{
		vy += 0.00021875f * dt;
	}
}

void Item::SetState(int state)
{
	switch (state) {
	case 1 :
		DebugOut(L"Imhere\n");
		vy = -0.2f;
	}
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}


