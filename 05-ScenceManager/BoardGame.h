#pragma once
#include "Sprites.h"
#include "GameObject.h"
#include <string>
#include "CarretSpeed.h"
#include "Number.h"
#include "BoardAddSprite.h"
#include "PMaxSpeed.h"
#include "Mario.h"
#include "Timer.h"
#include "TripleBoxItem.h"

#define MAX_PLAY_TIME			300

class BoardGame : public BoardAddSprite
{
	vector<LPCARRET>  ListCarretRight;
	vector<LPGAMEOBJECT>  listNumber;
	vector<LPGAMEOBJECT>  DitgitQuantityCoin;
	vector<LPGAMEOBJECT>  DitgitScores;
	vector<LPGAMEOBJECT>  listAssetOfMario;

	Number* healthNumber = new Number();
	PMaxSpeed* pMaxSpeed;
	TripleBoxItem* tripleBoxItem = new TripleBoxItem();

	CMario* mario;
	Timer* playTime = new Timer(MAX_PLAY_TIME * CLOCKS_PER_SEC);
	int countdown = 0;

	int typeItemMarioColleted = 0;

	bool pMaxSpeedStart = false;
	bool isPushedAsset = true;

	bool isTogglePMaxSpeed = false;
	int rangeCarret = 0;
	float posX;
	float posY;
	int score;
	int heart;
	int life;
	int time;

	void DrawHealthNumber();
	void DrawAssetOfMario();
	void DrawTripleBoxItem();
	void DrawBackGround();
	void DrawScores();
	void DrawQuantityCoin(float x, float y);
	void DrawNumber(float x, float y);
	void DrawCarretRightSpeed();

	vector<LPGAMEOBJECT> detectAndCreateListNumber(int);

public:
	void Update(DWORD dt, float camX, float camY);
	void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	BoardGame(CMario* player);
	~BoardGame();
};

