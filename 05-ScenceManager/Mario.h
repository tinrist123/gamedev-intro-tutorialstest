#pragma once
#include "GameObject.h"
#include <assert.h>
#include <algorithm>
#include "Constant.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Ground.h"
#include "Item.h"
#include "Portal.h"
#include "ColorBox.h"
#include "Game.h"
#include "Utils.h"
#include "Pipe.h"
#include "Koopas.h"
#include "FireBullet.h"
#include "Flower.h"
#include "MarioTail.h"
#include "Coin.h"
#include "MarioBullet.h"
#include "Timer.h"
#include "PortalStop.h"

//#include "Scence.h"


class CMario : public CGameObject
{
public:
	static CMario* __instance;
	bool isSwitchedScreen = false;
	bool isEndMap = false;
	
	bool isForcusTurnOffIntro = false;

	// Asset of Mario
	vector<int> listItemCollected;
	Timer* timeToShowEndGameText;

	bool isIntroScence = true;

	/// <summary>
	///  World Selection Variables
	bool isInWorldSelectionMap = true;
	int lastPortalStopIndex = -1;
	bool isMovedToLeft = true;
	bool isMovedToTop = true;
	bool isMovedToRight = true;
	bool isMovedToBottom = true;
	bool isCanGetIntoWorldMap = false;
	/// </summary>
	
	int level = 1;
	int setTimeRenderingAni = 100;
	int untouchable;
	int coinCollected = 0;
	int scores = 1000;


	int scenceId = 0;

	void StoreScenceId(int id) { scenceId = id; }

	Constant* constant;
	DWORD timeFlyingForTail = 0;
	MarioTail* Tail = new MarioTail();
	MarioBullet* bullet = new MarioBullet();
	PortalStop* portalStop = new PortalStop();


	float BBox_TopOfMario;  //top cua bbox
	float BBox_LeftOfMario;
	float BBox_BottomOfMario;
	float BBox_RightOfMario;
	bool isInSkyArea = false;

	bool isCanHoldingKoopas = false;
	bool isEnteredFirstSpaceUp = false;
	bool isKeepJumpingHigher = false;
	bool isFireShoot = false;
	bool onSitting = false;

	bool isFalling = true;
	bool blockJumping = false;
	bool isKeepJumping = false;

	bool isRollBack = false;
	bool isWalking = false;
	bool isTested = false;

	bool isPreventedSpamSpace = false;
	bool isMaxSpeed = false;

	int  lastNx;
	int  totalOfBulletsReadyForFiring = 2;

	bool isRunning = false;

	bool FlagTurnBack = false;

	bool imMovable = true;
	bool isAttackPress = false;
	bool isKickedKoopas = false;

	bool isTurboFlying = false;

	void KickingKoopas() { isKickedKoopas = true; tDraw = GetTickCount64(); }

	

	// Turn on SwitchScreen
	void SwitchScreen();
	void CompleteSwitchedScreen();


	// Press DIK_DOWN on Pipe 
	bool isDownPressed = false;
	
	// Process Mario Get into Pipe
	bool isSlidingInPipe = false;

	void MarioSlideIntoPipe() { this->isSlidingInPipe = true; }
	void MarioSlideOutPipe() { this->isSlidingInPipe = false; }
	bool CheckMarioSlideIntoPipe() { return this->isSlidingInPipe; }
	
	// Mario is In Hidden Map
	bool isInHiddenMap = false;
	bool isInPipe = false;
	// Store position of Pipe that Mario get into
	float posX_OfPipe_HaveHiddenMap;
	float posY_OfPipe_HaveHiddenMap;
	void  storePosPipe_HaveHiddenMap(float posX, float posY);


	float detectVxLevelSpeed;
	float lastVx;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	static CMario* GetInstance();

	DWORD tDraw = 0;
	bool isAttacking = false;
	bool isJumpingAttack = false;

	bool checkFlagLevelSpeedToTempLevelSpeed();
	void setFlagLevelSpeedToTempLevelSpeed();

	CMario(float x = 0.0f, float y = 0.0f);
	int DetectLevelSpeedMario();

	void ReduceVelocityWhenFly() {

		if (fabs(vx) > MARIO_MAX_WALKING_SPEED)
		{
			vx -= MARIO_WALKING_DECELERATION;
		}
		else if (fabs(vx) < -MARIO_MAX_WALKING_SPEED)
		{
			vx += MARIO_WALKING_DECELERATION;
		}
	}
	void OffPreventSpamSpace()
	{
		isEnteredFirstSpaceUp = false;
		isPreventedSpamSpace = false;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj , vector<LPGAMEOBJECT>* dynamic_item , vector<LPGAMEOBJECT>* enemies, vector<LPGAMEOBJECT>* listPortalStop = NULL);
	virtual void Render();

	void collisionWithStaticObj()
	{
		if (imMovable)
			imMovable = false;
		isRunning = false;
		//this->ReduceVelocityWhenFly();
	}

	void MarioSetOnGround() { isOnGround = true; vy = 0; }

	void setTailPos()
	{
		Tail->SetPosition(this->x + 24, this->y + 18);
		Tail->nx = nx;
	}

	void SetState(int state);
	void SetLevel(int l) { level = l;  constant->changeLevelMario(level); }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void SetTimeFlyingForTail() { timeFlyingForTail = GetTickCount64(); }
	bool CheckExpiredFlyingForTail() { return GetTickCount64() - timeFlyingForTail > 5500;	}
	void ChainKickKoopas(CKoopas* &koopas,bool isElastic);

	void MarioHitEnemy();
	void AccurateCollisionWithEnemy(LPGAMEOBJECT enemies);
	void CollisionWithFireball(vector <LPGAMEOBJECT> listFireball);

	void CollideWithCoinTransform(vector<LPGAMEOBJECT> listCoinTransform, vector<LPGAMEOBJECT> listWeakBrick);
	void CollideWithEnemy(vector<LPGAMEOBJECT> enemies);
	void playerHittingSpecialItem(LPGAMEOBJECT& item,vector<LPGAMEOBJECT>& listEffect);
	void CollideWithItem(vector<LPGAMEOBJECT> items, vector<LPGAMEOBJECT>& listEffect);

	bool checkMarioTurnBack() 
	{
		if (nx == 1 && vx < 0)
		{
			return true;
		}
		if (nx == -1 && vx > 0)
		{
			return true;
		}
		return false;
	}

	void DecelerateVelocityRunning()
	{
		if (detectVxLevelSpeed > MARIO_MAX_WALKING_SPEED)
		{
			detectVxLevelSpeed -= 0.0002;
		}
 		else if (detectVxLevelSpeed < -MARIO_MAX_WALKING_SPEED)
		{
			detectVxLevelSpeed += 0.0002;
		}
	}

	void Reset();

	void MarioIsFalling() 
	{ 
		this->isFalling = true;
		this->isOnGround = false;
	}

	void increCoinCollected() { this->coinCollected++; }
	int getCoinCollected() { return this->coinCollected; }
	void increScores() { this->scores+=100; }
	int getScores() {  return this->scores; }

	void IncreScoreMario()
	{
		this->increCoinCollected();
		this->increScores();
	}

	void GetBoundingBoxForRespectiveLevel(float& left, float& top, float& right, float& bottom, int level, int nx);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};