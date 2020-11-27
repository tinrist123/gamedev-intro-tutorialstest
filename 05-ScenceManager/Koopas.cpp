#include "Koopas.h"
#include "Utils.h"
#include "Ground.h"
#include "ColorBox.h"
#include "Pipe.h"
#include "MarioWeapon.h"
#include "QuestionBrick.h"

CKoopas::CKoopas(int typeKoopas, int typeColorKoopas)
{
	this->TypeKoopas = typeKoopas;
	this->TypeColorKoopas = typeColorKoopas;
	nx = 1;
	autoLoadAni();
	switch (this->TypeKoopas)
	{
	case KOOPAS_TYPE_HAVE_WING:
		SetState(KOOPAS_STATE_HAVE_WING_FLYING);
		health++;
		break;
	case KOOPAS_TYPE_NORMAL:
		SetState(KOOPAS_STATE_WALKING);
		break;
	case KOOPAS_TYPE_HAVE_WING_WALKING:
		SetState(KOOPAS_STATE_HAVE_WING_WALKING);
		health++;
		break;
	default:
		break;
	}
	health++;
	boxCalcFutureRight = new DetectFuture();
	boxCalcFutureRight->UpdatePosition(x+20 , y);
	boxCalcFutureLeft = new DetectFuture();
	boxCalcFutureLeft->UpdatePosition(x - 20, y);
}


void CKoopas::autoLoadAni()
{
	if (this->TypeColorKoopas == KOOPAS_GREEN_TYPE)
	{
		AddAniForKoopasGreenToList();
	}
	else if (this->TypeColorKoopas == KOOPAS_RED_TYPE)
	{
		AddAniForKoopasRedToList();
	}
}



void CKoopas::ExecuteRevingKoopas()
{
	if (state == KOOPAS_STATE_PRE_REVIE)
	{
		if (CheckTimeKoopasPreReive_2())
		{
			SetState(KOOPAS_STATE_PRE_REVIE_2);
		}
	}
	if (state == KOOPAS_STATE_PRE_REVIE_2)
	{
		if (CheckTimeKoopasRevive())
		{
			SetState(KOOPAS_STATE_WALKING);
			IncreHealth();
			resetTimeToRevie();
		}
	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	if (state == KOOPAS_ANI_SHELL_OVERTURNED_MOVING
		||
		state == KOOPAS_ANI_SHELL_OVERTURNED
		||
		state == KOOPAS_ANI_SHELL_MOVING
		||
		state == KOOPAS_ANI_SHELL_OVERTURNED_HEALTH
		||
		state == KOOPAS_STATE_PRE_REVIE
		||
		state == KOOPAS_STATE_PRE_REVIE_2
		||
		state == KOOPAS_STATE_SHELL_OUT_CONTROL
		)
	{
		top = y + 9;
	}
	else{
		top = y;
	}
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT - 2;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	if (effectPoint )
	{
		effectPoint->UpdateOrDeleteEffect(dt, coObjects, effectPoint);
	}

	if (isOutOfControl)
	{
		x = marioPositionX;
		y = marioPositionY;
		vy = 0;
	}
	else
	{
		CGameObject::Update(dt, coObjects);
	}


	// KOOPAS REVIVE
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_OUT_CONTROL)
	{
		if (CheckTimeKoopasPreReive())
		{
			SetState(KOOPAS_STATE_PRE_REVIE);
		}
	}
	ExecuteRevingKoopas();

	// Detect Koopas in next Update to calc whether koopas fall
	boxCalcFutureRight->UpdatePosition(x + 16, y + KOOPAS_BBOX_HEIGHT);
	boxCalcFutureLeft->UpdatePosition(x, y + KOOPAS_BBOX_HEIGHT);

	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
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
			if (dynamic_cast<MarioWeapon*>(e->obj))
			{
				if (e->nx != 0 || e->ny != 0)
				{
					DebugOut(L"Hitting Bullet\n");
				}
			}
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (!boxCalcFutureRight->AABBCollision(e->obj))
				{
					vx = -fabs(vx);
					this->nx = -1;
				}
				else if (!boxCalcFutureLeft->AABBCollision(e->obj))
				{
					vx = fabs(vx);
					this->nx = 1;
				}
				if (e->ny < 0)
				{
					if (state == KOOPAS_STATE_HAVE_WING_FLYING)
					{	
						vy = -0.2f;
					}
					else {
						vy = 0;
					} 
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += vx * dt;
				}
				if (e->ny < 0)
				{
					vy = 0;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
				if (e->nx != 0)
				{
					vx = -vx;
					this->nx = -this->nx;
				}
			}
			else if (dynamic_cast<QuestionBrick *>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CKoopas::AddAniForKoopasGreenToList()
{
	listAnimationKoopas.clear();
	listAnimationKoopas.push_back(KOOPAS_ANI_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_ANI_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_FLYING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_FLYING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_ANI_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_ANI_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_MOVING);
}

void CKoopas::AddAniForKoopasRedToList()
{
	listAnimationKoopas.clear();
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_WALKING_LEFT);
	/*listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_FLYING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_FLYING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_WALKING_RIGHT);*/
	listAnimationKoopas.push_back(0);
	listAnimationKoopas.push_back(0);
	listAnimationKoopas.push_back(0);
	listAnimationKoopas.push_back(0);
	// ADD SPRITE NGU NEN BI DAO THU TU
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_MOVING);
}
void CKoopas::Render()
{
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0) ani = listAnimationKoopas.at(0);
		else if (vx < 0) ani = listAnimationKoopas.at(1);
	}
	else if (state == KOOPAS_STATE_HAVE_WING_FLYING)
	{
		if (nx == -1)
			ani = listAnimationKoopas.at(2);
		else if (nx == 1)
			ani = listAnimationKoopas.at(3);
	}
	else if (state == KOOPAS_STATE_HAVE_WING_WALKING)
	{
		if (nx == -1)
			ani = listAnimationKoopas.at(4);
		else if (nx == 1)
			ani = listAnimationKoopas.at(5);
	}
	else if (state == KOOPAS_STATE_PRE_REVIE)
	{
		ani = listAnimationKoopas.at(6);
	}
	else if (state == KOOPAS_STATE_PRE_REVIE_2)
	{
		ani = listAnimationKoopas.at(7);
	}
	else if (state == KOOPAS_STATE_SHELL)
	{
		ani = listAnimationKoopas.at(8);
	}
	else if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		ani = listAnimationKoopas.at(9);
	}
	animation_set->at(ani)->Render(x, y);

	boxCalcFutureRight->Render();
	boxCalcFutureLeft->Render();

	if (effectPoint)
	{
		effectPoint->Render();
	}
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = -KOOPAS_ANI_SHELL_MOVING_SPEED_X;
		ani = KOOPAS_ANI_SHELL;
		break;
	case KOOPAS_STATE_SHELL:
		EnemyNoDamage();
		vx = 0;
		vy = 0;
		CountTimeToRevive();
		break;
	case KOOPAS_STATE_WALKING:
		EnemyDamage();
		isOutOfControl = false;
		vx = nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		EnemyDamage();
		isOutOfControl = false;
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		break;
	case KOOPAS_STATE_HAVE_WING_WALKING:
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		break;
	case KOOPAS_STATE_HAVE_WING_FLYING:
		vx = 0.05f *nx;
		//vy = -0.10f;
		break;
	case KOOPAS_STATE_PRE_REVIE:
		break;
	case KOOPAS_STATE_REIVING:
		break;
	case KOOPAS_STATE_SHELL_OUT_CONTROL:
		EnemyNoDamage();
		isOutOfControl = true;
		isPlayerHolding = true;
		break;
	default:
		break;
	}

}