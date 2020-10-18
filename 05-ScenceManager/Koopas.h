#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.1f;

#define KOOPAS_BBOX_WIDTH							16
#define KOOPAS_BBOX_HEIGHT							26
#define KOOPAS_BBOX_HEIGHT_DIE						16

#define KOOPAS_STATE_WALKING						1
#define KOOPAS_STATE_DIE							2
#define KOOPAS_STATE_SHELL_MOVING					3
#define KOOPAS_STATE_SHELL							4

#define KOOPAS_ANI_WALKING_LEFT						0
#define KOOPAS_ANI_WALKING_RIGHT					1
#define KOOPAS_ANI_DIE								2
#define KOOPAS_ANI_SHELL							3
#define KOOPAS_ANI_SHELL_MOVING						4

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
};