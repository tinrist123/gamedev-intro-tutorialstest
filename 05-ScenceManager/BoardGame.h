#pragma once
#pragma once
#include "Sprites.h"
#include "GameObject.h"
#include <string>
#include "CarretSpeed.h"
#include "Letter.h"
#include "Number.h"
#include "BoardAddSprite.h"
#include "PMaxSpeed.h"
#include "Mario.h"
#include "Timer.h"

#define MAX_PLAY_TIME			300

class BoardGame : public BoardAddSprite
{
	vector<LPCARRET>  ListCarretRight;
	vector<LPGAMEOBJECT>  listNumber;
	vector<LPGAMEOBJECT>  DitgitQuantityCoin;
	vector<LPGAMEOBJECT>  DitgitScores;
	PMaxSpeed* pMaxSpeed;

	CMario* mario;
	Timer* playTime = new Timer(MAX_PLAY_TIME * CLOCKS_PER_SEC);
	int countdown = 0;

	bool pMaxSpeedStart = false;

	bool isTogglePMaxSpeed = false;
	int rangeCarret = 0;
	int posX;
	int posY;
	int score;
	int heart;
	int life;
	int time;
	void DrawBoard();
	void DrawScores();
	void DrawQuantityCoin(float x, float y);
	void DrawNumber(float x, float y);
	void DrawCarretRightSpeed();

	vector<LPGAMEOBJECT> detectAndCreateListNumber(int);

public:
	void Update(DWORD dt, int camX, int camY);
	void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	BoardGame(CMario* player);
	~BoardGame();
};

