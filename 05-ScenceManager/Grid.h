#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Utils.h"
#define CELL_WIDTH 250
#define CELL_HEIGHT 250

//#define CELL_WIDTH 10
//#define CELL_HEIGHT 10

#define MAP_WIDTH 3328
#define MAP_HEIGHT  1000



class Grid
{
	int map_width, map_height;
	int cell_width, cell_height;
	vector<vector<vector<LPGAMEOBJECT>>> allcells;
public:
	Grid(int cell_width = CELL_WIDTH, int cell_height = CELL_HEIGHT);
	~Grid();
	void Resize(int map_width, int map_height);
	void ClearGrid(int nums_row, int nums_col);
	void ResetGrid(vector<LPGAMEOBJECT> list);
	void PushGrid(vector<LPGAMEOBJECT> list);
	void GetGrid(vector<LPGAMEOBJECT>& list);
	void PushGridStart(LPGAMEOBJECT obj, int row, int col);
	void CheckCamGrid(vector<LPGAMEOBJECT>& list);
};
