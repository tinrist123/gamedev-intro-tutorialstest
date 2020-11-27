#include "Item.h"


Item::Item(int typeItem,int kindAni,int x ,int y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(5);
	SetAnimationSet(ani_set);
	this->typeItem = typeItem;
	this->kindAni = kindAni;
	this->start_x = x;
	this->start_y = y;
}

void Item::Render()
{
	if (this->typeItem == SPECIAL_ITEM)
	{
		if (this->kindAni == 2) ani = ITEM_ANI_MUSHROOM;
		else ani = ITEM_ANI_LEAF;
	}
	else if (this->typeItem == ITEM_COIN)
	{
		if (this->kindAni == 1) ani = ITEM_ANI_COIN;
	}
	if (pointEff)
	{
		pointEff->Render();
	}
	else if (!pointEff)
	{
		animation_set->at(ani)->Render(x,y,255); 
	}
	//RenderBoundingBox();
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (pointEff)
	{
		pointEff->Update(dt, coObjects);
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
	vy += GRAVITY * dt;
	// No collision occured, proceed normally

	if (coEvents.size() == 0)
	{
		if (state == ITEM_STATE_SPECIAL_ITEM)
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
			y += dy;
			x += dx;
		}

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
		x += min_tx * dx + nx * 0.1f;	
		y += min_ty * dy + ny * 0.1f;
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
			if (dynamic_cast<Ground *>(e->obj))
			{
				if (e->ny < 0)
				{
					if (state == ITEM_STATE_COIN_JUMP)
					{
						addPointToItem();
					}
					else
					{
						vy = START_SPEED_Y;
						isOnGround = true;
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Item::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state) {
	case ITEM_STATE_SPECIAL_ITEM:
		vy = -MUSHROOM_SPEED_Y;
		break;
	case ITEM_STATE_HITTING_MARIO:
		setObjDisappear();
		break;
	case ITEM_STATE_COIN_JUMP:
		vy = -0.2f;
		break;
	default:
		break;
	}
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isBoundingBox)
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

void Item::addPointToItem()
{
	pointEff = new EffectPoint();
	pointEff->SetPosition(this->x, this->y);
}


