#pragma once
#include "GameObject.h"

#define SHAKE_TREE_SHORT	1
#define SHAKE_TREE_TALL		2


#define SHAKE_TREE_ANI		0

class ShakeTree : public CGameObject
{

public:
	bool isPushMarioOut;
	bool isInHiddenMap;
	int kindTreatmentPlayer;
public:
	ShakeTree();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

