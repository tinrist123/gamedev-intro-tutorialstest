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

	Constant* constant;
	int untouchable;
	bool onSitting = false;

	bool isFalling = true;
	bool blockJumping = false;
	bool isKeepJumping = false;

	bool isRollBack = false;
	bool isWalking = false;
	bool isTested = false;
	bool isTested2 = false;

	int  lastNx;

	bool isRunning = false;

	float lastVx;

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

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};