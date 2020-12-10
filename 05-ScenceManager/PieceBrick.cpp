#include "PieceBrick.h"

PieceBrick::PieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY)
{
	x = position.x;
	y = position.y;
	vx = BROKEN_BRICK_PIECE_DEFLECT_SPEED_X * nx;
	vy = -BROKEN_BRICK_PIECE_DEFLECT_SPEED_Y * deflectFactorY;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(13);
	SetAnimationSet(ani_set);
}

void PieceBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += BROKEN_BRICK_PIECE_GRAVITY * dt;

	x += dx;
	y += dy;

	if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2)
		this->setObjDisappear();
}

void PieceBrick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void PieceBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}
