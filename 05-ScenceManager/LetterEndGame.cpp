#include "LetterEndGame.h"

LettersEndGame::LettersEndGame(float x, float y, string inputText)
{
	this->x = x;
	this->y = y;
	this->STextInput = inputText;
	detectStringToAnimation();
}

void LettersEndGame::UpperCaseString()
{
	for (auto& c : STextInput) c = toupper(c);
}

void LettersEndGame::detectStringToAnimation()
{
	for (int i = 0; i < STextInput.length(); i++)
	{
		if (STextInput[i] != 32)
		{
			LetterAnimation* character = new LetterAnimation(STextInput[i] - 64 + LETTER_ANI_INDEX_START);
			character->SetPosition(x + 8 * i, y);
			listLetter.push_back(character);
		}
	}
}

void LettersEndGame::Render()
{
	for (size_t i = 0; i < listLetter.size(); i++)
	{
		listLetter.at(i)->Render();
	}
}



void LettersEndGame::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case LETTER_STATE_A: break;
		case LETTER_STATE_B: break;
		case LETTER_STATE_C: break;
		case LETTER_STATE_D: break;
		case LETTER_STATE_E: break;
		case LETTER_STATE_F: break;
		case LETTER_STATE_G: break;
		case LETTER_STATE_H: break;
		case LETTER_STATE_I: break;
		case LETTER_STATE_J: break;
		case LETTER_STATE_K: break;
		case LETTER_STATE_L: break;
		case LETTER_STATE_M: break;
		case LETTER_STATE_N: break;
		case LETTER_STATE_O: break;
		case LETTER_STATE_P: break;
		case LETTER_STATE_Q: break;
		case LETTER_STATE_R: break;
		case LETTER_STATE_S: break;
		case LETTER_STATE_T: break;
		case LETTER_STATE_U: break;
		case LETTER_STATE_V: break;
		case LETTER_STATE_W: break;
		case LETTER_STATE_X: break;
		case LETTER_STATE_Y: break;
		case LETTER_STATE_Z: break;
	default:
		break;
	}

}

void LettersEndGame::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}

LetterAnimation::LetterAnimation(int aniTextIndex)
{
	this->aniTextIndex = aniTextIndex;
}

void LetterAnimation::Render()
{
	animation_set->at(aniTextIndex)->Render(x, y);
}

void LetterAnimation::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
