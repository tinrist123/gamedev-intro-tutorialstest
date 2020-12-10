#pragma once
#include "MarioTail.h"
#include "EffectHitTail.h"
#include "Koopas.h"
#include "Goomba.h"
#include "WeakBrick.h"

MarioTail::MarioTail()
{
	this->type = 06;
}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (LPGAMEOBJECT effect : listEffect)
	{
		if (effect->objectDisappear())
			listEffect.erase(listEffect.begin());
	}

	for (LPGAMEOBJECT effect : listEffect)
		effect->Update(dt, coObjects);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (this->AABBCollision(e))
		{
			if (e->category == Category::ENEMY)
			{
				if (e->getTypeObject() == Type::KOOPAS)
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e);
					CreateEffect();
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						e->subHealth();
						koopas->addPointToItem();
					}
					//KOOPAS_HIT_BY_WEAPON_MARIO = 11
					e->nx = (this->x - e->x < 0)?1:-1;
					e->SetState(KOOPAS_STATE_HIT_BY_WEAPON_MARIO);
				}
				else if (e->getTypeObject() == Type::GOOMBA)
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e);
					CreateEffect();
					if (goomba->GetState() == GOOMBA_STATE_WALKING 
						|| goomba->GetState() == GOOMBA_STATE_WALKING_WITH_WING
						|| goomba->GetState() == GOOMBA_STATE_PRE_JUMPING
						|| goomba->GetState() == GOOMBA_STATE_JUMPING
						)
					{
						goomba->addPointToItem();
					}
					e->nx = (this->x - e->x < 0) ? 1 : -1;
					e->SetState(GOOMBA_STATE_HIT_BY_WEAPON);
					e->subHealth();

				}
			}
			else if (e->getTypeObject() == Type::WEAKBRICK)
			{
				WeakBrick* weakbrick = dynamic_cast<WeakBrick*>(e);
				weakbrick->SetState(WEAK_STATE_DESTROY);
			}
			else if (e->getTypeObject() == Type::QUESTIONBRICK)
			{
				QuestionBrick* questionbrick = dynamic_cast<QuestionBrick*>(e);
				if (questionbrick->health != 0)
					questionbrick->SetState(1);
			}
		}
	}

}

void MarioTail::Render()
{
	for (LPGAMEOBJECT effect : listEffect)
		effect->Render();

	RenderBoundingBox();
}

void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isBoundingBox)
		return;

	if (nx == -1)
	{
		left = x - TAIL_BBOX_WIDTH - 16;
		right = x ;

	}
	else if (nx == 1)
	{
		left = x;
		right = x + TAIL_BBOX_WIDTH;

	}
	top = y;
	bottom = y + TAIL_BBOX_HEIGHT;
}

void MarioTail::CreateEffect()
{
	EffectHitTail* effect = new EffectHitTail();
	if (nx == -1)
	{
		effect->SetPosition(this->x - 25 - TAIL_BBOX_WIDTH, this->y - 8);
	}
	else if (nx == 1)
	{
		effect->SetPosition(this->x, this->y - 8);
	}
	effect->nx = nx;
	listEffect.push_back(effect);
}