#include "Coin.h"
#include "Brick.h"



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

	//if (state != ITEM_STATE_HITTING_MARIO)
	//	CalcPotentialCollisions(coObjects, coEvents);

	if (this->y < start_y - 16)
	{
		this->isTouchable = true;
	}


	if (state == ITEM_STATE_COIN_JUMP)
	{
		if (isBoundingBox) 
			isBoundingBox = false;
		
		vy += (GRAVITY + 0.0002f)* dt;

		if (this->isTouchable)
		{
			if (this->y > start_y - 16)
			{
				addPointToItem();
			}
		}
		
	}

	y += dy;

}

void Coin::Render()
{
	if (pointEff)
	{
		pointEff->Render();
	}
	else
	{
		if (this->GetState() == ITEM_STATE_IDLE_COIN)
		{
			ani = ITEM_ANI_IDLE_COIN;
		}
		animation_set->at(ani)->Render(x, y);
	}
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

