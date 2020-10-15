#include "Mario.h"
#include "debug.h"


void CMario::Update(DWORD dt)
{
	if (vy < -MARIO_JUMP_SPEED_Y * 2)
	{
		vy = -MARIO_JUMP_SPEED_Y;
	}
	//CGameObject::Update(dt);
	vx = gsp;
	vy = vy + MARIO_GRAVITY * dt;
	//DebugOut(L"[INFO] vx: %f\n", vx);
	x += vx * dt;
	y += vy * dt;
	
	if (y > 400.0f)
	{
		y = 400.0f;
		vy = 0.0f;
		isOnGround = true;
	}

	
	
	// simple screen edge collision!!!
	
	
	//vy += MARIO_GRAVITY * dt;

	if (vx > 0 && x > 1280) x = 290;
	if (vx < 0 && x < 0) x = 0;

}

void CMario::Render()
{
	
	if (gsp == 0)
	{
		if (nx>0) ani = MARIO_ANI_IDLE_RIGHT;
		else ani = MARIO_ANI_IDLE_LEFT;
	}
	else if (gsp > 0 )
	{
		if (onRollBack && T_draw == 0)
		{
			ani = MARIO_ANI_WALKING_STOP_RIGHT;
			T_draw = GetTickCount64();
		}
		if (onRollBack && GetTickCount64() - T_draw > 200 && T_draw!=0)
		{
			ani = MARIO_ANI_WALKING_RIGHT;
			onRollBack = false;
			T_draw = 0;
		}
		if (onRollBack == false)
		{
			ani = MARIO_ANI_WALKING_RIGHT;
		}
		
	}
	else if (gsp < 0)
	{
		/*if (onRollBack)
		{
			ani = MARIO_ANI_WALKING_STOP_LEFT;
			onRollBack = false;
		}
		else
			ani = MARIO_ANI_WALKING_LEFT;*/
		if (onRollBack && T_draw == 0)
		{
			DebugOut(L"I'm in here gsp < 0\n");
			ani = MARIO_ANI_WALKING_STOP_LEFT;
			T_draw = GetTickCount64();
		}
		if (onRollBack && GetTickCount64() - T_draw > 200 && T_draw != 0)
		{
			DebugOut(L"I'm in here gsp < 0 second if\n");
			ani = MARIO_ANI_WALKING_LEFT;
			onRollBack = false;
			T_draw = 0;
		}
		if (onRollBack == false )
		{
			ani = MARIO_ANI_WALKING_LEFT;
		}
	}


	animations[ani]->Render(x, y);
	//DebugOut(L"currentframe%d", animations.at(ani)->currentFrame);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
	{
		if (gsp < 0)
		{
			
			gsp += MARIO_WALKING_ACCELEROMETER;
			/*if (gsp >= MARIO_MAX_SPEED)
			{
				gsp = MARIO_MAX_SPEED;
			}*/

			onRollBack = true;
		}
		else if (gsp <=	 MARIO_MAX_SPEED)
		{
			gsp += MARIO_WALKING_ACCELEROMETER;
			if (gsp >= MARIO_MAX_SPEED)
			{
				gsp = MARIO_MAX_SPEED;
			}
		}
		nx = 1;
		break;
	}
	case MARIO_STATE_RUNNING:
	{
		isRunning = true;
	}

	case MARIO_STATE_WALKING_LEFT: 
	{
		if (gsp > 0)
		{
			gsp -= MARIO_WALKING_ACCELEROMETER;
			/*if (gsp >= 0)
			{
				gsp = -MARIO_MAX_SPEED;
			}*/
			onRollBack = true;
		}
		else if (gsp >= -MARIO_MAX_SPEED)
		{
			gsp -= MARIO_WALKING_ACCELEROMETER;
			if (gsp <= -MARIO_MAX_SPEED)
			{
				gsp = -MARIO_MAX_SPEED;
			}
		}

		nx = -1;
		break;
	}
	case MARIO_STATE_JUMP: 
	{
			vy = -MARIO_JUMP_SPEED_Y;
			if (isOnGround)
				isOnGround = false;
	}
	case MARIO_STATE_IDLE: 
	{
		gsp -= min(fabs(gsp),FRICTION) * nx;
		break;
	}
	}
	
}

