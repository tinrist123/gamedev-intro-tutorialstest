#pragma once
#include "GameObject.h"
class TripleBoxItem : public CGameObject
{
public:
	TripleBoxItem();
	void TripleBoxItem::Render();
	void TripleBoxItem::GetBoundingBox(float& l, float& t, float& r, float& b);
};

