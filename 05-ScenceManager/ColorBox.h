#include "GameObject.h"


class ColorBox : public CGameObject
{
		int height;
		int width;
	public:
		ColorBox(int width,int height);
		virtual void Render();
		virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};

