#include "WeakBrick.h"



WeakBrick::WeakBrick() : CBrick()
{
}

WeakBrick::WeakBrick(int width, int height, int kindOfItemInside) : CBrick::CBrick(width, height)
{
	this->width = width;
	this->height = height;
	this->kindOfItemInsinde = kindOfItemInside;
	this->type = Type::WEAKBRICK;
	this->isHaveP_Switch = kindOfItemInside == WEAK_BRICK_HAVE_P_SWITCH;

	if (this->kindOfItemInsinde == WEAK_BRICK_HAVE_COIN)
	{
		this->setHealth(10);
	}

}

void WeakBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case WEAK_STATE_EMPTY_ANI:
		{
			vy = -QUESTION_BRICK_SPEED_Y;
			subHealth();
			break;
		}
		case WEAK_STATE_DESTROY:
		{
			if (!isHaveP_Switch)
			{
				PieceBrick* topLeftPiece = new PieceBrick({ x - 1, y - 2 }, -1, 2);
				PieceBrick* topRightPiece = new PieceBrick({ x + BBOX_BIT - BROKEN_BRICK_PIECE_WIDTH + 1, y - 2 }, 1, 2);
				PieceBrick* bottomLeftPiece = new PieceBrick({ x - 1, y + BBOX_BIT - BROKEN_BRICK_PIECE_HEIGHT }, -1);
				PieceBrick* bottomRightPiece = new PieceBrick({ x + BBOX_BIT - BROKEN_BRICK_PIECE_WIDTH + 1, y + BBOX_BIT - BROKEN_BRICK_PIECE_HEIGHT }, 1);
				listPiece = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
				this->vanish = true;
				this->isAddedEffect = true;
			}
			break;
		}
		case WEAK_STATE_JUMP:
			vy = -QUESTION_BRICK_SPEED_Y;
			if (!this->kindOfItemInsinde == WEAK_BRICK_HAVE_EMPTY_ITEM)
				subHealth();
			break;
		default:
			break;
	}
}

void WeakBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	if (this->GetHealth() == 0)
	{
		this->state = (WEAK_STATE_EMPTY_ANI);
	}

	x += dx;
	y += dy;

	if (y >= start_y)
	{
		if (y == start_y) return;
		y = start_y;
		vy = 0;
	}
	else if (y < start_y)
	{
		vy += MARIO_GRAVITY * dt;
	}



	int num_vanishPiece = 0;
 	for (LPGAMEOBJECT piece : listPiece)
	{
		if (piece->objectDisappear())
			num_vanishPiece++;
	}

	if (num_vanishPiece == 4)
		this->setObjDisappear();
	
}

void WeakBrick::Render()
{
	if (state == WEAK_STATE_EMPTY_ANI)
	{
		animation_set->at(ANI_BRICK_EMPTY)->Render(x, y);
	}
	else 
	if (!vanish)
		animation_set->at(WEAK_BREAK_ANI)->Render(x, y);

	//RenderBoundingBox();

}

void WeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isTranformed || !isBoundingBox) l = t = r = b;
	
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH * width;
		b = y + BRICK_BBOX_HEIGHT * height;
	}

	
}