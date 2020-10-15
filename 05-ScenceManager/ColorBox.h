#pragma once
#include "Ground.h"

#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16
class ColorBox : public Ground
{
		int height;
		int width;
	public:
		ColorBox(int width,int height);
		virtual void Render();
		virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};

