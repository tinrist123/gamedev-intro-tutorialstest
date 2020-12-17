#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "Animations.h"
#include "define.h"
#include <d3dx9.h>
#include <algorithm>
#include "Textures.h"
#include "Game.h"
#include "Sprites.h"
#include "Utils.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define MARIO_GRAVITY									0.0006f
#define BBOX_BIT										16.0f

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \

#endif


class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	bool isDisappeared = false;
	bool isCreated = false;
	bool isOnGround = true;
	bool isBoundingBox = true;
	bool isAddedEffect = false;
	bool isDamageable = true;
	bool isInCamera = false;
	bool isTouchable = false;

	int type;
	int category = -1;

	int ani = -1;

	float x = 0; 
	float y = 0;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;
	int ny;

	int state = -1;

	int health = 1;

	DWORD dt; 

	LPANIMATION_SET animation_set;
public: 
	int getTypeObject() { return type; }
	int getCategoryObject() { return category; }

	float GetX() { return this->x; }

	void setAnimation(int ani) { this->ani = ani; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void setHealth(int health) { this->health = health; }
	void subHealth() { if (this->health > 0 )this->health--; }
	int GetHealth() { return health; }

	int GetState() { return this->state; }
	bool isAliveObject() { return health >= 1; }

	bool objectDisappear()
	{
		return health == 0 && isDisappeared && !isBoundingBox;
	}

	void setObjDisappear()
	{
		health = 0;
		isDisappeared = true;
		isBoundingBox = false;
	}

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx,		
		float &rdy);

public:
	bool AABBCollision(LPGAMEOBJECT b);

	CGameObject();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	~CGameObject();

	bool isGround(float x, float y, vector<LPGAMEOBJECT> arr)
	{
		int count = 0;
		for (int i = 0; i < arr.size(); i++)
		{
			float l, t, r, b;
			arr[i]->GetBoundingBox(l, t, r, b);
			/*if (y - t >= 5.0f)
				continue;*/
			if (arr[i]->getCategoryObject() == Category::GROUND
				|| arr[i]->getCategoryObject() == Category::BRICK)
			{
				if (x >= l && x <= r && y >= t && y <= b)
					return true;
			}
			else {
				count++;
			}
		}
		return false;
	}
};

