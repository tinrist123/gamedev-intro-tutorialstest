#pragma once
#include "ListEffects.h"

#define EFFECTS_DEFLECT			1
class EffectsFire :
    public ListEffects
{
public:
	EffectsFire();
	~EffectsFire();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


