#include "BoardGame.h"



void BoardGame::DrawScores()
{
	
	int scores = mario->getScores();
	
	std::string SScores = std::to_string(scores);
	std::string SNewScores = std::string(7 - SScores.length(), '0') + SScores;

	DitgitScores = detectAndCreateListNumber(SNewScores.size());

	for (size_t i = 0; i < SNewScores.size(); i++)
	{
		ani = SNewScores[i] - '0';
		DitgitScores.at(i)->SetPosition(posX + 16*6 + 4 - 8 * i, 448);
		DitgitScores.at(i)->setAnimation(ani);
		DitgitScores.at(i)->Render();
	}
}

void BoardGame::DrawQuantityCoin(float x, float y)
{

	int coinCollected = mario->getCoinCollected();
	std::string SCoinCollected= std::to_string(coinCollected);

	DitgitQuantityCoin = detectAndCreateListNumber(SCoinCollected.size());
	
	for (size_t i = 0; i < SCoinCollected.size(); i++)
	{
		ani = SCoinCollected[i] - '0';
		DitgitQuantityCoin.at(i)->SetPosition(x + 16 * 6 + 12 - 8 * i, 438);
		DitgitQuantityCoin.at(i)->setAnimation(ani);
		DitgitQuantityCoin.at(i)->Render();
	}
}

void BoardGame::DrawNumber(float x, float y)
{
	countdown = MAX_PLAY_TIME - (int)(playTime->GetElapsedTime() / CLOCKS_PER_SEC);
	
	std::string SCountdown = std::to_string(countdown);
	listNumber = detectAndCreateListNumber(SCountdown.size());

	for (size_t i = 0; i < SCountdown.size(); i++)
	{
		ani = SCountdown[i] - '0';
		listNumber.at(i)->SetPosition(x + 16*5 + 12 + 8 * i,447);
		listNumber.at(i)->setAnimation(ani);
		listNumber.at(i)->Render();
	}
}

void BoardGame::DrawCarretRightSpeed()
{
	//carretRight->Render();
}

vector<LPGAMEOBJECT> BoardGame::detectAndCreateListNumber(int quantity)
{
	vector<LPGAMEOBJECT> list;
	for (int i = 0; i < quantity; i++)
	{
		Number* num = new Number();
		list.push_back(num);
	}

	return list;
}

void BoardGame::Update(DWORD dt, int camX, int camY)
{
	posX = camX;
	posY = camY;
	this->rangeCarret = mario->DetectLevelSpeedMario();

	if (this->rangeCarret == 7)
	{
		this->isTogglePMaxSpeed = true;
	}
	else
	{
		this->isTogglePMaxSpeed = false;
		pMaxSpeedStart = false;
	}


	for (int i = 0; i < this->rangeCarret; i++)
	{
		if (i == 6) continue;
		ListCarretRight.at(i)->Update(camX + 52 + 8*i, camY + 6);
	}
	pMaxSpeed->Update(camX + 52 + 8 * 6 + 2, camY + 6);
}

void BoardGame::Render()
{
	this->animation_set->at(0)->Render(posX, posY,255);

	for (int i = 0; i < this->rangeCarret; i++)
	{
		if (i == 6) continue;
		ListCarretRight.at(i)->Render();
	}
	if (isTogglePMaxSpeed)
	{
		if (pMaxSpeedStart)
		{
			pMaxSpeed->Render();
		}
		else
		{
			pMaxSpeedStart = true;
		}
	}

	this->DrawNumber(posX + 32, posY + 16);
	this->DrawQuantityCoin(posX + 32, posY + 16);
	this->DrawScores();
}

void BoardGame::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom;
}

BoardGame::BoardGame(CMario *player)
{
	this->mario = player;
	pMaxSpeed = new PMaxSpeed();

	for (int i =0; i < 6; i++)
	{
		CarretSpeed *carretRight = new CarretSpeed();
		ListCarretRight.push_back(carretRight);
	}

	this->playTime->Start();
}

BoardGame::~BoardGame()
{
}
