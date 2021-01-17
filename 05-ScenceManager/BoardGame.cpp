#include "BoardGame.h"
#include "BoxItem.h"
#include "RandomItem.h"


void BoardGame::DrawHealthNumber()
{
	healthNumber->ani = mario->health;

	healthNumber->Render();
}

void BoardGame::DrawAssetOfMario()
{
	for (size_t i = 0; i < listAssetOfMario.size(); i++)
	{
		listAssetOfMario[i]->Render();
	}
}

void BoardGame::DrawTripleBoxItem()
{
	tripleBoxItem->Render();
}



void BoardGame::DrawBackGround()
{
	LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(1);

	CGame::GetInstance()->Draw(posX - 32, posY - 5, darken, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 255);
}

void BoardGame::DrawScores()
{
	
	int scores = mario->getScores();
	
	std::string SScores = std::to_string(scores);
	std::string SNewScores = std::string(7 - SScores.length(), '0') + SScores;

	DitgitScores = detectAndCreateListNumber(SNewScores.size());

	for (size_t i = 0; i < SNewScores.size(); i++)
	{
		ani = SNewScores[i] - '0';
		DitgitScores.at(i)->SetPosition(posX + 16*6 + 4 - 8 * i, this->posY + 16);
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
		DitgitQuantityCoin.at(i)->SetPosition(x + 16 * 6 + 12 - 8 * i,this->posY + 8);
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
		listNumber.at(i)->SetPosition(x + 16*5 + 12 + 8 * i,this->posY + 16);
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

void BoardGame::Update(DWORD dt, float camX, float camY)
{
	posX = camX + 32.0f;
	posY = camY - 8;
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
		ListCarretRight.at(i)->Update(posX + 52 + 8*i, posY+8);
	}
	pMaxSpeed->Update(posX + 52 + 8 * 6 + 2, posY + 8);

	if (isTogglePMaxSpeed)
	{
		if (!pMaxSpeedStart)
		{
			pMaxSpeedStart = true;
		}
	}

	tripleBoxItem->SetPosition(posX + 160, posY);

	if (mario->listItemCollected.size() > 0 && this->isPushedAsset)
	{
		for (size_t i = 0; i < mario->listItemCollected.size(); i++)
		{
			RandomItem* item = new RandomItem(0, 0);
			item->SetPosition(CGame::GetInstance()->cam_x + 196 + 8 * i, posY + 6);
			item->typeItemCollected = mario->listItemCollected[i];
			item->isRandomizeState = false;
			listAssetOfMario.push_back(item);
		}
		this->isPushedAsset = false;
	}

	/*for (size_t i = 0; i < listAssetOfMario.size(); i++)
	{
		listAssetOfMario[i]->SetPosition(CGame::GetInstance()->cam_x + 196 + 8 * i, posY + 6);
	}*/
	
	healthNumber->SetPosition(posX + BBOX_BIT + BBOX_BIT/2 + BBOX_BIT / 4, posY + BBOX_BIT);
}

void BoardGame::Render()
{
	/*if (!mario->isInWorldSelectionMap)
	{
		if (mario->timeToShowEndGameText)
			if (mario->timeToShowEndGameText->IsTimeUp())
				DrawLetterEndGame();

	}*/

	this->DrawBackGround();
	this->animation_set->at(0)->Render(posX, posY,255);

	for (int i = 0; i < this->rangeCarret; i++)
	{
		if (i == 6) continue;
		ListCarretRight.at(i)->Render();
	}
	

	this->DrawNumber(posX + 32, posY + 16);
	this->DrawQuantityCoin(posX + 32, posY + 16);
	this->DrawScores();
	if (pMaxSpeedStart)
	{
		pMaxSpeed->Render();
	}

	DrawTripleBoxItem();
	DrawHealthNumber();

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

	//timeToShowItemEndGame = new Timer(5500);

}

BoardGame::~BoardGame()
{
}
