#include "Mushroom.h"

Mushroom::~Mushroom()
{
}

void Mushroom::Render()
{
	if (pointEff)
	{
		pointEff->Render();
	}
	else {
		if (this->kindOfMushroom == KIND_RED_MUSHROOM) ani = ITEM_ANI_MUSHROOM;
		else if (this->kindOfMushroom == KIND_GREEN_MUSHROOM) ani = ITEM_ANI_GREEN_MUSHROOM;

		animation_set->at(ani)->Render(x, y, 255);
	}
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (pointEff)
	{
		pointEff->Update(dt, coObjects);
		return;
	}

	if (y <= start_y - 16 && !onGravity)
	{
		y = start_y - 16;
		onGravity = true;
		vx = 0.08*nx;
		this->isTouchable = true;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (onGravity)
	{
		vy += MARIO_GRAVITY * dt;
	}

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
			//x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		//if (nx != 0) vx = -vx;
		if (ny != 0) {
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->getCategoryObject() == Category::GROUND)
			{
				if (e->ny < 0)
				{
					vy = 0;
					isOnGround = true;
				}
			}
			if (e->obj->getTypeObject() == Type::COLORBOX) {
				if (e->nx != 0) {
					this->x += dx;
				} 
			}
			else if (e->obj->getTypeObject() == Type::PIPE) {
				if (e->nx != 0) {
					this->vx = -this->vx ;
				}
			}
			else if (e->obj->getCategoryObject() == Category::ENEMY) {
				if (e->nx != 0) {
					this->x += dx;
				}
			}
			else if (e->obj->getCategoryObject() == Category::BRICK) {
				if (e->nx != 0) {
					this->vx = -this->vx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isBoundingBox)
	{
		return;
	}
	else {
		l = x;
		t = y;
		r = x + ITEM_BBOX_WIDHT;
		b = y + ITEM_BBOX_HEIGHT;
	}
}

Mushroom::Mushroom(float posX, float posY)
{
	this->type = 41;

	this->start_x = posX;
	this->start_y = posY;

}