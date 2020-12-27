#pragma once
#include "GameObject.h"

#define BROKEN_BRICK_PIECE_GRAVITY			0.0009f
#define BROKEN_BRICK_PIECE_WIDTH			8
#define	BROKEN_BRICK_PIECE_HEIGHT			8
#define	BROKEN_BRICK_PIECE_DEFLECT_SPEED_X	0.06f
#define	BROKEN_BRICK_PIECE_DEFLECT_SPEED_Y	0.16f

class PieceBrick : public CGameObject
{
public:
	PieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY = 1);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

