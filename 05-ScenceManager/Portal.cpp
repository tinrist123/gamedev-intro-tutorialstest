#include "Portal.h"


CPortal::CPortal(int quantityW,int quantityH,int scene_id )
{
	this->scene_id = scene_id;
	this->quantity_height = quantityH;
	this->quantity_width = quantityW;

	this->type = Type::PORTAL;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BBOX_BIT * quantity_width;
	b = y + BBOX_BIT * quantity_height;
}