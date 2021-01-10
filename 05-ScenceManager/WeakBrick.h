#pragma once
#include "Brick.h"
#include "PieceBrick.h"

#define QUESTION_BRICK_SPEED_Y		0.07f	

#define WEAK_BREAK_ANI			1	
#define ANI_BRICK_EMPTY			3	

#define WEAK_STATE_DESTROY		1	
#define WEAK_STATE_EMPTY_ANI	2
#define WEAK_STATE_JUMP			3


#define WEAK_BRICK_HAVE_EMPTY_ITEM			0
#define WEAK_BRICK_HAVE_P_SWITCH			1
#define WEAK_BRICK_HAVE_COIN				2
#define WEAK_BRICK_HAVE_LEVEL_MUSHROOM		3
#define WEAK_BRICK_HAVE_SPECIAL_ITEM		4





class WeakBrick :
    public CBrick
{
public:
	vector<LPGAMEOBJECT> listPiece;

	bool vanish = false;
	bool isTranformed = false;
	int height;
	int width;
	bool isHaveP_Switch = false;
	int kindOfItemInsinde = 0;
public:
	WeakBrick();
	WeakBrick(int width, int height, int = 0);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void activeTranform() { this->isTranformed = true; }
	void InactiveTranform() { this->isTranformed = false; }
};

