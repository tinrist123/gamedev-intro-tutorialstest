#pragma once
#include "Item.h"

#define ITEM_END_GAME_TYPE_MUSHROOM			1
#define ITEM_END_GAME_TYPE_FLOWER			2
#define ITEM_END_GAME_TYPE_STAR			3


class RandomItem: public Item
{
public:
	bool isRandomizeState = true;
	int typeItemCollected;

	void SetState(int state);
	RandomItem(float x, float y);
	RandomItem();
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RandomizeItem() { this->typeItemCollected = rand() % 3 + 1; }
};

