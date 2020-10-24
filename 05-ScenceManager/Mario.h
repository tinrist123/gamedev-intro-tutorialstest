#pragma once
#include "GameObject.h"
#include "Constant.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Ground.h"
#include "Item.h"
#include "Portal.h"
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
	bool isTested2 = false;

	bool isSpecialGravity = false;
	bool isPreventedSpamSpace = false;
	bool isMaxSpped = false;

	int  lastNx;
	int  totalOfBulletsReadyForFiring = 2;

	bool isRunning = false;

	float lastVx;
	float lastVy = 0.07;
	//CScene* s;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	DWORD tDraw = 0;
	bool isAttacking = false;
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void ReloadBullets() { timeReload = GetTickCount64(); }
	bool IsReloadedBullets() { return GetTickCount64() - timeReload > 500; }

	void setAutoReload() { autoReloadTime = GetTickCount64(); }
	void resetAutoReload() { autoReloadTime = 0; }
	bool isReloaded() { return GetTickCount64() - autoReloadTime > 500; }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};