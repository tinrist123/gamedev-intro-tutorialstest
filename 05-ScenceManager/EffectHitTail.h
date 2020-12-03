#pragma once
#include "ListEffects.h"
class EffectHitTail : public ListEffects
{
public:
	EffectHitTail();
	~EffectHitTail();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

