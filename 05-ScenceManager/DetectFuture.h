#pragma once
#include "GameObject.h"
class DetectFuture : public CGameObject
{


public:
	void UpdatePosition(float x, float y);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

