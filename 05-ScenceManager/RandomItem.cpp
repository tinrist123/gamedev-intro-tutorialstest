#include "RandomItem.h"

void RandomItem::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_MOVE_UP:
		vy = -MOVE_UP_SPEED;
		RandomizeItem();
		this->isRandomizeState = false;
		break;
	default:
		break;
	}
}

RandomItem::RandomItem(float x, float y) :  Item()
{
	this->type = Type::RANDOMITEM;
	this->start_x = x;
	this->start_y = y;
	this->isTouchable = true;
}

RandomItem::RandomItem()
{
}

void RandomItem::Render()
{
	if (isRandomizeState)
	{
		ani = ITEM_ANI_RANDOMIZE_END_GAME;
	}
	else if (!isRandomizeState)
	{
		if (this->typeItemCollected == ITEM_END_GAME_TYPE_MUSHROOM)
			ani = ITEM_ANI_MUSHROOM_END_GAME;
		else if (this->typeItemCollected == ITEM_END_GAME_TYPE_FLOWER)
			ani = ITEM_ANI_FLOWER_END_GAME;
		else if (this->typeItemCollected == ITEM_END_GAME_TYPE_STAR)
			ani = ITEM_ANI_STAR_END_GAME;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void RandomItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	x += dx;
}

void RandomItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_BIT;
	b = y + BBOX_BIT;
}
