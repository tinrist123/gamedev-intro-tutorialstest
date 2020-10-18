#pragma once
#include "Brick.h"

#define QUESTION_BRICK_AVAILABLE	1

class QuestionBrick :
    public CGameObject
{
public:
	bool isSpecial = false;


	int height;
	int width;

	int state;

	int start_x;
	int start_y;

	bool isTouched = false;

public:
	QuestionBrick(int start_x,int start_y,int width, int height);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);	
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

