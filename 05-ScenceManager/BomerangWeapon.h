#pragma once
#include "Enemy.h"

#define BOOMERANG_SPEED_X						0.06f
#define BOOMERANG_SPEED_Y						0.015f
#define INERTIA_SPEED_X							0.04f



#define DECELERATE_SPEED						0.0009175f

#define BOOMERANG_STATE_MOVE_HORIZONTAL			1
#define BOOMERANG_STATE_MOVE_VERTICAL			2
#define BOOMERANG_STATE_MOVE_NO_GRAVITY			3
#define BOOMERANG_STATE_PREPARING_THROWING		4

#define BOOMERANG_ANI_BOOMERANG_PREPARING_THROWING_LEFT			0
#define BOOMERANG_ANI_BOOMERANG_PREPARING_THROWING_RIGHT		1
#define BOOMERANG_ANI_BOOMERANG_THROWING						2

class BoomerangWeapon : public Enemy
{
public:
	bool isPreTurningBack = false;
	bool isTurningBack = false;
	bool isCurvedBack = false;

	int ny;
	float tempVx = -1;

	// Amplitude Axis-y of Boomerang
	float amplitudeOy;

	// this flat express this boomerang have been holding yet
	bool isAimToStatus = false;

	BoomerangWeapon();

	
	void storeAmplitydeOy(float y);
	void reduceTempVelocity();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

