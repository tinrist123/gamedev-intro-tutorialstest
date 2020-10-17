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
}

void Item::Render()
{
	if (this->typeItem)
	{
		int ani = 0;
		if (this->typeItem == 1) ani = 1;
		else ani = 2;
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
	if (state != 4)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	vy += 0.0006f * dt;
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
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
			vy = 0;
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
					vy = 0;
					isOnGround = true;
				}
			}
			/*if (dynamic_cast<CMario *>(e->obj))
			{
				if (e->nx != 0)
				{
					SetState(2);
				}

			}*/
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Item::SetState(int state)
{
	switch (state) {
	case 1 :
		vy = -0.1f;
		vx = 0.1f;
		break;
	case 2 :
		isDisappeared = true;
		subHealth();
		break;
	default:
		break;
	}
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}


