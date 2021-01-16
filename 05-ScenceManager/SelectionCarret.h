#pragma once
#include "GameObject.h"
class SelectionCarret : public CGameObject
{
public:
	SelectionCarret();
	~SelectionCarret();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

