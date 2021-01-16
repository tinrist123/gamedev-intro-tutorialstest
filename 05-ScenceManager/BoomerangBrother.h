#pragma once
#include "Enemy.h"
#include "Timer.h"

#define BOOMERANG_BROTHER_WALKING_SPEED_X					0.03f
#define BOOMERANG_BROTHER_WALKING_SPEED_Y					0.1f
#define BOOMERANG_BROTHER_JUMP_DIE							0.15f

#define BOOMERANG_BROTHER_STATE_WALKING_OR_ATTACKING		100
#define BOOMERANG_BROTHER_STATE_PREPARING_ATTACK			200
#define BOOMERANG_BROTHER_STATE_DEATH						300

#define BOOMERANG_BROTHER_ANI_WALKING_RIGHT					0
#define BOOMERANG_BROTHER_ANI_WALKING_LEFT					1
#define BOOMERANG_BROTHER_ANI_PREPARE_ATTACK_RIGHT			2
#define BOOMERANG_BROTHER_ANI_PREPARE_ATTACK_LEFT			3
#define BOOMERANG_BROTHER_ANI_DEATH_RIGHT					4
#define BOOMERANG_BROTHER_ANI_DEATH_LEFT					5

#define BOOMERANG_BROTHER_MAXIMUM_RANGE						32

class BoomerangBrother: public Enemy
{
public:
	int countBoomerangWeapon = 1;
	Timer* DelayGetBoomerangTime = new Timer(100);
	Timer* WaitingAnotherShootingBmrTime = new Timer(1200);
	Timer* loadingShootingTime = new Timer(400);
	bool getAllBoomerang = true;

	float distanceMoving = 0;
	bool isCanHoldingBoomerang = false;
public:
	void CollideWithBoomerang(vector<LPGAMEOBJECT>* listFireBall);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	BoomerangBrother();
	~BoomerangBrother();
};

