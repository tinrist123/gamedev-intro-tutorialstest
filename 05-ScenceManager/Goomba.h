#pragma once
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED				0.05f;

#define GOOMBA_BBOX_DISTANCE_MAX_HEIGHT		8
#define GOOMBA_BBOX_WIDTH					20
#define GOOMBA_BBOX_WIDTH_BROWN_GOOMBA		16
#define GOOMBA_BBOX_HEIGHT					24
#define GOOMBA_BBOX_HEIGHT_DIE				9

#define GOOMBA_STATE_WALKING				100
#define GOOMBA_STATE_DIE					200
#define GOOMBA_STATE_PRE_JUMPING			300
#define GOOMBA_STATE_JUMPING				400
#define GOOMBA_STATE_WALKING_WITH_WING		500
#define GOOMBA_STATE_HIT_BY_WEAPON			600

#define GOOMBA_ANI_WALKING					0
#define GOOMBA_ANI_DIE						1
#define GOOMBA_ANI_BROWN_GOOMBA_SUPINE		7
///// GOOMBA RED
#define GOOMBA_RED_ANI_WALKING_WING			2
#define GOOMBA_RED_ANI_JUMPING				3
#define GOOMBA_RED_ANI_DIE					4
#define GOOMBA_RED_ANI_WALKING				5
#define GOOMBA_ANI_PARAGOOMBA_SUPINE		6


#define GOOMBA_LEVEL_BROWN					1
#define PARAGOOMBA							2


#define GOOMBA_STATE_JUMPING_VY				0.23f
#define GOOMBA_STATE_MINI_JUMPING_VY		0.08f


// TODO: Con goomba mau do
//	done	co 3 chuyen dong, di binh thuong, bay lac lien luc, bay lac len troi 
//	done	bi dap mat cai canh
//	done	Binh thuong giong goomba


class CGoomba : public Enemy
{
public:
	int nx = -1;
	int level = 1;
	DWORD timeWaitingDeath = 0;

	// Time var for ParaGoomba
	DWORD timeChangeState = 0;

	bool IsBlockingChangeState = false;
	bool firstTimeHighJump = false;

	void subLevel() { this->level--; }
	void followPlayerByNx(int nx) { this->nx = nx; }
	void blockingChangeState(bool a) { IsBlockingChangeState = a; }
	void resetTimeChangeState() { timeChangeState = 0; }
	void startTimeChangeState() { timeChangeState = GetTickCount64(); }
	bool changeStateTo_MiniJumpGoomba() { return GetTickCount64() - timeChangeState > 1000 && timeChangeState != 0; }
	bool changeStateTo_MaxJumpGoomba() { return GetTickCount64() - timeChangeState > 1500 && timeChangeState != 0; }


	void setTimeWaitingDeath() { timeWaitingDeath = GetTickCount64(); }
	bool checkTimeWaitingDeath() {
		return GetTickCount64() - timeWaitingDeath
	 > 300;
	}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public: 	
	CGoomba(float,float, int);
	virtual void SetState(int state);
};