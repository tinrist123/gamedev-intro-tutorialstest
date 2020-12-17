#pragma once
#include "Brick.h"
#include "PieceBrick.h"

#define WEAK_BREAK_ANI			1	

#define WEAK_STATE_DESTROY		1	

class WeakBrick :
    public CBrick
{
public:
	vector<LPGAMEOBJECT> listPiece;

	bool vanish = false;
	int height;
	int width;

	bool isHaveP_Swtich = false;
public:
	WeakBrick();
	WeakBrick(int width, int height, int);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

