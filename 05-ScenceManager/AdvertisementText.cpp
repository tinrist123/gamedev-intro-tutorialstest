#include "AdvertisementText.h"

AdvertisementText::AdvertisementText()
{
	this->type = Type::ADVERTISEMENT;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(20);
	SetAnimationSet(ani_set);
}

AdvertisementText::~AdvertisementText()
{
}

void AdvertisementText::Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj)
{
	CGameObject::Update(dt, staticObj);
	
	y += dy;
	x += dx;
}

void AdvertisementText::Render()
{
	animation_set->at(2)->Render(x, y);
}

void AdvertisementText::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
