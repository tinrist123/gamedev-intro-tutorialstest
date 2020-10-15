
#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16
#include "GameObject.h"

class Pipe : public CGameObject
{
	int height;
	int width;
public:

	Pipe(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


