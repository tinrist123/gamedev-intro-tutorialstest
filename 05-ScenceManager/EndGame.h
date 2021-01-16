#pragma once
#include "BoardAddSprite.h"
#include "Timer.h"
#include "RandomItem.h"
#include "BoxItem.h"

class EndGame: public BoardAddSprite
{
public:
	vector<LPGAMEOBJECT>  listLetter_FirstRow;
	vector<LPGAMEOBJECT>  listLetter_SecondRow;

public:
	EndGame();

	RandomItem* randomizeItem;
	Timer* timeToShowItemEndGame;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

