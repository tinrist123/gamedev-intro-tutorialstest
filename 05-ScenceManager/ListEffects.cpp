#include "ListEffects.h"

void ListEffects::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	y += dy;
	if (checkDisApearEffect())
	{
		setObjDisappear();
	}
}
