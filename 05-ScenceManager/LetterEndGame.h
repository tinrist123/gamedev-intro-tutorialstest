#pragma once
#include "BoardAddSprite.h"

#define LETTER_ANI_A	10
#define LETTER_ANI_B	11
#define LETTER_ANI_C	12
#define LETTER_ANI_D	13
#define LETTER_ANI_E	14
#define LETTER_ANI_F	15
#define LETTER_ANI_G	16
#define LETTER_ANI_H	17
#define LETTER_ANI_I	18
#define LETTER_ANI_J	19
#define LETTER_ANI_K	20
#define LETTER_ANI_L	21
#define LETTER_ANI_M	22
#define LETTER_ANI_N	23
#define LETTER_ANI_O	24
#define LETTER_ANI_P	25
#define LETTER_ANI_Q	26
#define LETTER_ANI_R	27
#define LETTER_ANI_S	28
#define LETTER_ANI_T	29
#define LETTER_ANI_U	30
#define LETTER_ANI_V	31
#define LETTER_ANI_W	32
#define LETTER_ANI_X	33
#define LETTER_ANI_Y	34
#define LETTER_ANI_Z	35

#define LETTER_STATE_A	10
#define LETTER_STATE_B	11
#define LETTER_STATE_C	12
#define LETTER_STATE_D	13
#define LETTER_STATE_E	14
#define LETTER_STATE_F	15
#define LETTER_STATE_G	16
#define LETTER_STATE_H	17
#define LETTER_STATE_I	18
#define LETTER_STATE_J	19
#define LETTER_STATE_K	20
#define LETTER_STATE_L	21
#define LETTER_STATE_M	22
#define LETTER_STATE_N	23
#define LETTER_STATE_O	24
#define LETTER_STATE_P	25
#define LETTER_STATE_Q	26
#define LETTER_STATE_R	27
#define LETTER_STATE_S	28
#define LETTER_STATE_T	29
#define LETTER_STATE_U	30
#define LETTER_STATE_V	31
#define LETTER_STATE_W	32
#define LETTER_STATE_X	33
#define LETTER_STATE_Y	34
#define LETTER_STATE_Z	35

class LetterEndGame : public BoardAddSprite
{
public:
	LetterEndGame();
	virtual void Render();
	void Update(float camX, float camY);
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

