#pragma once
#include "ListEffects.h"

#define EFFECT_STATE_7UP		9


#define EFFECT_ANI_7UP			8

class EffectPoint: public ListEffects
{
public:
	bool isCreateEffect = false;
public:
	void CreateEffect() { isCreateEffect = true; }
	EffectPoint();
	~EffectPoint();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void UpdateOrDeleteEffect(DWORD dt, vector<LPGAMEOBJECT> *coObjects, EffectPoint *&effectPoint)
	{
		
		if (effectPoint->objectDisappear())
		{
			effectPoint = NULL;
		}
		else
		{
			effectPoint->Update(dt, coObjects);
		}
		return;
	}
};

