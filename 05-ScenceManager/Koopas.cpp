#include "Koopas.h"
#include "Goomba.h"
#include "WeakBrick.h"


CKoopas::CKoopas() : Enemy()
{
}

CKoopas::CKoopas(float start_x,float start_y,int typeKoopas, int typeColorKoopas)
{
	this->start_x = start_x;
	this->start_y = start_y;
	this->start_nx = -1;


	this->type = Type::KOOPAS;

	this->TypeKoopas = typeKoopas;
	this->TypeColorKoopas = typeColorKoopas;
	nx = -1;

	

	autoLoadAni();
	switch (this->TypeKoopas)
	{
	case KOOPAS_TYPE_HAVE_WING:
		this->start_state = KOOPAS_STATE_HAVE_WING_JUMPING;
		SetState(KOOPAS_STATE_HAVE_WING_JUMPING);
		health++;
		break;
	case KOOPAS_TYPE_NORMAL:
		this->start_state = KOOPAS_STATE_WALKING;

		SetState(KOOPAS_STATE_WALKING);
		break;
	case KOOPAS_TYPE_HAVE_WING_WALKING:
		this->start_state = KOOPAS_STATE_HAVE_WING_WALKING;

		SetState(KOOPAS_STATE_HAVE_WING_WALKING);
		health++;
		break;
	case KOOPAS_TYPE_HAVE_WING_FLYING:
		this->start_state = KOOPAS_STATE_HAVE_WING_WALKING;
		SetState(KOOPAS_STATE_HAVE_WING_WALKING);
		health++;
		break;
	default:
		break;
	}
	health++;
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
	if (!isBoundingBox)
	{
		left = top = right = bottom;
	}
	else
	{
		left = x;
		if (state == KOOPAS_STATE_SHELL_MOVING
			||
			state == KOOPAS_STATE_PRE_REVIE
			||
			state == KOOPAS_STATE_PRE_REVIE_2
			||
			state == KOOPAS_STATE_SHELL_OUT_CONTROL
			||
			state == KOOPAS_STATE_SHELL
			)
		{
			top = y + 9;
		}
		else {
			top = y;
		}
		right = x + KOOPAS_BBOX_WIDTH;

		if (state == KOOPAS_STATE_DIE)
			bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
		else
			bottom = y + KOOPAS_BBOX_HEIGHT + 2;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	if (pointEff )
	{
		pointEff->UpdateOrDeleteEffect(dt, coObjects, pointEff);
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
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_OUT_CONTROL 
		)
	{
		if (CheckTimeKoopasPreReive())
		{
			SetState(KOOPAS_STATE_PRE_REVIE);
		}
	}
	ExecuteRevingKoopas();

	// Detect Koopas in next Update to calc whether koopas fall



	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally
	
	if (CheckKoopasNeverFalling() && this->TypeColorKoopas == KOOPAS_RED_TYPE)
	{
		if (this->nx == 1 && firstTime)
		{
			if (!this->isGround(this->x + KOOPAS_BBOX_WIDTH - 4, this->y + KOOPAS_BBOX_HEIGHT + 3, *coObjects))
			{
				this->nx = -1;
				vx = fabs(this->vx) * nx;
			}
		}
		else if (this->nx == -1 && firstTime)
		{
			if (!this->isGround(this->x - 1, this->y + KOOPAS_BBOX_HEIGHT + 5, *coObjects))
			{
				this->nx = 1;
				vx = fabs(this->vx);
			}
		}
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
		DebugOut(L"%f\n", this->vx);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{			
				if (e->ny < 0)
				{
					if (!firstTime)
					{
						firstTime = true;
					}
					if (state == KOOPAS_STATE_HAVE_WING_JUMPING)
					{	
						vy = -0.2f;
					}
					else if (state == KOOPAS_STATE_HIT_BY_WEAPON_MARIO) {
						SetState(KOOPAS_STATE_SHELL);
					} 
					else
					{
						vy = 0;
					}
				}
				else if (e->nx != 0)
				{
					this->nx = -this->nx;
					this->vx = fabs(this->vx) * this->nx;
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->ny > 0)
				{
					y += dy;
				}
				else if (e->ny < 0)
				{
					if (!firstTime)
					{
						firstTime = true;
					}
					if (state == KOOPAS_STATE_HAVE_WING_JUMPING)
					{
						vy = -0.2f;
					}
					else if (state == KOOPAS_STATE_HIT_BY_WEAPON_MARIO) {
						SetState(KOOPAS_STATE_SHELL);
					}
					else
					{
						vy = 0;
					}
				}
				if (e->nx != 0)
				{
					x += vx * dt;
				}
			}
			else if (dynamic_cast<QuestionBrick *>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
				}
			}
			else if (e->obj->getTypeObject() == Type::WEAKBRICK)
			{
				if (this->GetState() == KOOPAS_STATE_SHELL_MOVING)
				{
					WeakBrick* weakbrick = dynamic_cast<WeakBrick*>(e->obj);
					
					if (weakbrick->isHaveP_Switch)
					{
						weakbrick->SetState(WEAK_STATE_EMPTY_ANI);
					}
					else
					{
						weakbrick->SetState(WEAK_STATE_DESTROY);
					}
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
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_MOVING);
	listAnimationKoopas.push_back(KOOPAS_ANI_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_ANI_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_OVERTURNED);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_OVERTURNED_MOVE);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_FLYING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_ANI_HAVE_WING_FLYING_LEFT);
}

void CKoopas::CollideWithEnemies
(vector<LPGAMEOBJECT>* enemies, vector<LPGAMEOBJECT>* listEffect)
{
	for (size_t i = 0; i < enemies->size(); i++)
	{
		if (this->AABBCollision(enemies->at(i)))
		{
			EffectPoint* effectPoint = new EffectPoint();
			if (enemies->at(i)->getTypeObject() == Type::GOOMBA)
			{
				enemies->at(i)->SetState(GOOMBA_STATE_HIT_BY_WEAPON);
			}
			else if (enemies->at(i)->getTypeObject() == Type::FLOWER)
			{
				enemies->at(i)->setObjDisappear();
			}
			else if (enemies->at(i)->getTypeObject() == Type::KOOPAS )
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(enemies->at(i));
				if (this == koopas) continue;
				if (this->GetState() == KOOPAS_STATE_SHELL_MOVING)
				{
					this->health++;

					koopas->SetState(KOOPAS_STATE_HIT_BY_WEAPON_MARIO);
					koopas->SetNoCollision();
				}
			}
			EffectHitTail* effect = new EffectHitTail();
			effect->SetPosition(this->x, this->y);
			effectPoint->SetPosition(this->x, this->y);
			listEffect->push_back(effect);
			listEffect->push_back(effectPoint);
		}
	}
	
}

void CKoopas::AddAniForKoopasRedToList()
{
	listAnimationKoopas.clear();
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_MOVING);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_OVERTURNED);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE_2);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_WALKING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_WALKING_LEFT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_FLYING_RIGHT);
	listAnimationKoopas.push_back(KOOPAS_RED_ANI_HAVE_WING_FLYING_LEFT);
}
void CKoopas::Render()
{
	if (state == KOOPAS_STATE_HIT_BY_WEAPON_MARIO)
	{
		ani = listAnimationKoopas.at(6);
	}
	else if (state == KOOPAS_STATE_WALKING)
	{
		if (nx == 1 ) ani = listAnimationKoopas.at(0);
		else if (nx == -1) ani = listAnimationKoopas.at(1);
	}
	else if (state == KOOPAS_STATE_HAVE_WING_JUMPING)
	{
		if (nx == -1)
			ani = listAnimationKoopas.at(13);
		else if (nx == 1)
			ani = listAnimationKoopas.at(12);
	}
	else if (state == KOOPAS_STATE_HAVE_WING_WALKING)
	{
		if (nx == -1)
			ani = listAnimationKoopas.at(11);
		else if (nx == 1)
			ani = listAnimationKoopas.at(10);
	}
	else if (state == KOOPAS_STATE_PRE_REVIE)
	{
		if (isOverturned)
		{
			ani = listAnimationKoopas.at(8);
		}
		else if (!isOverturned)
		{
			ani = listAnimationKoopas.at(4);
		}
	}
	else if (state == KOOPAS_STATE_PRE_REVIE_2)
	{
		if (isOverturned)
		{
			ani = listAnimationKoopas.at(9);
		}
		else if (!isOverturned)
		{
			ani = listAnimationKoopas.at(5);
		}
	}
	else if (state == KOOPAS_STATE_SHELL)
	{
		if (isOverturned)
		{
			ani = listAnimationKoopas.at(6);
		}
		else if (!isOverturned)
		{
			ani = listAnimationKoopas.at(2);
		}
	}
	else if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		if (isOverturned)
		{
			ani = listAnimationKoopas.at(7);
		}
		else if (!isOverturned)
		{
			ani = listAnimationKoopas.at(3);
		}
	}
	animation_set->at(ani)->Render(x, y);
	if (pointEff)
	{
		pointEff->Render();
	}
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_HIT_BY_WEAPON_MARIO:
		vy = -KOOPAS_SHELL_JUMP_VY;
		vx = nx*0.1f;
		isOverturned = true;
		break; 
	case KOOPAS_STATE_DIE:
		y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = -KOOPAS_ANI_SHELL_MOVING_SPEED_X;
		isBoundingBox = false;
		break;
	case KOOPAS_STATE_SHELL:
		EnemyNoDamage();
		vx = 0;
		vy = 0;
		CountTimeToRevive();
		break;
	case KOOPAS_STATE_WALKING:
		isOverturned = false;
		EnemyDamage();
		isOutOfControl = false;
		vx = nx * KOOPAS_WALKING_SPEED
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		EnemyDamage();
		isOutOfControl = false;
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx *2;
		break;
	case KOOPAS_STATE_HAVE_WING_WALKING:
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		break;
	case KOOPAS_STATE_HAVE_WING_JUMPING:
		vx = 0.05f *nx;
		//vy = -0.10f;
		break;
	case KOOPAS_STATE_HAVE_WING_FLYING:

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