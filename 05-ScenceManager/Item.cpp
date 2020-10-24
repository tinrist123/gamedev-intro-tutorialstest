#include "Item.h"
#include "Utils.h"
#include "Ground.h"
#include "Mario.h"

Item::Item(int width, int height, int typeitem, int x, int y)
{
	this->width = width;
	this->height = height;
	this->typeItem = typeitem;
	this->start_x = x;
	this->start_y = y;
	this->x = x;
	this->y = y;
	ani = ITEM_ANI_COIN;
}

void Item::Render()
{
	if (this->typeItem)
	{
		if (this->typeItem == 1) ani = ITEM_ANI_MUSHROOM;
		else ani = ITEM_ANI_LEAF;
		animation_set->at(ani)->Render(x,y,255);
	}
	RenderBoundingBox();
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != 4) CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	vy += GRAVITY * dt;
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (y <= start_y - HEIGHT_MUSHROOM_UP && !isGrowthUp)
		{
			y = start_y - HEIGHT_MUSHROOM_UP;
			isGrowthUp = true;
			vy = START_SPEED_Y;
			vx = MUSHROOM_SPEED_X *nx;
		}
		else
		{
			y += dy;
			x += dx;
		}

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;	
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = START_SPEED_Y;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground *>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = START_SPEED_Y;
					isOnGround = true;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Item::SetState(int state)
{
	switch (state) {
	case 1 :
		vy = -MUSHROOM_SPEED_Y;
		
		break;
	case 2 :
		isDisappeared = true;
		subHealth();
		break;
	case 3 :
		tested = true;
		break;
	default:
		break;
	}
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (tested)
	{
		return;
	}
	else {
		l = x;
		t = y;
		r = x + ITEM_BBOX_WIDHT;
		b = y + ITEM_BBOX_HEIGHT;
	}
	
}


