#pragma once
#include "GameObject.h"

#define VY_SPEED			0.04f


class ListEffects : public CGameObject
{
public:
	DWORD timeExist = 0;
	virtual void Render() = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	void startTimeAlive() { timeExist = GetTickCount64(); }
	bool checkDisApearEffect() { return GetTickCount64() - timeExist > 500 && timeExist != 0; }
};
typedef ListEffects *LPEFFECTS;
#define EFFECTS_DEFLECT			1

