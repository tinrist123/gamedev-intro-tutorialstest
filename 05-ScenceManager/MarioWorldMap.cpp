#include "MarioWorldMap.h"

MarioWorldMap::MarioWorldMap(float x, float y)
{
	this->x = x;
	this->y = y;
	this->type = Type::MARIOWORLDSELECTION;
}

void MarioWorldMap::Render()
{
	animation_set->at(0)->Render(x,y,255);
}

void MarioWorldMap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}

void MarioWorldMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void MarioWorldMap::SetState(int state)
{
}
