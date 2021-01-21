#include "GreetingDoor.h"

GreetingDoor::GreetingDoor()
{
	this->type = Type::GREETINGDOOR;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(20);
	SetAnimationSet(ani_set);
}


GreetingDoor::~GreetingDoor()
{
}

void GreetingDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj)
{
	CGameObject::Update(dt, staticObj);

	{
		x += dx;
		y += dy;
	}
}

void GreetingDoor::Render()
{
	animation_set->at(1)->Render(x, y );
}

void GreetingDoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
