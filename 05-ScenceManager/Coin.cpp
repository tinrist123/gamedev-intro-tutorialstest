#include "Coin.h"



Coin::Coin() : Item(1,1,1,1)
{
}

Coin::~Coin()
{
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Coin::Render()
{
	animation_set->at(3)->Render(x, y);
	//RenderBoundingBox();
}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isBoundingBox)
	{
		l = r;
		t = b;
	}
	else
	{
		l = x;
		t = y;
		r = x + BBOX_BIT + 1.0f;
		b = y + BBOX_BIT + 1.0f;
	}
}