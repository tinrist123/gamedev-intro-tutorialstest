#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Utils.h"
#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	CAnimationFrame* SetTime(DWORD time) { this->time = time; return this; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
public:
	bool isAllowChangeAni;


	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;

	int changeableTime ;
	DWORD startFrameTime;
	DWORD tDraw;
	DWORD totalTimeFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) {
		this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; this->changeableTime
			= defaultTime;
	}
	void Add(int spriteId, DWORD time = 0);
	CAnimation* setTimeAnimation(int time = 100) { this->changeableTime = time; return this; }
	void Render(float x, float y, int alpha = 255);

	void SetCurrentFrame(int currentFrame = -1) { this->currentFrame = currentFrame; }
	void StartRenderAnimation() { tDraw = GetTickCount64(); };
	bool IsRenderOver(DWORD a) { 
		return GetTickCount64() - tDraw >= a;
	};
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations * GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets * GetInstance();
};