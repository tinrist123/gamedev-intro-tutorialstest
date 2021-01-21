#pragma once
#include "GameObject.h"
class Sympol_3 : public CGameObject
{
public:
	Sympol_3();
	~Sympol_3();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

