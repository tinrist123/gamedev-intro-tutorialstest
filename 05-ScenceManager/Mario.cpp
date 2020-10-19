#include <algorithm>
#include <assert.h>
#include "Mario.h"
#include "ColorBox.h"
#include "Game.h"
#include "Utils.h"
#include "Pipe.h"
#include "Koopas.h"
#include "FireBullet.h"


/// <summary>
/// Constant field
/// <param name="kindMario"></param>
/// </summary>

CMario::CMario(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;
	level = 3;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	constant = new Constant(level);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	/*if (fabs(vx) > MARIO_MAX_WALKING_SPEED && !isOnGround && !isRunning)
	{
		vx -= min(fabs(vx), FRICTION) * nx;
	}*/

	if (isSpecialGravity) vy += MARIO_GRAVITY_KEPP_JUMP * dt;
	else vy += MARIO_GRAVITY * dt;
	lastVx = vx;
	// Simple fall down
	if (vy > 0)
	{
		isFalling = true;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	

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

		if (ny != 0) {
			vy = 0;
		}

		if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0 && isTested)
		{
			isRollBack = false;
			tDraw = 0;
			isTested = false;
		}

		if (isKeepJumping && GetTickCount64() - tDraw > 300)
		{
			isKeepJumping = false;
			tDraw = 0;
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
					vy = 0;
					isOnGround = true;
				}

			}
			if (dynamic_cast<Item *>(e->obj))
			{
				Item* item = dynamic_cast<Item *>(e->obj);
				if (e->nx != 0 || e->ny != 0)
				{
					if (item->health != 0) item->SetState(2);
					//else
					{
						item->SetState(3);
						level = (level >= 3) ? 3 : ++level;
						constant->changeLevelMario(level);
						if (level == 2) y += -15.0f;
						else y += -0.4f;
					}
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* questionbrick = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->ny > 0)
				{
					if (questionbrick->health != 0)
						questionbrick->SetState(1);
				}
				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
			}
			else if (dynamic_cast<ColorBox*>(e->obj))
			{

				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
				if (e->ny != 0 || e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (level == MARIO_LEVEL_SMALL)
						{
							SetState(MARIO_STATE_DIE);
							isBoundingBox = false;
						}
						else
						{
							level--;
							constant->changeLevelMario(level);
							StartUntouchable();
						}
					}
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
				{
					if (e->ny != 0 || e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (level == MARIO_LEVEL_SMALL)
							{
								SetState(MARIO_STATE_DIE);
								isBoundingBox = false;
							}
							else
							{
								level--;
								constant->changeLevelMario(level);
								StartUntouchable();
							}
						}
					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_SHELL)
				{
					if (e->ny != 0 || e->nx != 0)
					{
						if (x > koopas->x + 8) koopas->nx = -1;
						else if (x < koopas->x + 8) koopas->nx = 1;
						else koopas->nx = 1;
						koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_WALKING)
				{
					if (e->ny == -1)
					{
						isOnGround = true;
						if (koopas->health == 2)
						{
							koopas->SetState(KOOPAS_STATE_SHELL);
							koopas->subHealth();
							isOnGround = false;
							vy = -0.15f;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (level == MARIO_LEVEL_SMALL)
							{
								SetState(MARIO_STATE_DIE);
								isBoundingBox = false;
							}
							else
							{
								level--;

								constant->changeLevelMario(level);
								
								StartUntouchable();
							}
						}
						
					}
				}
			}
			
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


int CalcRollBackAndWalking(int rollbackValue,int normalValue,bool &isTested,bool &isRollBack,bool isOnGround,float vx,float lastVx,int nx ,DWORD &tDraw,int ani)
{
	
	if (isRollBack && isOnGround && tDraw == 0)
	{
		tDraw = GetTickCount64();
		return rollbackValue;
	}
	else if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0 && isTested)
	{
		return normalValue;
	}
	if (!isRollBack && isOnGround)
	{
		return normalValue;
	}
	return ani;
}

int CalcRenderForMARIO_BIG(CMario *mario,vector<int> listAnimationForResMario )
{
	if (mario->vx == 0)
	{
		if (mario->nx == 1)
		{
			if (mario->onSitting == true )
			{
				if (mario->isOnGround)
					mario->ani = listAnimationForResMario.at(0);
			}
			else
			{
				mario->ani = listAnimationForResMario.at(1);
			}
		}
		else {
			if (mario->onSitting == true)
			{
				if (mario->isOnGround)
					mario->ani = listAnimationForResMario.at(2);
			}
			else mario->ani = listAnimationForResMario.at(3);
		}
	}
	else if (mario->vx != 0)
	{
		if (mario->isOnGround == false)
		{
			mario->onSitting = false;
		}
		if (mario->nx ==1)
		{
			if (mario->onSitting == true && mario->isWalking == false)
			{
				mario->ani = listAnimationForResMario.at(0);
			}
			else {
				/*if (isRollBack && tDraw == 0 && isOnGround == true)
				{
					ani = MARIO_ANI_BIG_WALKING_STOP_RIGHT;
					tDraw = GetTickCount64();
				}
				if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0)
				{
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
					isRollBack = false;
					tDraw = 0;
				}
				if (isRollBack == false)
				{
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}*/
				mario->ani = CalcRollBackAndWalking(listAnimationForResMario.at(4), listAnimationForResMario.at(5), mario->isTested, mario->isRollBack, mario->isOnGround, mario->vx, mario->lastVx, mario->nx, mario->tDraw, mario->ani);
				if (mario->vx > 0)
				{
					if (mario->isOnGround && mario->isRunning && mario->vx >= MARIO_RUNNING_MAX_SPEED)
					{
						mario->ani = listAnimationForResMario.at(12);
					}
				}
			}
		}
		else if (mario->nx == -1)
		{
			/*if (isRollBack)
			{
				ani = MARIO_ANI_WALKING_STOP_LEFT;
				isRollBack = false;
			}
			else
				ani = MARIO_ANI_WALKING_LEFT;*/
			if (mario->onSitting == true && mario->isWalking == false)
			{
				mario->ani = listAnimationForResMario.at(2);
			}
			else {
				/*if (isRollBack && tDraw == 0 && isOnGround == true)
				{
					ani = MARIO_ANI_BIG_WALKING_STOP_LEFT;
					tDraw = GetTickCount64();
				}
				if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0)
				{
					ani = MARIO_ANI_BIG_WALKING_LEFT;
					isRollBack = false;
					tDraw = 0;
				}
				if (isRollBack == false)
				{
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				}*/
				mario->ani = CalcRollBackAndWalking(listAnimationForResMario.at(6), listAnimationForResMario.at(7), mario->isTested, mario->isRollBack, mario->isOnGround, mario->vx, mario->lastVx, mario->nx, mario->tDraw, mario->ani);
				if (mario->vx < 0)
				{
					if (mario->isOnGround &&  mario->isRunning && mario->vx <= -MARIO_RUNNING_MAX_SPEED && !mario->isRollBack)
					{
						mario->ani = listAnimationForResMario.at(13);
					}
				}
			}
		}
	}
	if (mario->onSitting == false)
	{
		if (mario->isFalling == false && fabs(mario->vx) < MARIO_RUNNING_MAX_SPEED)
		{
			
			if (mario->nx == 1)
			{
				mario->ani = listAnimationForResMario.at(8);
			}
			else if (mario->nx == -1)
			{
				mario->ani = listAnimationForResMario.at(9);
			}
			
		}
		else if (mario->isFalling == false && !mario->isOnGround  && fabs(mario->vx) >= MARIO_RUNNING_MAX_SPEED)
		{
			if (mario->nx == 1)
			{
				mario->ani = listAnimationForResMario.at(14);
			}
			else if (mario->nx == -1)
			{
				mario->ani = listAnimationForResMario.at(15);
			}
		}
		else
		{
			if (mario->isOnGround == false)
			{
				if (mario->nx == 1)
				{
					if (mario->state == MARIO_STATE_BIG_TAIL_KEEP_JUMP)
					{
						if (mario->isKeepJumping)
						{
							mario->ani = listAnimationForResMario.at(16);
						}
					}
					else {
						mario->ani = listAnimationForResMario.at(10);
					}
				}
				else if (mario->nx == -1)
				{
					if (mario->state == MARIO_STATE_BIG_TAIL_KEEP_JUMP)
					{
						if (mario->isKeepJumping)
						{
							mario->ani = listAnimationForResMario.at(17);
						}
					}
					else {
						mario->ani = listAnimationForResMario.at(11);
					}
				}
			}
			
		}
	}
	return mario->ani;
}


void CMario::Render()
{
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx != 0)
		{
			if (nx == 1)
			{
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_RIGHT, MARIO_ANI_SMALL_WALKING_RIGHT, isTested, isRollBack, isOnGround, vx, lastVx, nx, tDraw, ani);
				if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
				{
					ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
				}
			}
			else if (nx == -1)
			{
				/*if (isRollBack && isOnGround && tDraw == 0)
				{
					tDraw = GetTickCount64();
					ani = MARIO_ANI_SMALL_WALKING_STOP_LEFT;
				}
				else if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0 && isTested)
				{
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				if (!isRollBack)
				{
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}*/
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_LEFT, MARIO_ANI_SMALL_WALKING_LEFT, isTested, isRollBack, isOnGround, vx, lastVx, nx, tDraw, ani);
				if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
				{
					ani = MARIO_ANI_SMALL_RUNNING_LEFT;
				}
			}
		}
		if (isOnGround == false)
		{
			if (nx == 1)
			{

				if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
				{
					ani = MARIO_ANI_SMALL_RUNNING_FLYING_RIGHT;
				}
				else 
					ani = MARIO_ANI_SMALL_FLYING_RIGHT;

			}
			else if ( nx == -1 )
			{

				if (isRunning && vx <= -MARIO_RUNNING_MAX_SPEED)
				{
					ani = MARIO_ANI_SMALL_RUNNING_FLYING_LEFT;
				}
				else 
					ani = MARIO_ANI_SMALL_FLYING_LEFT;

			}
		}
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		/*if (vx == 0)
		{
			if (nx > 0)
			{
				if (onSitting == true)
				{
					ani = MARIO_ANI_BIG_SITTING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
				}
			}
			else {
				if (onSitting == true)
				{
					ani = MARIO_ANI_BIG_SITTING_LEFT;
				}
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
		}
		else if (vx != 0)
		{*/
			//if (isOnGround == false)
			//{
			//	onSitting = false;
			//}
			//if (vx > 0)
			//{
			//	if (onSitting == true)
			//	{
			//		ani = MARIO_ANI_BIG_SITTING_RIGHT;
			//	}
			//	else {
			//		ani = CalcRollBackAndWalking(MARIO_ANI_BIG_WALKING_STOP_RIGHT, MARIO_ANI_BIG_WALKING_RIGHT, isRollBack, isOnGround, tDraw, ani);
			//	}

			//}
			//else if (vx < 0)
			//{
			//	/*if (isRollBack)
			//	{
			//		ani = MARIO_ANI_WALKING_STOP_LEFT;
			//		isRollBack = false;
			//	}
			//	else
			//		ani = MARIO_ANI_WALKING_LEFT;*/
			//	if (onSitting == true)
			//	{
			//		ani = MARIO_ANI_BIG_SITTING_LEFT;
			//	}
			//	else {
			//		ani = CalcRollBackAndWalking(MARIO_ANI_BIG_WALKING_STOP_LEFT, MARIO_ANI_BIG_WALKING_LEFT, isRollBack, isOnGround, tDraw, ani);

			//	}
			//}


		//}
		/*if (onSitting == false)
		{
			if (isFalling == false)
			{
				if (nx == 1)
				{
					ani = MARIO_ANI_BIG_FLYING_RIGHT;
				}
				else ani = MARIO_ANI_BIG_FLYING_LEFT;
			}
		}*/
		Constant *constant = new Constant(level);
		ani = CalcRenderForMARIO_BIG(this,constant->listAni_Mario_Big);
		if (fabs(vx) >= MARIO_RUNNING_MAX_SPEED && !isOnGround && isFalling)
		{
			if (nx == 1)
			{
				ani = MARIO_ANI_BIG_RUNNING_FLYING_RIGHT;
			}
			else if (nx == -1)
			{
				ani = MARIO_ANI_BIG_RUNNING_FLYING_LEFT;
			}
		}
	}
	else if (level == MARIO_LEVEL_BIG_TAIL)
	{
		constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this,constant->listAni_Mario_Big);
		if (state == MARIO_STATE_BIG_TAIL_ATTACK)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT;
			}
			else if (nx < 0)
			{
				ani = MARIO_ANI_BIG_TAIL_ATTACKING_LEFT;
			}
		}
		else if (fabs(vx) >= MARIO_RUNNING_MAX_SPEED && !isOnGround && isFalling)
		{
			if (nx == 1)
			{
				ani = constant->listAni_Mario_Big.at(18);
			}
			else if (nx == -1)
			{
				ani = constant->listAni_Mario_Big.at(19);
			}
		}
	}
	else if (level == MARIO_LEVEL_BIG_FIRE)
	{
		constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this, constant->listAni_Mario_Big);
		if (state == MARIO_STATE_BIG_FIRE_ATTACK)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT;
			}
			else if (nx < 0)
			{
				ani = MARIO_ANI_BIG_FIRE_ATTACKING_LEFT;
			}
		}
		else if (fabs(vx) >= MARIO_RUNNING_MAX_SPEED && !isOnGround && isFalling)
		{
			if (nx == 1)
			{
				ani = constant->listAni_Mario_Big.at(14);
			}
			else if (nx == -1)
			{
				ani = constant->listAni_Mario_Big.at(15);
			}
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->setTimeAnimation(isRunning? setTimeRenderingAni :100)->Render(x, y, alpha);
	
	RenderBoundingBox();
}
void CMario::SetState(int state)
{
	CGameObject::SetState(state);


	switch (state)
	{
	/*case MARIO_STATE_STOP_SITTING:
		onSitting = false;
		break;*/
	case MARIO_STATE_BIG_TAIL_ATTACK:
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (nx == 1) animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->StartRenderAnimation();
			if (nx == -1) animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->StartRenderAnimation();
		}
		else if (level == MARIO_LEVEL_BIG_FIRE)
		{
			if (nx == 1) animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT)->StartRenderAnimation();
			if (nx == -1) animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_LEFT)->StartRenderAnimation();
		}
		
		break;
	case MARIO_STATE_NOT_WALKING:
		isWalking = false;
		isRollBack = false;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		isWalking = true;
		if (vx < 0)
		{
			vx += MARIO_WALKING_DECELERATION;
			isRollBack = true;
		}
		else if (vx <= MARIO_MAX_WALKING_SPEED)
		{
			vx += MARIO_WALKING_ACCELEROMETER;
			if (vx >= MARIO_MAX_WALKING_SPEED && !isRunning)
			{
				vx = MARIO_MAX_WALKING_SPEED;
			}
		}
		else if (vx >= MARIO_MAX_WALKING_SPEED && !isRunning)
		{
			vx -= MARIO_WALKING_ACCELEROMETER;
		}
		if ((double)lastVx * vx <= 0)
		{
			isTested = true;
		}

		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		isWalking = true;
		if (vx > 0)
		{
			vx -= MARIO_WALKING_DECELERATION;
			isRollBack = true;
		}
		else if (vx >= -MARIO_MAX_WALKING_SPEED)
		{
			vx -= MARIO_WALKING_ACCELEROMETER;
			if (vx <= -MARIO_MAX_WALKING_SPEED && !isRunning)
			{
				vx = -MARIO_MAX_WALKING_SPEED;
			}
		}
		else if (vx <= -MARIO_MAX_WALKING_SPEED && !isRunning)
		{
			vx += MARIO_WALKING_ACCELEROMETER;
		}
		if ((double)lastVx * vx <= 0)
		{
			isTested = true;
		}

		nx = -1;
		break;
	case MARIO_STATE_STOP_RUNNING:
	{
		isRunning = false;
		break;
	}
	case MARIO_STATE_RUNNING:
	{
		isRunning = true;
		if (nx == 1)
		{
			vx += MARIO_WALKING_ACCELEROMETER*1.5;
			if (vx >= MARIO_RUNNING_PRE_MAX_SPEED)
			{
				setTimeRenderingAni = 40;
				
			}
			else setTimeRenderingAni = 100;
			if (vx >= MARIO_RUNNING_MAX_SPEED)
			{
				vx = MARIO_RUNNING_MAX_SPEED;
				setTimeRenderingAni = 10;
			}
		}
		else if (nx == -1)
		{
			vx -= MARIO_WALKING_ACCELEROMETER * 1.5;
			if (vx <= -MARIO_RUNNING_PRE_MAX_SPEED)
			{
				setTimeRenderingAni = 40;
			}
			else setTimeRenderingAni = 100;
			if (vx <= -MARIO_RUNNING_MAX_SPEED)
			{
				vx = -MARIO_RUNNING_MAX_SPEED;
				setTimeRenderingAni = 10;
			}
		}

		break;	
	} 
	case MARIO_STATE_BIG_TAIL_KEEP_JUMP:
		if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
		{
			vy -= (MARIO_GRAVITY + 0.002f * 2) * dt;
			if (vy <= -0.5f)
			{
				vy = -0.5f;
			}
		}
		else if (isFalling)
		{
			/*if (vy - ((MARIO_GRAVITY * 15.0f + 0.0002f) * dt) < 0)
			{
				DebugOut(L"vy = %f\n", vy);
				break;
			}	
			else*/ {
				isSpecialGravity = true;
				if (vy > 0) vy = 0;
			}
			isKeepJumping = true;
			if (nx == 1 ) animation_set->at(MARIO_ANI_BIG_TAIL_FALLING_RIGHT)->StartRenderAnimation();
			if (nx == -1)
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_FALLING_LEFT)->StartRenderAnimation();
			}
		}
		break;
	case MARIO_ANI_SHORT_JUMP:
		if (!isOnGround && !isFalling)
		{
			vy = vy + MARIO_GRAVITY*dt * 10;
		}
		blockJumping = false;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
			if (isOnGround && blockJumping == false )
			{
				if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
				{
					vy = -MARIO_JUMP_MAX_SPEED_Y;
				}
				else {
					vy = -MARIO_JUMP_SPEED_Y;
				}
				isFalling = false;
				isOnGround = false;
				blockJumping = true;
			}
		break;
	case MARIO_STATE_IDLE:
		isSpecialGravity = false;
		onSitting = false;
		isRunning = false;
		if (lastVx * vx <= 0)
		{
			isRollBack = false;
		}
		if (nx == -1 && vx > 0)
		{
			lastNx = nx;
			nx = 1;
		}
		else if (nx == 1 && vx < 0 ) {
			lastNx = nx;
			nx = -1;
		}
		if (isOnGround) vx -= min(fabs(vx), FRICTION) * nx;
		if (lastNx != 0)
		{
			isRollBack = false;
			nx = lastNx;
			lastNx = 0;
		}
		break;
	case MARIO_STATE_SITTING: 
		if (isWalking == false)
		{
			onSitting = true;
		}
		if (lastVx * vx <= 0)
		{
			isRollBack = false;
		}
		if (nx == -1 && vx > 0)
		{
			lastNx = nx;
			nx = 1;
		}
		else if (nx == 1 && vx < 0) {
			lastNx = nx;
			nx = -1;
		}
		if (isOnGround) vx -= min(fabs(vx), FRICTION) * nx;
		if (lastNx != 0)
		{
			isRollBack = false;
			nx = lastNx;
			lastNx = 0;
		}
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CalcRenderBoxBIG_MARIO(float &top,float &right,float &bottom,int state,bool isOnGround,bool &isWalking,float x, float y,vector<int> listBBox_Render)
{
	if (state == MARIO_STATE_SITTING && isWalking == false)
	{
		top = y + 8;
	}
	right = x + listBBox_Render.at(0);
	bottom = y + listBBox_Render.at(1);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isBoundingBox)
	{
		left = right;
		top = bottom;
	}
	else
	{
		left = x;
		top = y;

		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_BIG_FIRE)
		{

			CalcRenderBoxBIG_MARIO(top, right, bottom, state, isOnGround, isWalking, x, y, constant->listBBox_Mario_Big);
		}
		else if (level == MARIO_LEVEL_BIG_TAIL)
		{
			CalcRenderBoxBIG_MARIO(top, right, bottom, state, isOnGround, isWalking, x, y, constant->listBBox_Mario_Big);
		}
		else
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
	}
	
	
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(level);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
