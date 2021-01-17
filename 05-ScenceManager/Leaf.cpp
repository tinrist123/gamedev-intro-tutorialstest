#include "Leaf.h"

Leaf::~Leaf()
{
}
void Leaf::Render()
{
	// 2 mean animation Leaf
	if (vx > 0)
	{
		animation_set->at(ITEM_ANI_LEAF_RIGHT)->Render(x, y, 255);
	}
	else if (vx < 0)
	{
		animation_set->at(ITEM_ANI_LEAF_LEFT)->Render(x, y, 255);
	}

	//RenderBoundingBox();
}
void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	float Gra = GRAVITY / 16;
	if (vy > 0)
	{
		if (!isTouchable) this->isTouchable = true;
		vy += Gra * dt;
		if (vy >= 0.015f)

		{
			vy = 0.015f;
		}
	}
	else 
		vy += GRAVITY * dt;

	if (x >= RightLimit)
	{
		vx = -fabs(vx);
	}
	else if (x <= LeftLimit)
	{
		vx = fabs(vx);
	}


}
void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; 
	t = y;
	r = x + BBOX_BIT;
	b = y + BBOX_BIT;
}
Leaf::Leaf(float posX, float posY)
{
	this->type = Type::SUPER_LEAF;

	this->start_x = posX;
	this->start_y = posY;

	LeftLimit = posX;
	RightLimit = posX + 32.0f;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(5);
	SetAnimationSet(ani_set);
}
