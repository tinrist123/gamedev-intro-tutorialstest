#include "MapScence.h"

void MapScence::Render()
{
	animation_set->at(0)->Render(x, y);
}