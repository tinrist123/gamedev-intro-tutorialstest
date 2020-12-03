#pragma once
#include "Enemy.h"
#include "Utils.h"
#include "ColorBox.h"
#include "Pipe.h"
#include "MarioWeapon.h"
#include "QuestionBrick.h"

#define KOOPAS_WALKING_SPEED 0.05f;

#define KOOPAS_TYPE_HAVE_WING										1
#define KOOPAS_TYPE_NORMAL											2
#define KOOPAS_TYPE_HAVE_WING_WALKING								3
#define KOOPAS_TYPE_HAVE_WING_FLYING								4


#define KOOPAS_BBOX_WIDTH											16
#define KOOPAS_BBOX_HEIGHT											26
#define KOOPAS_BBOX_HEIGHT_DIE										16

#define KOOPAS_STATE_WALKING										1
#define KOOPAS_STATE_DIE											2
#define KOOPAS_STATE_SHELL_MOVING									3
#define KOOPAS_STATE_SHELL											4
#define KOOPAS_STATE_HAVE_WING_WALKING								5
#define KOOPAS_STATE_HAVE_WING_FLYING								6
#define KOOPAS_STATE_REIVING										7
#define KOOPAS_STATE_SHELL_OUT_CONTROL								8
#define KOOPAS_STATE_PRE_REVIE										9
#define KOOPAS_STATE_PRE_REVIE_2									10
#define KOOPAS_STATE_HIT_BY_WEAPON_MARIO							11

#define KOOPAS_ANI_WALKING_LEFT										0
#define KOOPAS_ANI_WALKING_RIGHT									1
#define KOOPAS_ANI_SHELL											2
#define KOOPAS_ANI_SHELL_MOVING										3
#define KOOPAS_ANI_PRE_REVIE										4
#define KOOPAS_ANI_PRE_REVIE_2										5
#define KOOPAS_ANI_SHELL_OVERTURNED									6
#define KOOPAS_ANI_SHELL_OVERTURNED_MOVE							7
#define KOOPAS_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE					8
#define KOOPAS_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE_2				9
#define KOOPAS_ANI_HAVE_WING_WALKING_RIGHT							10
#define KOOPAS_ANI_HAVE_WING_WALKING_LEFT							11
#define KOOPAS_ANI_HAVE_WING_FLYING_RIGHT							12
#define KOOPAS_ANI_HAVE_WING_FLYING_LEFT							13


#define KOOPAS_RED_ANI_WALKING_LEFT									14
#define KOOPAS_RED_ANI_WALKING_RIGHT								15
#define KOOPAS_RED_ANI_SHELL										16
#define KOOPAS_RED_ANI_SHELL_MOVING									17
#define KOOPAS_RED_ANI_PRE_REVIE									18
#define KOOPAS_RED_ANI_PRE_REVIE_2									19
#define KOOPAS_RED_ANI_SHELL_OVERTURNED								20
#define KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE						21
#define KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE				22
#define KOOPAS_RED_ANI_SHELL_OVERTURNED_MOVE_PRE_REVIE_2			23
#define KOOPAS_RED_ANI_HAVE_WING_WALKING_RIGHT						24
#define KOOPAS_RED_ANI_HAVE_WING_WALKING_LEFT						25
#define KOOPAS_RED_ANI_HAVE_WING_FLYING_RIGHT						26
#define KOOPAS_RED_ANI_HAVE_WING_FLYING_LEFT						27


#define KOOPAS_ANI_WALKING_RIGHT_SPEED_X							0.1f
#define KOOPAS_ANI_SHELL_MOVING_SPEED_X								0.2f
#define KOOPAS_SHELL_JUMP_VY										0.2f

#define KOOPAS_GREEN_TYPE											1
#define KOOPAS_RED_TYPE												2


// TODO : KOOPAS
//	DONE	KOOPAS 2 loai mau
//	DONE	KOOPAS hoi sinh
//	DONE	KOOPAS WALKING
//	DONE	KOOPAS HAVE WING WALKING
//	DONE	KOOPAS HAVE WING JUMPING
//		KOOPAS MAU DO K BAO GIO ROT

class CKoopas : public Enemy
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void AddAniForKoopasRedToList();
	virtual void Render();

public:
	int TypeColorKoopas = KOOPAS_GREEN_TYPE;
	int TypeKoopas;
	DWORD revivalTime;

	
	int marioPositionX = 0;
	int marioPositionY = 0;

	bool firstTime = false;
	bool isPlayerHolding = false;
	bool isOutOfControl = false;
	bool isOverturned = false;

	vector <int> listAnimationKoopas;

	CKoopas(int typeKoopas, int typeColorKoopas);


	void autoLoadAni();

	void AddAniForKoopasGreenToList();

	void setKoopasWasHeldByPlayer() { isPlayerHolding = true; }
	void ExecuteRevingKoopas();
	bool CheckTimeKoopasPreReive() { return GetTickCount64() - revivalTime > 3800 && revivalTime != 0; }
	bool CheckTimeKoopasPreReive_2() { return GetTickCount64() - revivalTime > 4500 && revivalTime != 0; }
	bool CheckTimeKoopasRevive() { return GetTickCount64() - revivalTime > 5000 && revivalTime != 0; }
	void CountTimeToRevive() { revivalTime = GetTickCount64(); }
	void resetTimeToRevie() { revivalTime = 0; }

	bool CheckKoopasNeverFalling() {
		return (state == KOOPAS_STATE_WALKING);
	}

	void updatePositionKoopasByPositionMario(int x, int y) { this->marioPositionX = x; this->marioPositionY = y; }
	void IncreHealth() { health++; }
	
	virtual void SetState(int state);
};