#pragma once
#include "GameObject.h"

class PortalStop : public CGameObject
{
public:

	int quantityW;
	int quantityH;

	int isMovedLeft;
	int isMovedTop;
	int isMovedRight;
	int isMovedBottom;
public:
	PortalStop();
	~PortalStop();
	PortalStop(int, int,int,int,int,int);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

