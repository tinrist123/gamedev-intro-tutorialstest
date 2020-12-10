#include "Coin.h"



Coin::Coin(float posX, float posY)
{
	this->type = Type::COIN;
	this->start_x = posX;
	this->start_y = posY;
	this->ani = ITEM_ANI_COIN;
}

Coin::Coin()
{
	this->isTouchable = true;
	this->type = Type::COIN;
	this->ani = ITEM_ANI_COIN_IDLE;
}

Coin::~Coin()
{
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (pointEff)
	{
		pointEff->Update(dt,coObjects);
		return;
	}
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 
	if (state != ITEM_STATE_HITTING_MARIO)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	//vy += GRAVITY* dt;
	// No collision occured, proceed normally

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 
		// block every object first!
		//x += min_tx * dx + nx * 0.1f;
		//y += min_ty * dy + ny * 0.1f;
		/*if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = START_SPEED_Y;
		}*/
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny < 0)
				{
					if (state == ITEM_STATE_COIN_JUMP)
					{
						addPointToItem();
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Coin::Render()
{
	if (pointEff)
	{
		pointEff->Render();
	}
	else
	{
		animation_set->at(ani)->Render(x, y);
	}
	RenderBoundingBox();
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

