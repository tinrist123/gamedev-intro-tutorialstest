#include "MarioBullet.h"
#include "Ground.h"
#include "ColorBox.h"
#include "Brick.h"
#include "Pipe.h"
#include "Flower.h"
#include "WeakBrick.h"
#include "QuestionBrick.h"




void MarioBullet::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}
void MarioBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isBoundingBox)
	{
		l = r;
		t - b;
	}
	else
	{
		l = x;
		t = y;
		r = x + BULLET_BBOX;
		b = y + BULLET_BBOX;
	}

}

void MarioBullet::SetUpAddingEffectCollision()
{
	this->subHealth();

	pointCollisionX = x;
	pointCollisionY = y;

	isBoundingBox = false;
	isDisappeared = true;
}

void MarioBullet::SetState(int state)
{
	CGameObject::SetState(state);


	switch (state)
	{
	case BULLET_STATE_NORMAL_MOVING:
		vy = ny * BULLET_SPEED_Y;
		vx = nx * BULLET_SPEED_X;
		break;
	case 2:
		vy = BULLET_START_SPEED_Y;
		break;
	case 3:
		vy = BULLET_START_SPEED_Y;
		break;
	default:
		break;
	}
}

void MarioBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vy += MARIO_GRAVITY * dt;

	// turn off collision when die 
	if (state != 4) CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
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
					vy = -0.15;
				}
			}
			if (dynamic_cast<ColorBox *>(e->obj))
			{
				if ( e->nx != 0)
				{
					x += vx * dt;
				}
				else if (e->ny < 0)
				{
					vy = -0.15;
				}
			}
			else if (dynamic_cast<CBrick *>(e->obj))
			{
				if (e->nx != 0)
				{
					SetUpAddingEffectCollision();
				}
				else if (e->ny < 0)
				{
					vy = -0.15;
				}
			}
			else if (dynamic_cast<Pipe *>(e->obj))
			{
				if (e->nx != 0)
				{
					SetUpAddingEffectCollision();
				}
			}
			else if (dynamic_cast<CFlower *>(e->obj))
			{
				if (e->ny != 0 || e->nx != 0)
				{
					SetUpAddingEffectCollision();
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

