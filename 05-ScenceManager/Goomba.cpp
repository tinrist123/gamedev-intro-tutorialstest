#include "Goomba.h"
#include "Ground.h"
#include "ColorBox.h"
#include "Pipe.h"
#include "Utils.h"
CGoomba::CGoomba(float x ,float y,int typeGoomba)
{
	this->start_x = x;
	this->start_y = y;

	this->type = Type::GOOMBA;
	this->level = typeGoomba;
	if (this->level == PARAGOOMBA)
	{
		SetState(GOOMBA_STATE_WALKING_WITH_WING);
		health++;
		//SetState(GOOMBA_STATE_DIE);
	}
	else if (this->level == GOOMBA_LEVEL_BROWN)
	{
		SetState(GOOMBA_STATE_WALKING);
	}

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isBoundingBox)
	{
		left = right;
		top = bottom;
	}
	else {
		left = x;
		right = x + GOOMBA_BBOX_WIDTH_BROWN_GOOMBA;
		top = y;
		bottom = y + GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_DISTANCE_MAX_HEIGHT;
		if (this->level == PARAGOOMBA)
		{
			top = y + GOOMBA_BBOX_DISTANCE_MAX_HEIGHT - 1;
			bottom = y + GOOMBA_BBOX_HEIGHT;
			right = x + GOOMBA_BBOX_WIDTH ;
		}
		if (state == GOOMBA_STATE_DIE)
		{
			bottom = y + GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_DISTANCE_MAX_HEIGHT;
		}
	}
}
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (pointEff)
	{
		pointEff->UpdateOrDeleteEffect(dt, coObjects, pointEff);
	}


	CGameObject::Update(dt, coObjects);


	if (state == GOOMBA_STATE_DIE)
	{
		if (this->timeWaitingDeath == 0)
		{
			this->setTimeWaitingDeath();
		}
		else if (checkTimeWaitingDeath())
		{
			setObjDisappear();
		}
	}

	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;

	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (health == 2)
	{
		if (changeStateTo_MiniJumpGoomba() && state != GOOMBA_STATE_PRE_JUMPING)
		{
			SetState(GOOMBA_STATE_PRE_JUMPING);
		}
		else if (changeStateTo_MaxJumpGoomba() && state != GOOMBA_STATE_JUMPING)
		{
			SetState(GOOMBA_STATE_JUMPING);
			resetTimeChangeState();
		}
	}


	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
		if (x < 0)
		{
			x = 1;
			nx = -nx;
			vx = -vx;
		}
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
					if (state == GOOMBA_STATE_PRE_JUMPING)
					{
						vy = -GOOMBA_STATE_MINI_JUMPING_VY;
					}
					else if (state == GOOMBA_STATE_JUMPING)
					{
						if (!firstTimeHighJump)
						{
							vy = -GOOMBA_STATE_JUMPING_VY;
							firstTimeHighJump = true;
						}
						else if (firstTimeHighJump)
						{
							firstTimeHighJump = false;
							SetState(GOOMBA_STATE_WALKING_WITH_WING);
							blockingChangeState(false);
						}
					}
					else {
						vy = 0;
					}
				}
				else if (e->nx != 0)
				{
					vx = -vx;
					
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += vx * dt;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
				if (e->nx != 0)
				{
					vx = -vx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	if (this->level == PARAGOOMBA)
	{
		if (state == GOOMBA_STATE_WALKING)
		{
			ani = GOOMBA_RED_ANI_WALKING;
		}
		else  if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_RED_ANI_DIE;
		}
		else if (state == GOOMBA_STATE_JUMPING || state == GOOMBA_STATE_PRE_JUMPING)
		{
			ani = GOOMBA_RED_ANI_JUMPING;
		}
		else if (state == GOOMBA_STATE_WALKING_WITH_WING)
		{
			ani = GOOMBA_RED_ANI_WALKING_WING;
		}
		else if (state == GOOMBA_STATE_HIT_BY_WEAPON)
		{
			ani = GOOMBA_ANI_PARAGOOMBA_SUPINE;
		}
	}
	else if (this->level == GOOMBA_LEVEL_BROWN)
	{
		if (state == GOOMBA_STATE_WALKING)
		{
			ani = GOOMBA_ANI_WALKING;
		}
		else  if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_DIE;
		}
		else if (state == GOOMBA_STATE_HIT_BY_WEAPON)
		{
			ani = GOOMBA_ANI_BROWN_GOOMBA_SUPINE;
		}
	}


	animation_set->at(ani)->Render(x,y);

	if (pointEff)
	{
		pointEff->Render();
	}

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_HIT_BY_WEAPON:
			vy = -0.2;
			isBoundingBox = false;
			break;
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = nx * GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_WALKING_WITH_WING:
			vx = nx * GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_PRE_JUMPING:
			//vy = -0.1;
			break;
		case GOOMBA_STATE_JUMPING:
			//vy = -0.3;
			break;
	}
}


