#include "Koopas.h"
#include "Utils.h"
#include "Ground.h"
#include "ColorBox.h"
#include "Pipe.h"

CKoopas::CKoopas(int typeKoopas)
{
	this->TypeKoopas = typeKoopas;
	nx = 1;

	switch (this->TypeKoopas)
	{
	case KOOPAS_TYPE_HAVE_WING:
		SetState(KOOPAS_STATE_HAVE_WING_FLYING);
		break;
	case KOOPAS_TYPE_NORMAL:
		SetState(KOOPAS_STATE_WALKING);
		break;
	case KOOPAS_TYPE_HAVE_WING_WALKING:
		SetState(KOOPAS_STATE_HAVE_WING_WALKING);
		break;
	/*case KOOPAS_TYPE_HAVE_WING_FLYING:
		SetState(KOOPAS_STATE_WALKING);
		break;*/
	default:
		break;
	}
	health++;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		/*if (rdx != 0 && rdx!=dx)
			x += nx*abs(rdx); */

			// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny < 0)
				{
					if (state == KOOPAS_STATE_HAVE_WING_FLYING)
					{	
						vy = -0.2f;
					}
					else {
						vy = 0;
					}
				}
				else if (e->nx != 0)
				{
					vx = -vx;
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += vx * dt;
				}
				if (e->ny < 0)
				{
					vy = 0;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj))
			{
				if (e->nx != 0)
				{
					vx = -vx;
					nx = -nx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = -KOOPAS_ANI_SHELL_MOVING_SPEED_X;
		ani = KOOPAS_ANI_DIE;
		break;
	case KOOPAS_STATE_SHELL:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		ani = KOOPAS_ANI_SHELL;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		ani = KOOPAS_ANI_SHELL_MOVING;
		break;
	case KOOPAS_STATE_HAVE_WING_WALKING:
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		if (nx == -1)
			ani = KOOPAS_ANI_HAVE_WING_WALKING_LEFT;
		else if (nx == 1)
			ani = KOOPAS_ANI_HAVE_WING_WALKING_RIGHT;
		break;
	case KOOPAS_STATE_HAVE_WING_FLYING:
		vx = KOOPAS_ANI_WALKING_RIGHT_SPEED_X *nx;
		vy = 0;
		if (nx == -1)
			ani = KOOPAS_ANI_HAVE_WING_FLYING_LEFT;
		else if (nx == 1)
			ani = KOOPAS_ANI_HAVE_WING_FLYING_RIGHT;
		break;
	default:
		break;
	}

}