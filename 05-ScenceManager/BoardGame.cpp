#include "BoardGame.h"

void BoardGame::DrawCarretRightSpeed()
{
	//carretRight->Render();
}

void BoardGame::Update(DWORD dt, int camX, int camY, CMario* mario)
{
	posX = camX;
	posY = camY;
	this->rangeCarret = mario->DetectLevelSpeedMario();

	if (this->rangeCarret == 6)
	{
		this->isTogglePMaxSpeed = true;
	}
	else
	{
		this->isTogglePMaxSpeed = false;
	}

	for (int i = 0; i < this->rangeCarret; i++)
	{
		ListCarretRight.at(i)->Update(camX + 52 + 8*i, camY + 6);

	}

	pMaxSpeed->Update(camX + 52 + 8 * 6 + 2, camY + 6);
}

void BoardGame::Render()
{
	this->animation_set->at(0)->Render(posX, posY,255);

	for (int i = 0; i < this->rangeCarret; i++)
	{
		ListCarretRight.at(i)->Render();
	}
	if (isTogglePMaxSpeed)
	{
		pMaxSpeed->Render();
	}
}

void BoardGame::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom;
}

BoardGame::BoardGame()
{
	pMaxSpeed = new PMaxSpeed();

	for (int i =0; i < 6; i++)
	{
		CarretSpeed *carretRight = new CarretSpeed();
		ListCarretRight.push_back(carretRight);
	}
}
