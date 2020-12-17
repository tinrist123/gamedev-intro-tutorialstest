#include "Mario.h"
#include "P_Switch.h"



// TODO: MARIO CAM RUA THA NUT ATACKKING CHUA DA DUOC	DONE
// TODO: ANIMATION CAM RUA CON CHUA ON DINH				DONE
// TODO: CHUA LAM CON GOOMBA ( 2 LOAI)					DONE
// TODO: CHUA LAM COIN									DONE
// TODO: Chua lam Con mario chon` Atackking				DONE
// TODO: (Dangerous) Chua lam HUB
// TODO: MARIO DE SHIFT CHAY BAY CO VAN DE ANIMATION	DONE	
// TODO: CAM CON RUA ROI THA RA CHUA DUOC NEU ADD NHIEU CON KOOPAS	DONE

// CAM RUA , DA RUA XONG HET


// TODO: NGAY MAI
//	FIX MARIO FIRE ATTACK					DONE
//	KOOPAS NEVER FALL						DONE
//	KOOPAS BI LOI 2 CUC GACH				DONE
//	FLOWER ATTACK BY MOUTH
//	GRID
//	SEPERATE OBJECT TO UPDATE AND RENDER
//	COIN									DONEs
//	HUB!!!!!
//	isRenderOver co van de					hot/fix


/// <summary>
/// Constant field
/// <param name="kindMario"></param>
/// </summary>

int  CMario::DetectLevelSpeedMario()
{
		if (checkMarioTurnBack() && !this->FlagTurnBack)
		{
			if ((nx == 1 && vx >= -0.15) || (nx == -1 && vx <= 0.15))
			{
				this->setFlagLevelSpeedToTempLevelSpeed();
				detectVxLevelSpeed = lastVx;
			}
		}

	float detectVelocity = fabs(vx);

	if (this->checkFlagLevelSpeedToTempLevelSpeed() )
	{
		DecelerateVelocityRunning();
		detectVelocity = fabs(detectVxLevelSpeed);

		if (fabs(detectVxLevelSpeed) <= MARIO_MAX_WALKING_SPEED)
		{
			this->FlagTurnBack = false;
		}

		if (isRunning)
		{
			if (nx == 1)
			{
				vx = fabs(detectVxLevelSpeed);
			}
			else if (nx == -1)
			{
				vx = -fabs(detectVxLevelSpeed);
			}
			detectVelocity = vx;
			this->FlagTurnBack = false;
		}

	}




	if (detectVelocity > 0.12f)
	{
		for (int i = 7; i >= 1; i--)
		{
			if (detectVelocity - 0.12f >= (float)((1.0*8 / 700)*i*1.0))
			{
				return i;
			}
		}
	}
	return 0;
}

void CMario::storePosPipe_HaveHiddenMap(float posX, float posY)
{
	this->posX_OfPipe_HaveHiddenMap = posX;
	this->posY_OfPipe_HaveHiddenMap = posY;
}

bool CMario::checkFlagLevelSpeedToTempLevelSpeed()
{
	return FlagTurnBack;
}

void CMario::setFlagLevelSpeedToTempLevelSpeed()
{
	FlagTurnBack = true;

}

CMario::CMario(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;
	level = 4;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	constant = new Constant(level);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	this->type = 0;
}


void  CMario::ChainKickKoopas(CKoopas* &koopas,bool isElastic)
{
	if (isElastic)
	{
		vy = -MARIO_ELASTIC;
	}
	if (x > koopas->x ) koopas->nx = -1;
	else if (x < koopas->x) koopas->nx = 1;
	koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
	koopas->subHealth();
	// Mario Kick Koopas
	KickingKoopas();
}

void CMario::MarioHitEnemy()
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

void CMario::AccurateCollisionWithEnemy(LPGAMEOBJECT enemies)
{
	if (enemies == NULL) return;
	// HAM NAY PING THAT MA CO DIEU CO VAI ENEMIES VAN CO THE VA CHAM DUOC
	if (state == MARIO_STATE_DIE)
		return;
	if (untouchable == 1)
		return;
	if (this->y < enemies->y) return;
	//if (dynamic_cast<CGoomba*>(enemies)) return;
	//if (dynamic_cast<CKoopas*>(enemies)) return;
	if (!enemies->GetHealth() == 0 && enemies->isDamageable)
	{
		if (AABBCollision(enemies))
		{
			this->MarioHitEnemy();
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	if (level == MARIO_LEVEL_BIG_TAIL && isAttacking)
	{
		setTailPos();
		Tail->Update(dt, coObjects);
	}
	


	if (!this->CheckMarioSlideIntoPipe())
	{
		vy += MARIO_GRAVITY * dt;
	}

	if (this->CheckMarioSlideIntoPipe() && !this->isInHiddenMap && vy < 0)
	{
		DebugOut(L"vy = %f\n", this->vy);
		if (y + 28.0f < posY_OfPipe_HaveHiddenMap)
			this->MarioSlideOutPipe();
	}

	lastVx = vx;

	if (vy > 0)
	{
		isFalling = true;
	}

	if (fabs(vx) < MARIO_RUNNING_MAX_SPEED && isOnGround)
	{
		isMaxSpeed = false;
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

	if (nx == 1) {
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (isKeepJumping && animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT)->IsRenderOver(150))
			{
				isKeepJumping = false;
			}
			else if (isAttacking && animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->IsRenderOver(375))
			{
				isAttacking = false;
			}
		}
		else if (level == MARIO_LEVEL_BIG_FIRE)
		{
			if (isAttacking && !isJumpingAttack && animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT)->IsRenderOver(75))
			{
				isAttacking = false;
			}
			else if (isJumpingAttack && animation_set->at(MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_RIGHT)->IsRenderOver(200))
			{
				isJumpingAttack = false;
			}
		}
	}	
	else if (nx == -1)
	{
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (isKeepJumping && animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->IsRenderOver(150))
			{
				isKeepJumping = false;
			}
			else if (isAttacking && animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->IsRenderOver(375))
			{
				isAttacking = false;
			}
		}
		else if (level == MARIO_LEVEL_BIG_FIRE)
		{
			if (isAttacking && animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_LEFT)->IsRenderOver(75))
			{
				isAttacking = false;
			}
			else if (isJumpingAttack && animation_set->at(MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_LEFT)->IsRenderOver(200))
			{
				isJumpingAttack = false;
			}
		}
	}

	if (isRollBack && GetTickCount64() - tDraw > 100 && tDraw != 0 && isTested)
	{
		isRollBack = false;
		tDraw = 0;
		isTested = false;
	}
	
	if (isKickedKoopas && GetTickCount64() - tDraw > 100)
	{
		isKickedKoopas = false;
		tDraw = 0;
	}

	if (level == MARIO_LEVEL_SMALL)
	{
		if (!this->isGround(this->x + constant->listBBox_Mario_Big.at(0) + 3.0f , this->y + MARIO_SMALL_BBOX_WIDTH + 5.0f, *coObjects)
			&& !this->isGround(this->x - 1, this->y + MARIO_SMALL_BBOX_HEIGHT + 5.0f, *coObjects))
			this->MarioIsFalling();
	}
	else if (level != MARIO_LEVEL_SMALL)
	{
		if (!this->isGround(this->x + constant->listBBox_Mario_Big.at(0) - 6.0f, this->y + MARIO_BIG_BBOX_HEIGHT + 5.0f, *coObjects)
			)
			if (!this->isGround(this->x - 1, this->y + MARIO_BIG_BBOX_HEIGHT + 5.0f, *coObjects))
			{
				this->MarioIsFalling();
			}
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;	
		y += min_ty * dy + ny * 0.5f;

		if (nx != 0)
		{
			
		}
		else imMovable = true;
		
		if (ny != 0) {
			//vy = 0;
			isEnteredFirstSpaceUp = false;
			isPreventedSpamSpace = false;
		}		
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					isOnGround = true;
					OffPreventSpamSpace();
					blockJumping = false;
				}
			}
			if (dynamic_cast<CGoomba *>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->ny < 0)
				{
					if (goomba->level == PARAGOOMBA && goomba->GetState() != GOOMBA_STATE_WALKING)
					{
						goomba->SetState(GOOMBA_STATE_WALKING);
						goomba->subHealth();
						goomba->addPointToItem();
					}
					else if (goomba->GetState() == GOOMBA_STATE_WALKING)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						int temp = (goomba->level == 1) ? 7 : 0;
						goomba->y = goomba->y - GOOMBA_BBOX_HEIGHT_DIE - temp ;
						goomba->subHealth();
						goomba->addPointToItem();
					}
					vy = -MARIO_ELASTIC;
				}
				else if (e->nx != 0)
				{
					MarioHitEnemy();
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
				if (e->nx != 0)
				{
					this->ReduceVelocityWhenFly();
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
				Pipe* pipe = dynamic_cast<Pipe*>(e->obj);

				if (e->ny < 0)
				{
					if (pipe->isHaveHiddenMap && this->isDownPressed)
					{
						vy = 0.01f;
						y += dy*2;
						this->MarioSlideIntoPipe();
						storePosPipe_HaveHiddenMap(pipe->x, pipe->y);
						DebugOut(L"Test\n");
					}
				}
				else if (e->ny > 0)
				{
					if (pipe->isInHiddenMap
						&&
						!pipe->isPushMarioOut
						)
					{
						vy = -0.02f;
						y += dy;
						this->MarioSlideIntoPipe();
						storePosPipe_HaveHiddenMap(pipe->x, pipe->y);
					}
				}
				else if (e->nx != 0)
				{
					this->ReduceVelocityWhenFly();
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
				else if (e->nx != 0)
				{
					this->ReduceVelocityWhenFly();
				}
			}
			else if (dynamic_cast<FireBullet*>(e->obj))
			{
				if (e->ny != 0 || e->nx != 0)
				{
					MarioHitEnemy();
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
				{
					if (e->ny > 0 || e->nx != 0)
					{
						MarioHitEnemy();
					}
					else if (e->ny < 0)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->IncreHealth();
						vy = -MARIO_ELASTIC;

					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_SHELL)
				{
					// is Attacking mean Hold DIK_Z
					if (isAttacking && e->nx != 0)
					{	
						isCanHoldingKoopas = true;
						koopas->SetState(KOOPAS_STATE_SHELL_OUT_CONTROL);
						
					}
					else if (e->ny != 0 || e->nx != 0)
					{
						ChainKickKoopas(koopas, e->ny != 0);
						//vy = (e->ny!=0)?-MARIO_ELASTIC:0;
						//if (x > koopas->x + 8) koopas->nx = -1;
						//else if (x < koopas->x + 8) koopas->nx = 1;
						//else koopas->nx = 1;
						//koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
						//koopas->subHealth();
						//// Mario Kick Koopas
						//KickingKoopas();
					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_WALKING)
				{
					if (e->ny == -1)
					{
						isOnGround = true;
						if (koopas->health == KOOPAS_MAX_HEALTH)
						{
							koopas->SetState(KOOPAS_STATE_SHELL);
							koopas->subHealth();
							koopas->setKoopasWasHeldByPlayer();
							isOnGround = false;
							vy = -MARIO_ELASTIC;
							koopas->addPointToItem();
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
				else if (koopas->GetState() == KOOPAS_STATE_HAVE_WING_WALKING ||
					koopas->GetState() == KOOPAS_STATE_HAVE_WING_FLYING)
				{
					if (e->ny == -1)
					{
						isOnGround = true;
						koopas->SetState(KOOPAS_STATE_WALKING);
						koopas->subHealth();
						vy = -MARIO_ELASTIC;
					}
					else if (e->nx != 0)
					{
						// Mario Die Because of his stupid
					}
					
				}
}
			else if (dynamic_cast<CFlower*>(e->obj))
			{
				/*if (e->nx != 0 || e->ny != 0)
				{
					if (AABBCollision(e->obj))
					{
						this->MarioHitEnemy();
					}
				}*/
			}
			else if (e->obj->getTypeObject() == Type::P_SWITCH)
			{
				P_Switch* p = dynamic_cast<P_Switch*>(e->obj);
				if (e->ny < 0)
				{
					this->MarioSetOnGround();
					p->isActived = true;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


int CalcRollBackAndWalking(int rollbackValue,int normalValue,int walkingHoldKoopasAni,CMario *mario)
{
	if (mario->isCanHoldingKoopas)
	{
		return walkingHoldKoopasAni;
	}
	if (mario->isRollBack && mario->isOnGround && mario->tDraw == 0)
	{
		
		mario->tDraw = GetTickCount64();
		return rollbackValue;
	}
	else if (mario->isRollBack && GetTickCount64() - mario->tDraw > 100 && mario->tDraw != 0 && mario->isTested)
	{
		return normalValue;
	}
	if (!mario->isRollBack && mario->isOnGround)
	{
		return normalValue;
	}
	
	return mario->ani;
}

int CalcRenderForMARIO_BIG(CMario *mario,vector<int> listAnimationForResMario )
{
	
	if (mario->isKickedKoopas && !mario->isCanHoldingKoopas)
	{
		if (mario->nx == 1) mario->ani = listAnimationForResMario.at(16);
		else if (mario->nx == -1) mario->ani = listAnimationForResMario.at(17);
	}
	else if (mario->vx == 0)
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
				if (mario->isCanHoldingKoopas) mario->ani = listAnimationForResMario.at(18);
				else if (!mario->isCanHoldingKoopas)
					mario->ani = listAnimationForResMario.at(1);
			}
		}
		else {
			if (mario->onSitting == true)
			{
				if (mario->isOnGround)
					mario->ani = listAnimationForResMario.at(2);
			}
			else 
			{
				if (mario->isCanHoldingKoopas) mario->ani = listAnimationForResMario.at(19);
				else if (!mario->isCanHoldingKoopas)
					mario->ani = listAnimationForResMario.at(3);
			}
		}
	}
	else if (mario->vx != 0)
	{
		/*if (mario->isOnGround == false)
		{
			mario->onSitting = false;
		}*/
		if (mario->nx == 1)
		{
			if (mario->onSitting == true && mario->isWalking == false)
			{
				mario->ani = listAnimationForResMario.at(0);
			}
			else {
				mario->ani = CalcRollBackAndWalking(listAnimationForResMario.at(4), listAnimationForResMario.at(5), listAnimationForResMario.at(20), mario);
				if (!mario->isCanHoldingKoopas &&  mario->vx > 0)
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
			
			if (mario->onSitting == true && mario->isWalking == false)
			{
				mario->ani = listAnimationForResMario.at(2);
			}
			else {
				mario->ani = CalcRollBackAndWalking(listAnimationForResMario.at(6), listAnimationForResMario.at(7), listAnimationForResMario.at(21), mario);
				if (!mario->isCanHoldingKoopas &&  mario->vx < 0)
				{
					if (mario->isOnGround &&  mario->isRunning && mario->vx <= -MARIO_RUNNING_MAX_SPEED && !mario->isRollBack)
					{
						mario->ani = listAnimationForResMario.at(13);
					}
				}
			}
		}
	}
	if (!mario->isCanHoldingKoopas && mario->onSitting == false && !mario->isKickedKoopas)
	{
		if (!mario->isOnGround && !mario->isFalling  && fabs(mario->vx) < MARIO_RUNNING_MAX_SPEED)
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
		else if (!mario->isKeepJumpingHigher && !mario->isFalling && !mario->isOnGround  && fabs(mario->vx) >= MARIO_RUNNING_MAX_SPEED && mario->isMaxSpeed)
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
		else if (mario->isFalling)
		{
			if (mario->isOnGround == false)
			{
				if (mario->nx == 1)
				{
					//if (mario->state == MARIO_STATE_BIG_TAIL_KEEP_JUMP)
					{
						if (mario->isKeepJumping)
						{
							mario->ani = MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT;
						}
						else if (fabs(mario->vx) >= MARIO_RUNNING_MAX_SPEED && !mario->isOnGround && mario->isFalling
							&& mario->level == MARIO_LEVEL_BIG_TAIL
							)
						{
							mario->ani = MARIO_ANI_BIG_TAIL_RUNNING_FALLING_RIGHT;
						}
						else if (mario->animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT)->IsRenderOver(150)) 
						{
							mario->ani = listAnimationForResMario.at(10);
						}
					}
				}
				else if (mario->nx == -1)
				{
					{
						if (mario->isKeepJumping)
						{
							mario->ani = MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT;
						}
						else if(fabs(mario->vx) >= MARIO_RUNNING_MAX_SPEED && !mario->isOnGround && mario->isFalling
							&& mario->level == MARIO_LEVEL_BIG_TAIL
						)
						{
							mario->ani = MARIO_ANI_BIG_TAIL_RUNNING_FALLING_LEFT;
						}
						else if(mario->animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->IsRenderOver(150)) {
							mario->ani = listAnimationForResMario.at(11);
						}
					}
				}
			}
		}
	}
	else if (mario->isCanHoldingKoopas)
	{
		if (!mario->isOnGround)
		{
			mario->ani = (mario->nx == 1) ? listAnimationForResMario.at(22) : listAnimationForResMario.at(23);
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
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_RIGHT, MARIO_ANI_SMALL_WALKING_RIGHT, MARIO_ANI_SMALL_WALKING_RIGHT, this);
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
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_LEFT, MARIO_ANI_SMALL_WALKING_LEFT, MARIO_ANI_SMALL_WALKING_LEFT, this);
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
		if (!isCanHoldingKoopas)
		{
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
	}
	else if (level == MARIO_LEVEL_BIG_TAIL)
	{
		constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this,constant->listAni_Mario_Big);
		if (!isCanHoldingKoopas)
		{
			if (isAttacking)
			{
				if (nx == 1)
				{
					ani = MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT;
				}
				else if (nx == -1)
				{
					ani = MARIO_ANI_BIG_TAIL_ATTACKING_LEFT;
				}
			}
			else if (isKeepJumpingHigher && isMaxSpeed && !isOnGround)
			{
				if (nx == 1)
				{
					ani = MARIO_ANI_BIG_TAIL_KEEP_FLYING_RUNNING_RIGHT;
				}
				else if (nx == -1)
				{
					ani = MARIO_ANI_BIG_TAIL_KEEP_FLYING_RUNNING_LEFT;
				}
			}
		}
		
		
	}
	else if (level == MARIO_LEVEL_BIG_FIRE)
	{
		constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this, constant->listAni_Mario_Big);
		if (!isCanHoldingKoopas)
		{
			if (isAttacking)
			{
				if (!isOnGround)
				{
					if (isJumpingAttack)
					{
						if (nx == 1)
						{
							ani = MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_RIGHT;
						}
						else if (nx == -1)
						{
							ani = MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_LEFT;
						}
					}
				}
				else if (isOnGround)
				{
					if (nx == 1)
					{
						ani = MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT;
					}
					else if (nx == -1)
					{
						ani = MARIO_ANI_BIG_FIRE_ATTACKING_LEFT;
					}
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
		
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	
	animation_set->at(100)->Render(x, y, alpha);

	if (isAttacking && level == MARIO_LEVEL_BIG_TAIL)
	{
		Tail->Render();
	}
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
	case MARIO_STATE_AUTO_GO:
		break;
	case MARIO_STATE_BIG_ATTACK:
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (nx == 1) 
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->SetCurrentFrame();
				animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->StartRenderAnimation();
			}
			else if (nx == -1)
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->SetCurrentFrame();
				animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->StartRenderAnimation();
			}
		}
		else if (level == MARIO_LEVEL_BIG_FIRE)
		{
			if (isOnGround)
			{
				if (nx == 1)
					animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT)->StartRenderAnimation();
				else if (nx == -1)
					animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_LEFT)->StartRenderAnimation();
			}
			else if (!isOnGround)
			{
				if (nx == 1)
				{
					animation_set->at(MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_RIGHT)->StartRenderAnimation();
				}
				else if (nx == -1)
					animation_set->at(MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_LEFT)->StartRenderAnimation();
			}
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
		
		if (vx > 0 )
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
			vx += (vx <= MARIO_MAX_WALKING_SPEED)? MARIO_WALKING_ACCELEROMETER :MARIO_WALKING_ACCELEROMETER_RUNNING;
			if (vx >= MARIO_RUNNING_PRE_MAX_SPEED)
			{
				isMaxSpeed = false;
			}
			else setTimeRenderingAni = 100;
			if (vx >= MARIO_RUNNING_MAX_SPEED)
			{
				isMaxSpeed = true;
				vx = MARIO_RUNNING_MAX_SPEED;
			}
		}
		else if (nx == -1)
		{
			vx -= (vx >= -MARIO_MAX_WALKING_SPEED) ? MARIO_WALKING_ACCELEROMETER : MARIO_WALKING_ACCELEROMETER_RUNNING;
			if (vx <= -MARIO_RUNNING_PRE_MAX_SPEED)
			{
				isMaxSpeed = false;
			}
			else setTimeRenderingAni = 100;
			if (vx <= -MARIO_RUNNING_MAX_SPEED)
			{
				isMaxSpeed = true;
				vx = -MARIO_RUNNING_MAX_SPEED;
			}
		}

		break;	
	} 
	case MARIO_STATE_BIG_TAIL_KEEP_JUMP:
		if (isMaxSpeed)
		{
			if (CheckExpiredFlyingForTail() && this->timeFlyingForTail != 0)
			{
				isKeepJumping = false;
				isMaxSpeed = false;
				isFalling = true;
				isKeepJumpingHigher = false;
				this->timeFlyingForTail = 0;
			}
			else if (this->timeFlyingForTail == 0)
			{
				SetTimeFlyingForTail();
				isKeepJumpingHigher = true;
			}
			vy = -(MARIO_GRAVITY + 0.002f * 4) * dt;
			if (vy <= -MARIO_KEEP_JUMP_LIMITED)
			{
				vy = -MARIO_KEEP_JUMP_LIMITED;
			}
		}
		else if (isFalling)
		{
			this->ReduceVelocityWhenFly();
			isKeepJumping = true;
			this->timeFlyingForTail = 0;
			if (nx == 1 )
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT)->StartRenderAnimation();
			}
			if (nx == -1 && animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->IsRenderOver(150))
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->StartRenderAnimation();
			}
		}
		break;
	case MARIO_STATE_JUMP:
			if (isOnGround && !blockJumping)
			{
				if (isRunning && fabs(vx) >= MARIO_RUNNING_MAX_SPEED)
				{
					vy = -MARIO_JUMP_MAX_SPEED_Y;
					vx = MARIO_MAX_WALKING_SPEED;
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
			if (!isCanHoldingKoopas)
			{
				nx = 1;
			}
		}
		else if (nx == 1 && vx < 0) {
			lastNx = nx;
			if (!isCanHoldingKoopas)
			{
				nx = -1;
			}
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

void CalcRenderBoxBIG_MARIO(float &top,float &right,float &bottom,int state,int level, bool isOnGround,bool &isWalking,float x, float y,vector<int> listBBox_Render)
{
	if (state == MARIO_STATE_SITTING && isWalking == false)
	{
		top = y + MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_TAIL_BBOX_SITTING_HEIGHT;
	}
	if (level == MARIO_LEVEL_BIG_TAIL)
	{
		right = x + listBBox_Render.at(0) + 1;
	}
	else 
	{
		right = x + listBBox_Render.at(0) - (MARIO_TAIL_WIDTH + 1);
	}
	bottom = y + listBBox_Render.at(1) + 1;
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
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (nx == 1)
				left = x + MARIO_BIG_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX + 1;
			else if (nx == -1)
			{
				left = x + MARIO_BIG_TAIL_BBOX_WIDTH - MARIO_BIG_BBOX;
			}
		}
		// MARIO BIG HAVENT TAIL
		else {
			left = x + MARIO_TAIL_WIDTH;
		}
		top = y;

		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_BIG_FIRE)
		{

			CalcRenderBoxBIG_MARIO(top, right, bottom, state,level, isOnGround, isWalking, x, y, constant->listBBox_Mario_Big);
		}
		else if (level == MARIO_LEVEL_BIG_TAIL)
		{
			CalcRenderBoxBIG_MARIO(top, right, bottom, state,level, isOnGround, isWalking, x, y, constant->listBBox_Mario_Big);
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
void CMario::CollideWithEnemy(vector<LPGAMEOBJECT> enemies)
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->isDamageable)
		{
			if (this->AABBCollision(enemies[i]))
			{
				// MARIO HIT ENEMY BECAUSE OF HIS STUPID
				this->MarioHitEnemy();
			}
		}
	}
}

void CMario::playerHittingSpecialItem(LPGAMEOBJECT& item, vector<LPGAMEOBJECT>& listEffect)
{
	item->SetState(ITEM_STATE_HITTING_MARIO);
	this->level = (this->level >= MARIO_LEVEL_BIG_TAIL) ? MARIO_LEVEL_BIG_TAIL : ++this->level;
	this->constant->changeLevelMario(this->level);
	// GET ITEM AND UPDATE MARIO LEVEL
	if (this->level == 2) this->y += -15.0f;
	else this->y += -0.4f;

	EffectPoint* effect = new EffectPoint();
	effect->SetPosition(item->x, item->y);
	listEffect.push_back(effect);
}

void CMario::CollideWithItem(vector<LPGAMEOBJECT> items, vector<LPGAMEOBJECT>& listEffect)
{
	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getTypeObject() == Type::P_SWITCH 
			|| items[i]->GetState() == ITEM_STATE_COIN_JUMP
			)
		{
			continue;
		}
		if (this->AABBCollision(items[i]))
		{
			if (items[i]->isTouchable)
			{
				if ((items[i]->GetState() == ITEM_STATE_SPECIAL_ITEM || items[i]->GetState() == ITEM_STATE_LEAF_FALL))
				{
					playerHittingSpecialItem(items[i], listEffect);
					this->increScores();
				}
				else if (items[i]->getTypeObject() == Type::COIN)
				{
					this->IncreScoreMario();
				}
    				items[i]->setObjDisappear();
				// Increase Poin of Player
			}
		}
	}
}
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel((level > MARIO_LEVEL_BIG_TAIL)? MARIO_LEVEL_SMALL:++level);
	constant->changeLevelMario(level);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}