#include "Brick.h"

void CBrick::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}


void CBrick::LoadSpritesElement(CTextures* textures, LPDIRECT3DTEXTURE9 texMisc, CSprites* sprites)
{
	sprites->Add(20001, 408, 225, 424, 241, texMisc);
}