#include "WeakBrick.h"



WeakBrick::WeakBrick(int width, int height, int isHaveItem = 0) : CBrick::CBrick(width, height)
{
	this->width = width;
	this->height = height;

	this->isHaveP_Swtich = (isHaveItem == 1);
	this->type = Type::WEAKBRICK;
}

void WeakBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WEAK_STATE_DESTROY:
	{
		PieceBrick* topLeftPiece = new PieceBrick({ x - 1, y - 2 }, -1, 2);
		PieceBrick* topRightPiece = new PieceBrick({ x + BBOX_BIT - BROKEN_BRICK_PIECE_WIDTH + 1, y - 2 }, 1, 2);
		PieceBrick* bottomLeftPiece = new PieceBrick({ x - 1, y + BBOX_BIT - BROKEN_BRICK_PIECE_HEIGHT }, -1);
		PieceBrick* bottomRightPiece = new PieceBrick({ x + BBOX_BIT - BROKEN_BRICK_PIECE_WIDTH + 1, y + BBOX_BIT - BROKEN_BRICK_PIECE_HEIGHT }, 1);
		listPiece = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
		this->vanish = true;


		break;
	}
	default:
		break;
	}
}

void WeakBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int num_vanishPiece = 0;

 	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Update(dt, coObjects);
		if (piece->objectDisappear())
			num_vanishPiece++;
	}

	if (num_vanishPiece == 4)
		this->setObjDisappear();

	
}

void WeakBrick::Render()
{
	for (LPGAMEOBJECT piece : listPiece)
		piece->Render();

	if (!vanish)
		animation_set->at(WEAK_BREAK_ANI)->Render(x, y);

	//RenderBoundingBox();

}

void WeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (vanish) l = t = r = b;
	
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH * width;
		b = y + BRICK_BBOX_HEIGHT * height;
	}

	
}