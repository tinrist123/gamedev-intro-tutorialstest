#include "LetterEndGame.h"

LetterEndGame::LetterEndGame()
{
}

void LetterEndGame::Render()
{
	switch (state)
	{
	case LETTER_STATE_A: ani = LETTER_ANI_A; break;
	case LETTER_STATE_B: ani = LETTER_ANI_B; break;
	case LETTER_STATE_C: ani = LETTER_ANI_C; break;
	case LETTER_STATE_D: ani = LETTER_ANI_D; break;
	case LETTER_STATE_E: ani = LETTER_ANI_E; break;
	case LETTER_STATE_F: ani = LETTER_ANI_F; break;
	case LETTER_STATE_G: ani = LETTER_ANI_G; break;
	case LETTER_STATE_H: ani = LETTER_ANI_H; break;
	case LETTER_STATE_I: ani = LETTER_ANI_I; break;
	case LETTER_STATE_J: ani = LETTER_ANI_J; break;
	case LETTER_STATE_K: ani = LETTER_ANI_K; break;
	case LETTER_STATE_L: ani = LETTER_ANI_L; break;
	case LETTER_STATE_M: ani = LETTER_ANI_M; break;
	case LETTER_STATE_N: ani = LETTER_ANI_N; break;
	case LETTER_STATE_O: ani = LETTER_ANI_O; break;
	case LETTER_STATE_P: ani = LETTER_ANI_P; break;
	case LETTER_STATE_Q: ani = LETTER_ANI_Q; break;
	case LETTER_STATE_R: ani = LETTER_ANI_R; break;
	case LETTER_STATE_S: ani = LETTER_ANI_S; break;
	case LETTER_STATE_T: ani = LETTER_ANI_T; break;
	case LETTER_STATE_U: ani = LETTER_ANI_U; break;
	case LETTER_STATE_V: ani = LETTER_ANI_V; break;
	case LETTER_STATE_W: ani = LETTER_ANI_W; break;
	case LETTER_STATE_X: ani = LETTER_ANI_X; break;
	case LETTER_STATE_Y: ani = LETTER_ANI_Y; break;
	case LETTER_STATE_Z: ani = LETTER_ANI_Z; break;
	default:
		break;
	}
	animation_set->at(ani + 1)->Render(x, y);
}

void LetterEndGame::Update(float camX, float camY)
{
}

void LetterEndGame::SetState(int state)
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

void LetterEndGame::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}
