#include "BoomerangBrother.h"
#include "Ground.h"
#include "ColorBox.h"

void BoomerangBrother::CollideWithBoomerang(vector<LPGAMEOBJECT>* listFireBall)
{
	for (size_t i = 0; i < listFireBall->size(); i++)
	{
		if (this->AABBCollision(listFireBall->at(i)))
		{
			if (listFireBall->at(i)->getTypeObject() == Type::BOOMERANG
				&& listFireBall->at(i)->isTouchable
				)
			{
				//if (DelayGetBoomerangTime->IsTimeUp())
				{
					//DelayGetBoomerangTime->Stop();
					listFireBall->at(i)->setObjDisappear();
					this->countBoomerangWeapon--;
				}
			}
		}
	}
}

void BoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_WALKING_OR_ATTACKING:
		vx = BOOMERANG_BROTHER_WALKING_SPEED_X *nx;
		break;
	case BOOMERANG_BROTHER_STATE_PREPARING_ATTACK:
		vx = BOOMERANG_BROTHER_WALKING_SPEED_X * nx;
		break;
	case BOOMERANG_BROTHER_STATE_DEATH:
		vy = -BOOMERANG_BROTHER_JUMP_DIE;
		this->isBoundingBox = false;
		break;
	default:
		break;
	}
}

void BoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isBoundingBox) left = top = right = bottom;
	else
	{
		left = x;
		top = y;
		right = x + BBOX_BIT;
		bottom = y + BBOX_BIT + BBOX_BIT / 2;
	}
}

void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (pointEff)
	{
		pointEff->UpdateOrDeleteEffect(dt, coObjects, pointEff);
	}

	CGameObject::Update(dt, coObjects);

	if (loadingShootingTime->IsTimeUp() && loadingShootingTime->isStarted)
		isCanHoldingBoomerang = false;

	if (countBoomerangWeapon == 1)
	{
		getAllBoomerang =  true;
	}

	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != 44)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally

	distanceMoving += fabs(dx);

	if (distanceMoving >= BOOMERANG_BROTHER_MAXIMUM_RANGE)
	{
		this->vx = -this->vx;
		distanceMoving = 0;
	}

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
		/*if (rdx != 0 && rdx!=dx)
			x += nx*abs(rdx); */

			// block every object first!
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (ny != 0) {
			vy = 0;
		}

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
					this->isOnGround = true;
					vy = 0;
				}
				if (e->nx != 0)
				{
					this->nx = -this->nx;
					this->vx = -this->vx;
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += vx * dt;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BoomerangBrother::Render()
{
	if (state == BOOMERANG_BROTHER_STATE_WALKING_OR_ATTACKING)
	{
		if (nx == 1)
		{
			ani = BOOMERANG_BROTHER_ANI_WALKING_RIGHT;
		}
		else if (nx == -1)
		{
			ani =  BOOMERANG_BROTHER_ANI_WALKING_LEFT;
		}
	}
	else if (state == BOOMERANG_BROTHER_STATE_PREPARING_ATTACK)
	{
		if (nx == 1)
		{
			ani = BOOMERANG_BROTHER_ANI_PREPARE_ATTACK_RIGHT;
		}
		else if (nx == -1)
		{
			ani = BOOMERANG_BROTHER_ANI_PREPARE_ATTACK_LEFT;
		}
	}
	else if (state == BOOMERANG_BROTHER_STATE_DEATH)
	{
		if (nx == 1)
		{
			ani = BOOMERANG_BROTHER_ANI_DEATH_RIGHT;
		}
		else if (nx == -1)
		{
			ani = BOOMERANG_BROTHER_ANI_DEATH_LEFT;
		}
	}

	if (pointEff)
	{
		pointEff->Render();
	}

	animation_set->at(ani)->Render(x, y);
}
BoomerangBrother::BoomerangBrother()
{
	this->type = Type::BOOMERANGBROTHER;
	this->category = Category::ENEMY;
	
	this->SetState(BOOMERANG_BROTHER_STATE_WALKING_OR_ATTACKING);
	this->nx = -1;
}

BoomerangBrother::~BoomerangBrother()
{
}
