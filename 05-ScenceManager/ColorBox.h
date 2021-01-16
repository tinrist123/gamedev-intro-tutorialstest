#pragma once
#include "GameObject.h"
#define BBOX_WIDTH	16
#define BBOX_HEIGHT	16

class ColorBox : public CGameObject
{
		int height;
		int width;
	public:
		ColorBox();
		ColorBox(int width,int height);
		virtual void Render();
		virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};

