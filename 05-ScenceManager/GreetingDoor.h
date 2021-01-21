#pragma once
#include "GameObject.h"
class GreetingDoor : public CGameObject
{
public:
	GreetingDoor();
	~GreetingDoor();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


};

