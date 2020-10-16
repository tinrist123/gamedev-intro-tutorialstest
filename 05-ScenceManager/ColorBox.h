#include "Ground.h"


class ColorBox : public Ground
{
		int height;
		int width;
	public:
		ColorBox(int width,int height);
		virtual void Render();
		virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};

