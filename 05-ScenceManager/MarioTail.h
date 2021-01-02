#pragma once
#include "MarioWeapon.h"

#define TAIL_BBOX_WIDTH			9
#define TAIL_BBOX_HEIGHT		6

class MarioTail :
	public MarioWeapon
{
public:
	vector<LPGAMEOBJECT> listEffect;
	int ny;

	MarioTail();
	void TurnOffDamageable();
	void TurnOnDamageable();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj , vector<LPGAMEOBJECT>* Enemies);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void CreateEffect();
};
