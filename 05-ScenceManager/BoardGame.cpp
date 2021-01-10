#include "BoardGame.h"
#include "BoxItem.h"

void BoardGame::loadLetterEndGame()
{
	LetterEndGame* letterC = new LetterEndGame();
	letterC->SetPosition(2624, 280);
	letterC->SetState(LETTER_STATE_C);
	LetterEndGame* letterO = new LetterEndGame();
	letterO->SetPosition(2640 - 8, 280);
	letterO->SetState(LETTER_STATE_O);
	LetterEndGame* letterU = new LetterEndGame();
	letterU->SetPosition(2656 - 16, 280);
	letterU->SetState(LETTER_STATE_U);
	LetterEndGame* letterR = new LetterEndGame();
	letterR->SetPosition(2672 - 24, 280);
	letterR->SetState(LETTER_STATE_R);

	LetterEndGame* letterS = new LetterEndGame();
	letterS->SetPosition(2688 - 32, 280);
	letterS->SetState(LETTER_STATE_S);

	LetterEndGame* letterE = new LetterEndGame();
	letterE->SetPosition(2704 - 40, 280);
	letterE->SetState(LETTER_STATE_E);

	LetterEndGame* letterC1 = new LetterEndGame();
	letterC1->SetPosition(2736 - 54, 280);
	letterC1->SetState(LETTER_STATE_C);

	LetterEndGame* letterL = new LetterEndGame();
	letterL->SetPosition(2752 - 54 - 8, 280);
	letterL->SetState(LETTER_STATE_L);


	LetterEndGame* letterE1 = new LetterEndGame();
	letterE1->SetPosition(2768 - 54 - 16, 280);
	letterE1->SetState(LETTER_STATE_E);

	LetterEndGame* letterA = new LetterEndGame();
	letterA->SetPosition(2784 - 54 - 24, 280);
	letterA->SetState(LETTER_STATE_A);

	LetterEndGame* letterR1 = new LetterEndGame();
	letterR1->SetPosition(2800 - 54 - 32, 280);
	letterR1->SetState(LETTER_STATE_R);


	LetterEndGame* letterY = new LetterEndGame();
	letterY->SetPosition(2592 + 16, 300);
	letterY->SetState(LETTER_STATE_Y);

	LetterEndGame* letterO1 = new LetterEndGame();
	letterO1->SetPosition(2608 + 8, 300);
	letterO1->SetState(LETTER_STATE_O);

	LetterEndGame* letterU1 = new LetterEndGame();
	letterU1->SetPosition(2624, 300);
	letterU1->SetState(LETTER_STATE_U);

	LetterEndGame* letterG = new LetterEndGame();
	letterG->SetPosition(2672 - 32, 300);
	letterG->SetState(LETTER_STATE_G);

	LetterEndGame* letterO2 = new LetterEndGame();
	letterO2->SetPosition(2688 - 32 - 8, 300);
	letterO2->SetState(LETTER_STATE_O);

	LetterEndGame* letterT = new LetterEndGame();
	letterT->SetPosition(2704 - 32 - 16, 300);
	letterT->SetState(LETTER_STATE_T);

	LetterEndGame* letterA1 = new LetterEndGame();
	letterA1->SetPosition(2736 - 64, 300);
	letterA1->SetState(LETTER_STATE_A);

	LetterEndGame* letterC2 = new LetterEndGame();
	letterC2->SetPosition(2688, 300);
	letterC2->SetState(LETTER_STATE_C);

	LetterEndGame* letterA2 = new LetterEndGame();
	letterA2->SetPosition(2688 + 8, 300);
	letterA2->SetState(LETTER_STATE_A);

	LetterEndGame* letterR2 = new LetterEndGame();
	letterR2->SetPosition(2688 + 16, 300);
	letterR2->SetState(LETTER_STATE_R);

	LetterEndGame* letterD = new LetterEndGame();
	letterD->SetPosition(2688 + 24, 300);
	letterD->SetState(LETTER_STATE_D);



	listLetter_FirstRow.push_back(letterC);
	listLetter_FirstRow.push_back(letterO);
	listLetter_FirstRow.push_back(letterU);
	listLetter_FirstRow.push_back(letterR);
	listLetter_FirstRow.push_back(letterS);
	listLetter_FirstRow.push_back(letterE);

	listLetter_FirstRow.push_back(letterC1);
	listLetter_FirstRow.push_back(letterL);
	listLetter_FirstRow.push_back(letterE1);
	listLetter_FirstRow.push_back(letterA);
	listLetter_FirstRow.push_back(letterR1);


	BoxItem* box = new BoxItem();
	box->SetPosition(2688 + 48, 292);
	listLetter_SecondRow.push_back(box);

	listLetter_SecondRow.push_back(letterY);
	listLetter_SecondRow.push_back(letterO1);
	listLetter_SecondRow.push_back(letterU1);

	listLetter_SecondRow.push_back(letterG);
	listLetter_SecondRow.push_back(letterO2);
	listLetter_SecondRow.push_back(letterT);

	listLetter_SecondRow.push_back(letterA1);

	listLetter_SecondRow.push_back(letterC2);
	listLetter_SecondRow.push_back(letterA2);
	listLetter_SecondRow.push_back(letterR2);
	listLetter_SecondRow.push_back(letterD);

}

void BoardGame::DrawCollectedItemEndGame()
{
	randomizeItem->Render();
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

void BoardGame::DrawLetterEndGame()
{
	for (size_t i = 0; i < listLetter_FirstRow.size(); i++)
	{
		listLetter_FirstRow[i]->Render();
	}

	if (timeToShowItemEndGame && timeToShowItemEndGame->IsTimeUp())
	{
		for (size_t i = 0; i < listLetter_SecondRow.size(); i++)
		{
			listLetter_SecondRow[i]->Render();
		}
		this->DrawCollectedItemEndGame();
	}
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
			item->SetPosition(posX + 164 + 8 * i, posY + 6);
			item->typeItemCollected = mario->listItemCollected[i];
			item->isRandomizeState = false;
			listAssetOfMario.push_back(item);
		}
		this->isPushedAsset = false;

		randomizeItem = new RandomItem(0, 0);
		randomizeItem->typeItemCollected = mario->listItemCollected[mario->listItemCollected.size() - 1];
		randomizeItem->isRandomizeState = false;
		randomizeItem->SetPosition(2688 + 52, 298);
	}

	if (!timeToShowItemEndGame->isStarted)
	{
		timeToShowItemEndGame->Start();
	}
}

void BoardGame::Render()
{
	if (!mario->isInWorldSelectionMap)
	{
		if (mario->timeToShowEndGameText)
			if (mario->timeToShowEndGameText->IsTimeUp())
				DrawLetterEndGame();

	}

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
	DrawAssetOfMario();

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

	loadLetterEndGame();
	timeToShowItemEndGame = new Timer(5500);

}

BoardGame::~BoardGame()
{
}
