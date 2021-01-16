#include "PortalStop.h"


PortalStop::PortalStop()
{
}

PortalStop::~PortalStop()
{
}

PortalStop::PortalStop(int W, int H,int l ,int t , int r,int b)
{
	this->type = Type::PORTALSTOP;

	this->quantityH = H;
	this->quantityW = W;

	isMovedLeft = l;
	isMovedTop = t;
	isMovedRight = r;
	isMovedBottom = b;

	this->category = Category::MAPOBJECT;
}

void PortalStop::Render()
{
	RenderBoundingBox();
}

void PortalStop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + quantityW * BBOX_BIT;
	b = y + quantityH * BBOX_BIT;
}
