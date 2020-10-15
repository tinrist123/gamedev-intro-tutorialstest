#include	 "Map.h"
#include	"GameObject.h"
#include "Utils.h"

Map::Map()
{
	CTextures::GetInstance()->Add(300, L"textures\\tilemap.png", D3DCOLOR_XRGB(255, 255, 255));
	ReadMap();
}
void Map::ReadMap()
{
	ifstream ifs("textures\\out.txt", ios::in);
	ifs >> row >> collum /*>> TileRow >> TiledCollum*/;	
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < collum; j++)
		{
			ifs >> TiledID[i][j];
		}
	}
	ifs.close();
}
void Map::DrawMap()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < collum; j++)
		{
			int Id = TiledID[i][j];
			RECT r;
			r.left = (Id - 1) % 11 * 16;
			if (Id % 11 == 0)
				r.top = (Id / 11 - 1) * 16;
			else
				r.top = Id / 11 * 16;
			r.right = r.left + 16;
			r.bottom = r.top + 16;
			CGame::GetInstance()->Draw(j * 16-1, i * 16 - 1, CTextures::GetInstance()->Get(300), r.left, r.top, r.right, r.bottom);
		}
}