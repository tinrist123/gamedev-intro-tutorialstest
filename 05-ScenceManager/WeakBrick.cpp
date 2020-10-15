#include "WeakBrick.h"
#include "../03-Keyboard-States/debug.h"

WeakBrick::WeakBrick(int width, int height) : CBrick::CBrick(width, height)
{
}

void WeakBrick::Render()
{
	animation_set->at(1)->Render(x, y);
	RenderBoundingBox();
}

void WeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16 * width;
	b = y + 16 * height;
}