#pragma once
#include "GameObject.h"
class BoxItem : public CGameObject
{
public:
	BoxItem();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

