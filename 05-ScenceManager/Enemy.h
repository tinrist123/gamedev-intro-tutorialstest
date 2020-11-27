#pragma once
#include "GameObject.h"
#include "EffectPoint.h"

class Enemy;
typedef Enemy* LPENEMY;

class Enemy : public CGameObject
{
public:
	EffectPoint* effectPoint;
	int score;
	int hp;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render() = 0;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	bool CheckOutCamera();
	void EnemyNoDamage() { isDamageable = false; }
	void EnemyDamage() { isDamageable = true; }

	void addPointToItem()
	{
		effectPoint = new EffectPoint();
		effectPoint->SetPosition(this->x, this->y);
	}

	Enemy();
	~Enemy();
};

