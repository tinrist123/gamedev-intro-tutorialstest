#include "Mario.h"
#include "P_Switch.h"
#include "WeakBrick.h"
#include "RandomItem.h"
#include "Mushroom.h"
#include "MovingWood.h"
#include "BoomerangBrother.h"

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
/// </summary>`

int  CMario::DetectLevelSpeedMario()
{
		/*if (checkMarioTurnBack() && !this->FlagTurnBack)
		{
			if ((nx == 1 && vx >= -0.15) || (nx == -1 && vx <= 0.15))
			{
				this->setFlagLevelSpeedToTempLevelSpeed();
				detectVxLevelSpeed = lastVx;
			}
		}*/

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

void CMario::SwitchScreen()
{
	this->isSwitchedScreen = true;
}

void CMario::CompleteSwitchedScreen()
{
	this->isSwitchedScreen = false;
}

void CMario::storePosPipe_HaveHiddenMap(float posX, float posY)
{
	this->posX_OfPipe_HaveHiddenMap = posX;
	this->posY_OfPipe_HaveHiddenMap = posY;
}


CMario* CMario::__instance = NULL;

CMario* CMario::GetInstance()
{

	if (__instance == NULL)
		__instance = new CMario();
	return __instance;
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
	this->type = Type::MARIO;
	health = 4;
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
	DebugOut(L"untouchable %d\n", untouchable);
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			SetState(MARIO_STATE_DIE);
			isBoundingBox = false;
		}
		else
		{
			this->SetLevel(--level);
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

void CMario::CollisionWithFireball(vector <LPGAMEOBJECT> listFireball)
{
	for (size_t i = 0; i < listFireball.size(); i++)
	{
		if (this->AABBCollision(listFireball[i]))
		{
			MarioHitEnemy();
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj , vector<LPGAMEOBJECT>* dynamic_item = NULL , vector<LPGAMEOBJECT>* enemies = NULL, vector<LPGAMEOBJECT>* listPortalStop)
{

#pragma region Mario Update
	// Calculate dx, dy
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;

	if (state == MARIO_STATE_BLOCK_STATE_END_GAME)
	{
		if (this->isOnGround)
		{
			vx = MARIO_MAX_WALKING_SPEED;
		}
	}

	if (level == MARIO_LEVEL_BIG_TAIL && isAttacking)
	{
		setTailPos();
		Tail->Update(dt, staticObj,enemies);
	}
	else if (level == MARIO_LEVEL_BIG_TAIL && !isAttacking && !Tail->isDamageable)
	{
		Tail->TurnOnDamageable();
	}

	if (!this->CheckMarioSlideIntoPipe() && !this->isInWorldSelectionMap)
	{
		vy += MARIO_GRAVITY * dt;
	}

	if (this->CheckMarioSlideIntoPipe() && !this->isInHiddenMap && vy < 0)
	{
		if (y + 29.0f < posY_OfPipe_HaveHiddenMap)
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

	/*if (level == MARIO_LEVEL_SMALL)
	{
		if (!this->isGround(this->x + constant->listBBox_Mario_Big.at(0) + 3.0f, this->y + MARIO_SMALL_BBOX_WIDTH + 5.0f, *staticObj)
			&& !this->isGround(this->x - 1, this->y + MARIO_SMALL_BBOX_HEIGHT + 5.0f, *staticObj))
			this->MarioIsFalling();
	}
	else if (level != MARIO_LEVEL_SMALL)
	{
		if (!this->isGround(this->x + constant->listBBox_Mario_Big.at(0) - 6.0f, this->y + MARIO_BIG_BBOX_HEIGHT + 5.0f, *staticObj)
			)
			if (!this->isGround(this->x - 1, this->y + MARIO_BIG_BBOX_HEIGHT + 5.0f, *staticObj))
			{
				this->MarioIsFalling();
			}
	}*/
#pragma endregion

#pragma region StaticObj Coliision
	vector<LPCOLLISIONEVENT> coEvents_static_Obj;
	vector<LPCOLLISIONEVENT> coEventsResult_static_Obj;

	coEvents_static_Obj.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(staticObj, coEvents_static_Obj);

	// No collision occured, proceed normally
	if (coEvents_static_Obj.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents_static_Obj, coEventsResult_static_Obj, min_tx, min_ty, nx, ny, rdx, rdy);
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		

		// block every object first!
		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

		if (nx != 0)
		{
		}
		else imMovable = true;

		if (ny != 0) {
			if (ny < 0)
			{
				vy = 0;
				isEnteredFirstSpaceUp = false;
				isPreventedSpamSpace = false;
			}
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult_static_Obj.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult_static_Obj[i];

			if (dynamic_cast<Ground*>(e->obj))
			{	
				if (e->ny < 0)
				{
					this->vy = 0;
					this->isOnGround = true;
					OffPreventSpamSpace();
					blockJumping = false;
				}
				else if (e->ny > 0)
				{
					this->vy = 0;
				}
			}
			if (e->obj->getTypeObject() == Type::MOVINGWOOD)
			{
				this->vy = 0;
				if (e->ny < 0)
				{
					MovingWood* wood = dynamic_cast<MovingWood*>(e->obj);
					wood->SetState(MOVING_WOOD_STATE_FALLING);
				}
				else if (e->nx != 0)
				{
					/*if (rdx != 0 && rdx!=dx)
						x += nx*abs(rdx); */
					//this->vx = 0;
					//this->MarioSetOnGround();
				}
				else if (e->ny > 0)
				{
					this->MarioSetOnGround();
				}
			}
			else if (e->obj->getTypeObject() == Type::QUESTIONBRICK)
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
			else if (e->obj->getTypeObject() == Type::WEAKBRICK)
			{
				if (e->ny > 0)
				{
					WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(e->obj);

					this->vy = 0;
					if (weakBrick->GetHealth() == 0)
					{
						break;
					}

					if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_EMPTY_ITEM)
					{
						if (this->level == MARIO_LEVEL_SMALL)
						{
							e->obj->SetState(WEAK_STATE_JUMP);
						}
						else
						{
							e->obj->SetState(WEAK_STATE_DESTROY);
						}
					}
					else
					{
						weakBrick->isTouchable = true;
						if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_P_SWITCH)
						{
							weakBrick->SetState(WEAK_STATE_EMPTY_ANI);
						}
						else
						{
							weakBrick->SetState(WEAK_STATE_JUMP);
						}
						
					}
				}
			}
			else if (e->obj->getTypeObject() == Type::COLORBOX)
			{
				if (e->ny < 0)
				{
					this->vy = 0;
					this->isOnGround = true;
					OffPreventSpamSpace();
					blockJumping = false;
				}
				else if (e->ny > 0)
				{
					y += vy * dt;
				}
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			else if (e->obj->getTypeObject() == Type::PIPE)
			{

				if (e->ny < 0)
				{
					Pipe* pipe = dynamic_cast<Pipe*>(e->obj);
					if (pipe->isHaveHiddenMap && this->isDownPressed)
					{
						vy = 0.01f;
						y += dy * 2;
						this->MarioSlideIntoPipe();
						storePosPipe_HaveHiddenMap(pipe->x, pipe->y);
					}
					this->isOnGround = true;
				}
				else if (e->ny > 0)
				{
					Pipe* pipe = dynamic_cast<Pipe*>(e->obj);
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
			else if (e->obj->getTypeObject() == Type::CBRICK)
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
			else if (e->obj->getTypeObject() == Type::CLOUDBRICK)
			{
				if (e->ny > 0)
				{
					y += vy * dt;
				}
				else if (e->ny < 0)
				{
					vy = 0;
					this->isOnGround = true;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents_static_Obj.size(); i++) delete coEvents_static_Obj[i];
#pragma endregion

#pragma region dynamic_item Collision
	vector<LPCOLLISIONEVENT> coEvents_dynamic_Item;
	vector<LPCOLLISIONEVENT> coEventsResult_dynamic_Item;

	coEvents_dynamic_Item.clear();
	//coEventsResult_dynamic_Item.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(dynamic_item, coEvents_dynamic_Item);


	// No collision occured, proceed normally
	
	if (coEvents_dynamic_Item.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents_dynamic_Item, coEventsResult_dynamic_Item, min_tx, min_ty, nx, ny, rdx, rdy);
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0;

		if (ny != 0) {
			vy = 0;
			isEnteredFirstSpaceUp = false;
			isPreventedSpamSpace = false;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult_dynamic_Item.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult_dynamic_Item[i];
			
			if (e->obj->getTypeObject() == Type::P_SWITCH)
			{
				if (e->ny < 0)
				{
					P_Switch* p_switch = dynamic_cast<P_Switch*>(e->obj);
					this->MarioSetOnGround();
					if (!p_switch->isActived)
					{
						p_switch->isActived = true;
						this->vy = -MARIO_ELASTIC;
					}

				}
				else if (e->nx != 0)
				{
					vx = 0;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents_dynamic_Item.size(); i++) delete coEvents_dynamic_Item[i];
#pragma endregion

#pragma region enemies
	vector<LPCOLLISIONEVENT> coEvents_Enemies;
	vector<LPCOLLISIONEVENT> coEventsResult_Enemies;

	coEvents_Enemies.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(enemies, coEvents_Enemies);

	// No collision occured, proceed normally
	if (coEvents_Enemies.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents_Enemies, coEventsResult_Enemies, min_tx, min_ty, nx, ny, rdx, rdy);
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0)
		{
			y -= min_ty * dy + ny * 0.1f;
		}
		if (ny != 0) {
			isEnteredFirstSpaceUp = false;
			isPreventedSpamSpace = false;
			if (ny < 0)
			{ 
				vy = 0;
				y += min_ty * dy + ny * 0.1f;
			}
			else if (ny > 0)
			{
				y -= min_ty * dy + ny * 0.8f;
			}
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult_Enemies.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult_Enemies[i];
			if (e->obj->getTypeObject() == Type::GOOMBA)
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
						goomba->y = goomba->y - GOOMBA_BBOX_HEIGHT_DIE - temp;
						goomba->subHealth();
						goomba->addPointToItem();
					}
					vy = -MARIO_ELASTIC;
				}
				if (e->nx != 0 || e->ny > 0 )
				{
					MarioHitEnemy();
				}
			}
			else if (e->obj->getTypeObject() == Type::KOOPAS)
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
				{
					if (e->ny > 0 || e->nx != 0)
					{
						MarioHitEnemy();
						y -= min_ty * dy + ny * 0.1f;
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
					if (isAttacking && (e->ny != 0 || e->nx != 0))
					{
						isCanHoldingKoopas = true;
						koopas->SetState(KOOPAS_STATE_SHELL_OUT_CONTROL);
					}
					else if (!isAttacking && (e->ny != 0 || e->nx != 0))
					{
						ChainKickKoopas(koopas, e->ny != 0);
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
							//koopas->setKoopasWasHeldByPlayer();
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
								this->SetLevel(--level);
								StartUntouchable();
							}
						}

					}
				}
				else if (koopas->GetState() == KOOPAS_STATE_HAVE_WING_WALKING ||
					koopas->GetState() == KOOPAS_STATE_HAVE_WING_JUMPING)
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
			else if (e->obj->getTypeObject() == Type::FLOWER)
			{
				if (e->nx != 0 || e->ny != 0)
				{
					if (AABBCollision(e->obj))
					{
						this->MarioHitEnemy();
					}
				}
			}
			else if (e->obj->getTypeObject() == Type::BOOMERANGBROTHER)
			{
				if (e->ny != 0)
				{
					if (e->ny > 0)
					{
						MarioHitEnemy();
					}
					else if (e->ny < 0)
					{
						BoomerangBrother* brother = dynamic_cast<BoomerangBrother*>(e->obj);
						brother->SetState(BOOMERANG_BROTHER_STATE_DEATH);
						brother->subHealth();
						brother->addPointToItem();
						vy = -MARIO_ELASTIC;
					}
				}
				else if (e->nx != 0)
				{
					MarioHitEnemy();
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents_Enemies.size(); i++) delete coEvents_Enemies[i];
#pragma endregion

#pragma region COLLISION WITH PSTOP OR PORTAL GAME
	if (listPortalStop != NULL)
	for (size_t i = 0; i < listPortalStop->size(); i++)
	{
		if (this->AABBCollision(listPortalStop->at(i)))
		{
			if (listPortalStop->at(i)->getTypeObject() == Type::PORTALSTOP)
			{
				if (i != lastPortalStopIndex)
				{
					PortalStop* pStop = dynamic_cast<PortalStop*>(listPortalStop->at(i));
					this->isMovedToLeft = pStop->isMovedLeft == 1;
					this->isMovedToTop = pStop->isMovedTop == 1;
					this->isMovedToRight = pStop->isMovedRight == 1;
					this->isMovedToBottom = pStop->isMovedBottom == 1;

					if (nx == 1)
					{
						if (x > listPortalStop->at(i)->x)
						{
							this->x = listPortalStop->at(i)->x;
							vx = 0;
							lastPortalStopIndex = i;
						}
					}
					else if (nx == -1)
					{
						if (x < listPortalStop->at(i)->x)
						{
							this->x = listPortalStop->at(i)->x;
							vx = 0;
							lastPortalStopIndex = i;
						}
					}
					if (ny == 1)
					{
						if (y > listPortalStop->at(i)->y)
						{
							this->y = listPortalStop->at(i)->y;
							vy = 0;
							lastPortalStopIndex = i;
						}
					}
					else if (ny == -1)
					{
						if (y < listPortalStop->at(i)->y)
						{
							this->y = listPortalStop->at(i)->y;
							vy = 0;
							lastPortalStopIndex = i;
						}
					}
					if (lastPortalStopIndex == i) 
						isWalking = false;
				}
			}
			else if (listPortalStop->at(i)->getTypeObject() == Type::PORTAL)
			{
				if (this->isCanGetIntoWorldMap)
				{
					CPortal* portal = dynamic_cast<CPortal*>(listPortalStop->at(i));
					portalStop->SetPosition(portal->x, portal->y);

					this->SwitchScreen();
					StoreScenceId(portal->GetSceneId());
				}
			}
		}

	}


#pragma endregion


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
		if (mario->isOnGround == false)
		{
			mario->onSitting = false;
		}
		if (mario->nx == 1)
		{
			if (mario->onSitting == true && mario->isWalking == false)
			{
				mario->ani = listAnimationForResMario.at(0);
			}
			else {
				mario->ani = CalcRollBackAndWalking(listAnimationForResMario.at(4), listAnimationForResMario.at(5), listAnimationForResMario.at(20), mario);
				if (!mario->isCanHoldingKoopas)
				{
					if (mario->isOnGround && mario->isMaxSpeed && mario->vx >= MARIO_RUNNING_MAX_SPEED)
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
		else if (!mario->isFalling && !mario->isOnGround && mario->isMaxSpeed)
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
						else if (mario->isMaxSpeed && !mario->isOnGround && mario->isFalling
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
						else if(mario->isMaxSpeed && !mario->isOnGround && mario->isFalling
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
	if (mario->isSlidingInPipe)
	{
		mario->ani = listAnimationForResMario.at(24);
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
			
			if (nx>0) 
			{
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				if (this->isCanHoldingKoopas)
				{
					ani = MARIO_ANI_SMALL_HOLD_JUPMING_KOOPAS_RIGHT;
				}
			}
			else if (nx < 0)
			{
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
				if (this->isCanHoldingKoopas)
				{
					ani = MARIO_ANI_SMALL_HOLD_JUPMING_KOOPAS_LEFT;
				}
			}
		}
		else if (vx != 0)
		{
			if (nx == 1)
			{
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_RIGHT, MARIO_ANI_SMALL_WALKING_RIGHT, MARIO_ANI_SMALL_WALKING_HOLD_KOOPAS_RIGHT, this);
				if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
				{
					ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
				}
			}
			else if (nx == -1)
			{
				ani = CalcRollBackAndWalking(MARIO_ANI_SMALL_WALKING_STOP_LEFT, MARIO_ANI_SMALL_WALKING_LEFT, MARIO_ANI_SMALL_WALKING_HOLD_KOOPAS_LEFT , this);
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
				if (!this->isCanHoldingKoopas)
				{
					if (isRunning && vx >= MARIO_RUNNING_MAX_SPEED)
					{
						ani = MARIO_ANI_SMALL_RUNNING_FLYING_RIGHT;
					}
					else
						ani = MARIO_ANI_SMALL_FLYING_RIGHT;
				}
				else if (this->isCanHoldingKoopas)
				{
					ani = MARIO_ANI_SMALL_HOLD_JUPMING_KOOPAS_RIGHT;
				}

			} 
			else if ( nx == -1 )
			{
				if (!this->isCanHoldingKoopas)
				{
					if (isRunning && vx <= -MARIO_RUNNING_MAX_SPEED)
					{
						ani = MARIO_ANI_SMALL_RUNNING_FLYING_LEFT;
					}
					else
						ani = MARIO_ANI_SMALL_FLYING_LEFT;
				}
				else if (this->isCanHoldingKoopas)
				{
					ani = MARIO_ANI_SMALL_HOLD_JUPMING_KOOPAS_LEFT;
				}

			}
		}
		if (this->isInWorldSelectionMap)
		{
			ani = MARIO_ANI_SMALL_WALKING_IN_SELECTION_WOLRD;
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
		if (!isCanHoldingKoopas && this->GetState() != MARIO_STATE_AUTO_GO)
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
		if (this->isInWorldSelectionMap)
		{
			ani = MARIO_ANI_BIG_WALKING_IN_SELECTION_WOLRD;
		}
	}
	else if (level == MARIO_LEVEL_BIG_TAIL)
	{
		//constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this,constant->listAni_Mario_Big);
		if (!isCanHoldingKoopas && this->GetState() != MARIO_STATE_AUTO_GO)
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
			else if (isKeepJumpingHigher && isMaxSpeed && !isOnGround && isEnteredFirstSpaceUp )
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
		if (this->isInWorldSelectionMap)
		{
			ani = MARIO_ANI_BIG_HAVE_TAIL_WALKING_IN_SELECTION_WOLRD;
		}
	}
	else if (level == MARIO_LEVEL_BIG_FIRE)
	{
		//constant->changeLevelMario(level);
		ani = CalcRenderForMARIO_BIG(this, constant->listAni_Mario_Big);
		if (!isCanHoldingKoopas && this->GetState() != MARIO_STATE_AUTO_GO)
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
		if (this->isInWorldSelectionMap)
		{
			ani = MARIO_ANI_BIF_FIRE_WALKING_IN_SELECTION_WOLRD;
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	
	animation_set->at(ani)->Render(x, y, alpha);

	if (isAttacking && level == MARIO_LEVEL_BIG_TAIL)
	{
		Tail->Render();
	}

	//RenderBoundingBox();
}
void CMario::SetState(int state)
{
	CGameObject::SetState(state);


	switch (state)
	{
	case MARIO_STATE_BLOCK_STATE_END_GAME:
		vx = 0;
		vy = 0;
		nx = 1;
		isEndMap = true;
		break;
	case MARIO_SELECTION_MAP_STATE_MOVE_RIGHT:
		vx = MARIO_SELECTION_MAP_MOVE_SPEED;
		nx = 1;
		isWalking = true;
		break;
	case MARIO_SELECTION_MAP_STATE_MOVE_LEFT: 
		vx = -MARIO_SELECTION_MAP_MOVE_SPEED;
		nx = 1;
		isWalking = true;
		break;
	case MARIO_SELECTION_MAP_STATE_MOVE_BOTTOM:
		vy = MARIO_SELECTION_MAP_MOVE_SPEED;
		ny = 1;
		isWalking = true;
		break;
	case MARIO_SELECTION_MAP_STATE_MOVE_TOP: 
		vy = -MARIO_SELECTION_MAP_MOVE_SPEED;
		ny = -1;
		isWalking = true;
		break;
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
				//isMaxSpeed = false;
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
				//isMaxSpeed = false;
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
			if (nx == 1)
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT)->StartRenderAnimation();
			}
			if (nx == -1 && animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->IsRenderOver(150))
			{
				animation_set->at(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT)->StartRenderAnimation();
			}
		}
		break;
	case MARIO_STATE_BIG_TAIL_JUMP_HIGHER:
	{
		
		break;
	}
	case MARIO_STATE_JUMP:
			if (isOnGround && !blockJumping)
			{
				if (isRunning && fabs(vx) >= MARIO_RUNNING_MAX_SPEED)
				{
					vy = -MARIO_JUMP_MAX_SPEED_Y;
					//vx = MARIO_MAX_WALKING_SPEED*nx;
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


void CMario::GetBoundingBoxForRespectiveLevel(float& left, float& top, float& right, float& bottom, int level, int nx)
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		left = left + 11;
		top = top + 14;
		if (nx == 1)
		{
			right = left + MARIO_SMALL_BBOX;
		}
		else if (nx == -1)
		{
			right = left + MARIO_SMALL_BBOX - 2;
		}
		bottom = top + MARIO_SMALL_BBOX;
		break;
	case MARIO_LEVEL_BIG:
		top = top + 3;
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
		if (nx > 0)
		{
			left = left + 10;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}
		else
		{
			left = left + 10;
			right = left + MARIO_BIG_BBOX_WIDTH - 2;
		}
		break;
	case MARIO_LEVEL_BIG_TAIL:
		top = top + 2;
		bottom = top + MARIO_BIG_TAIL_BBOX_HEIGHT;
		right = left + MARIO_BIG_TAIL_BBOX_WIDTH;
		if (nx > 0)
		{
			left = left + 11;
			right = left + MARIO_BIG_TAIL_BBOX_WIDTH - 2;
		}
		else
		{
			left = left + 8;
			right = left + MARIO_BIG_TAIL_BBOX_WIDTH;
		}
		break;
	case MARIO_LEVEL_BIG_FIRE:
		top = top + 3;
		right = left + MARIO_FIRE_BBOX_WIDTH;
		bottom = top + MARIO_FIRE_BBOX_HEIGHT;
		if (nx > 0)
		{
			left = left + 11;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}
		else
		{
			left = left + 9;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}
	}

	if ((this->onSitting))
	{
		top = top + 11;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (!this->isInWorldSelectionMap)
	{
		GetBoundingBoxForRespectiveLevel(left,top,right,bottom,level,nx);
	}
	else
	{
		right = x + 17;
		bottom = y + 22;
	}
	this->BBox_TopOfMario = top;  //top cua bbox
	this->BBox_LeftOfMario = left;
	this->BBox_BottomOfMario = bottom;
	this->BBox_RightOfMario = right;
}

void CMario::CollideWithCoinTransform(vector<LPGAMEOBJECT> listCoinTransform, vector<LPGAMEOBJECT> listWeakBrick)
{
	for (size_t i = 0; i < listCoinTransform.size(); i++)
	{
		if (this->AABBCollision(listCoinTransform[i]))
		{
			if (listCoinTransform[i]->isTouchable)
			{
				if (listCoinTransform[i]->getTypeObject() == Type::COIN)
				{
					this->IncreScoreMario();
				}
				listCoinTransform[i]->setObjDisappear();
				listWeakBrick[i]->setObjDisappear();
				// Increase Poin of Player
			}
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
	
	EffectPoint* effect = new EffectPoint();

	if (item->getTypeObject() == Type::SUPER_MUSHROOM)
	{
		Mushroom* mushroom = static_cast<Mushroom*>(item);

		if (mushroom->kindOfMushroom == KIND_RED_MUSHROOM)
		{
			this->SetLevel(++this->level);
		}
		else if (mushroom->kindOfMushroom == KIND_GREEN_MUSHROOM)
		{
			this->health++;
			effect->SetState(EFFECT_STATE_7UP);
			this->scores-=100;

		}
	}
	else if (item->getTypeObject() == Type::SUPER_LEAF)
	{
		this->SetLevel(++this->level);
	}

	listEffect.push_back(effect);
	effect->SetPosition(item->x, item->y);
	this->increScores();
}

void CMario::CollideWithItem(vector<LPGAMEOBJECT> items, vector<LPGAMEOBJECT>& listEffect)
{
	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->getTypeObject() == Type::P_SWITCH || 
			items[i]->GetState() == ITEM_STATE_COIN_JUMP)
		{
			continue;
		}
		if (this->AABBCollision(items[i]))
		{
			if (items[i]->isTouchable)
			{
				if (items[i]->getTypeObject() == Type::RANDOMITEM)
				{
					RandomItem* radomizeItem = dynamic_cast<RandomItem*>(items[i]);
					items[i]->SetState(ITEM_STATE_MOVE_UP);
					this->SetState(MARIO_STATE_BLOCK_STATE_END_GAME);
					this->listItemCollected.push_back(radomizeItem->typeItemCollected);
					radomizeItem->isTouchable = false;
					timeToShowEndGameText = new Timer(3000);
					timeToShowEndGameText->Start();
;					continue;
				}
				else if ((items[i]->GetState() == ITEM_STATE_SPECIAL_ITEM || items[i]->GetState() == ITEM_STATE_LEAF_FALL))
				{
					playerHittingSpecialItem(items[i], listEffect);
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
	SetPosition(x,y);
	CGame::GetInstance()->SetCamPos(x, y);
	SetSpeed(0, 0);
}