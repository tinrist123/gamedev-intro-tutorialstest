#pragma once
#include "GameObject.h"
class AdvertisementText : public CGameObject
{
public:
	AdvertisementText();
	~AdvertisementText();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

