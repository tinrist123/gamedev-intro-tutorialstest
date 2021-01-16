#include "BomerangWeapon.h"

BoomerangWeapon::BoomerangWeapon()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(18);
	this->SetAnimationSet(ani_set);

	this->category = Category::ENEMY;
	this->type = Type::BOOMERANG;
}

void BoomerangWeapon::storeAmplitydeOy(float y)
{
	this->amplitudeOy = y;
}

void BoomerangWeapon::reduceTempVelocity()
{
	if (tempVx > 0)
	{
		tempVx -= DECELERATE_SPEED;
		if (tempVx <= 0) tempVx = 0;
	}
	else if (tempVx < 0)
	{
		tempVx += DECELERATE_SPEED;
		if (tempVx >= 0) tempVx = 0;
	}
}

void BoomerangWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	if (!this->isTurningBack)
	{
		reduceTempVelocity();
		if (tempVx == 0)
		{
			//vx = -vx;
			if (!this->isPreTurningBack)
			{
				tempVx = INERTIA_SPEED_X;
				this->isPreTurningBack = true;
				this->SetState(BOOMERANG_STATE_MOVE_VERTICAL);
				this->isTouchable = true;
			}
		}

		if (y >= this->amplitudeOy + 14 && this->isCurvedBack)
		{
			this->SetState(BOOMERANG_STATE_MOVE_NO_GRAVITY);
			this->vy = 0;
			this->isTurningBack = true;
		}
		if (y >= this->amplitudeOy && !this->isCurvedBack)
		{
			if (tempVx == 0 && this->isPreTurningBack)
			{
				vx = -vx;
				this->isCurvedBack = true;
			}
		}
	}

	if (this->GetState() == BOOMERANG_STATE_MOVE_VERTICAL)
	{
		vy += 0.00003f * dt;
	}
	
	

	x += dx;
	y += dy;
}

void BoomerangWeapon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_STATE_MOVE_HORIZONTAL:
		isAimToStatus = false;
		vx = BOOMERANG_SPEED_X * nx;
		vy = -BOOMERANG_SPEED_Y;
		tempVx = vx;
		break;
	case BOOMERANG_STATE_MOVE_VERTICAL:
		
		break;
	case BOOMERANG_STATE_MOVE_NO_GRAVITY:
		break;
	default:
		break;
	}

}

void BoomerangWeapon::Render()
{
	if (this->GetState() == BOOMERANG_STATE_PREPARING_THROWING)
	{
		if (nx == 1)
		{
			ani = BOOMERANG_ANI_BOOMERANG_PREPARING_THROWING_RIGHT;
		}
		else if (nx == -1)
		{
			ani = BOOMERANG_ANI_BOOMERANG_PREPARING_THROWING_LEFT;
		}
	}
	else
	{
		ani = BOOMERANG_ANI_BOOMERANG_THROWING;
	}

	animation_set->at(ani)->Render(x, y);
}

void BoomerangWeapon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_BIT;
	b = y + BBOX_BIT;
}
