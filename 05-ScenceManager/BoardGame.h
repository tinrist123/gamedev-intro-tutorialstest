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

#define LIFE_TIME 300
#define HP_SIMON_AND_BOSS	16
#define A_MINUTE	60
#define BOARD 351
#define ITEM_KNIFE_SPRITE	202
#define ITEM_BOOMERANG_SPRITE	203
#define ITEM_AXE_SPRITE		204
#define ITEM_HOLY_SPRITE	205
#define ITEM_CLOCK_SPRITE	209

class BoardGame : public BoardAddSprite
{
	vector<LPCARRET>  ListCarretRight;
	PMaxSpeed* pMaxSpeed;

	bool isTogglePMaxSpeed = false;
	int rangeCarret = 0;
	int idSubWeapon;
	int healthSimon, healthSimonLost, healthBoss, healthBossLost;
	int posX;
	int posY;
	int score;
	int heart;
	int life;
	int time;
	void DrawBoard();
	void DrawNumber(int x, int y, string a);
	void DrawCarretRightSpeed();
public:
	void Update(DWORD dt, int camX, int camY, CMario* mario);
	void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	BoardGame();
	~BoardGame();
};

