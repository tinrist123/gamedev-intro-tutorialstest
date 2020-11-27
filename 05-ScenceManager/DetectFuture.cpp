#include "DetectFuture.h"
#include "Utils.h"

void DetectFuture::UpdatePosition(float x1, float y2)
{
	x = x1;
	y = y2;
}

void DetectFuture::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isBoundingBox)
	{
		left = right;
		top = bottom;
	}
	else
	{
		left = x;
		top = y;
		right = x + 2;
		bottom = y + 2;
	}
}

void DetectFuture::Render()
{
	RenderBoundingBox();
}
