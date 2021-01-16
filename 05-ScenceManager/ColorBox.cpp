#include "ColorBox.h"


ColorBox::ColorBox() 
{
}

ColorBox::ColorBox(int width, int height) 
{
	this->width = width;
	this->height = height;

	this->type = Type::COLORBOX;
	this->category = Category::GROUND;
}

void ColorBox::Render()
{
	//RenderBoundingBox();
}

void ColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH*width;
	b = y + BBOX_HEIGHT*height;
}