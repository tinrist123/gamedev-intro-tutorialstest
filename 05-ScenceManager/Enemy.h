#pragma once
#include "GameObject.h"
#include "EffectPoint.h"

class Enemy;
typedef Enemy* LPENEMY;

class Enemy : public CGameObject
{
public:
	int score;
	int hp;

	EffectPoint* pointEff;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render() = 0;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	bool CheckOutCamera();
	void EnemyNoDamage() { isDamageable = false; }
	void EnemyDamage() { isDamageable = true; }

	void addPointToItem()
	{
		pointEff = new EffectPoint();
		pointEff->SetPosition(this->x, this->y);
	}

	Enemy();
	~Enemy();
};

