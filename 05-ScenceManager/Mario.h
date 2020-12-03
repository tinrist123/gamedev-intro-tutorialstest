#pragma once
#include "GameObject.h"
#include "Constant.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Ground.h"
#include "Item.h"
#include "Portal.h"
#include <algorithm>
#include <assert.h>
#include "ColorBox.h"
#include "Game.h"
#include "Utils.h"
#include "Pipe.h"
#include "Koopas.h"
#include "FireBullet.h"
#include "Flower.h"
#include "MarioTail.h"
#include "Coin.h"

//#include "Scence.h"


class CMario : public CGameObject
{
public:
	int level = 1;
	int setTimeRenderingAni = 100;
	int untouchable;



	Constant* constant;
	DWORD timeReload = 0;
	DWORD autoReloadTime;
	DWORD timeFlyingForTail = 0;
	MarioTail* Tail = new MarioTail();

	bool isCanHoldingKoopas = false;
	bool isEnteredFirstSpaceUp = false;
	bool isKeepJumpingHigher = false;
	bool isFireShoot = false;
	bool onSitting = false;

	bool isFalling = true;
	bool blockJumping = false;
	bool isKeepJumping = false;

	bool isRollBack = false;
	bool isWalking = false;
	bool isTested = false;

	bool isPreventedSpamSpace = false;
	bool isMaxSpeed = false;

	int  lastNx;
	int  totalOfBulletsReadyForFiring = 2;

	bool isRunning = false;

	
	bool imMovable = true;
	bool isAttackPress = false;
	bool isKickedKoopas = false;
	void KickingKoopas() { isKickedKoopas = true; tDraw = GetTickCount64(); }


	float lastVx;
	float lastVy = 0.07;
	//CScene* s;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	DWORD tDraw = 0;
	bool isAttacking = false;
	bool isJumpingAttack = false;

	int DetectLevelSpeedMario();

	void ReduceVelocityWhenFly() {

		if (vx >= MARIO_RUNNING_MAX_SPEED && vx > MARIO_MAX_WALKING_SPEED)
		{
			vx -= MARIO_WALKING_DECELERATION;
		}
		else if (vx <= -MARIO_RUNNING_MAX_SPEED && vx < MARIO_MAX_WALKING_SPEED)
		{
			vx += MARIO_WALKING_DECELERATION;
		}
	}

	void OffPreventSpamSpace()
	{
		isEnteredFirstSpaceUp = false;
		isPreventedSpamSpace = false;
	}

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void MarioSetOnGround() { isOnGround = true; vy = 0; }

	void setTailPos()
	{
		Tail->SetPosition(this->x + 24, this->y + 18);
		Tail->nx = nx;
	}

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void ReloadBullets() { timeReload = GetTickCount64(); }
	bool IsReloadedBullets() { return GetTickCount64() - timeReload > 500; }

	void setAutoReload() { autoReloadTime = GetTickCount64(); }
	void resetAutoReload() { autoReloadTime = 0; }
	bool isReloaded() { return GetTickCount64() - autoReloadTime > 500; }

	void SetTimeFlyingForTail() { timeFlyingForTail = GetTickCount64(); }
	bool CheckExpiredFlyingForTail() { return GetTickCount64() - timeFlyingForTail > 2000;	}
	void ChainKickKoopas(CKoopas* &koopas,bool isElastic);

	void MarioHitEnemy();
	void AccurateCollisionWithEnemy(LPGAMEOBJECT enemies);

	void CollideWithEnemy(vector<LPENEMY> enemies);
	void playerHittingSpecialItem(Item*& item);
	void CollideWithItem(vector<LPITEM> items);
	void Reset();

	void MarioIsFalling() 
	{ 
		this->isFalling = true;
		this->isOnGround = false;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};