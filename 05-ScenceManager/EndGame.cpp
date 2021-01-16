#include "EndGame.h"

EndGame::EndGame() :  BoardAddSprite()
{
	timeToShowItemEndGame = new Timer(5500);
}

void EndGame::Render()
{
}

void EndGame::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
