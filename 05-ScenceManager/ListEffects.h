#pragma once
#include "GameObject.h"

#define VY_SPEED			0.04f
#define EFFECTS_DEFLECT			1


class ListEffects : public CGameObject
{
public:
	DWORD timeExist = 0;
	virtual void Render() = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void startTimeAlive() { timeExist = GetTickCount64(); }
	bool checkDisApearEffect() { return GetTickCount64() - timeExist > 300 && timeExist != 0; }
};

typedef ListEffects *LPEFFECTS;
