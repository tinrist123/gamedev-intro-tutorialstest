#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Utils.h"
#define CELL_WIDTH	195
#define CELL_HEIGHT 328


#define MAP_WIDTH 3328
#define MAP_HEIGHT  1000



class Grid
{	
	int map_width, map_height;
	int cell_width, cell_height;
	vector<vector<vector<LPGAMEOBJECT>>> allcells;
	vector<vector<vector<LPGAMEOBJECT>>> testCells;
	unordered_map<int, vector<unordered_map<int, int>>> objects_cell;

public:
	Grid(int cell_width = CELL_WIDTH, int cell_height = CELL_HEIGHT);
	~Grid();

	void Firstly_PushObjects(unordered_map<int, LPGAMEOBJECT>);

	
	void storeGrid_Object_FromFile(unordered_map<int, vector<unordered_map<int, int>>> objects_cell);
	void Resize(int map_width, int map_height);
	void ClearGrid(int nums_row, int nums_col);
	void PushGrid(vector<LPGAMEOBJECT> list);
	void ResetGrid(vector<LPGAMEOBJECT> listStaticObj, vector<LPGAMEOBJECT> listDynamicObj);
	void PushGridStart(LPGAMEOBJECT obj, int row, int col);
	void ResetCamGrid(vector<LPGAMEOBJECT>& list);

	void GetObjInGrid(vector<LPGAMEOBJECT>& listStaticObj, vector<LPGAMEOBJECT>& listDynamicObj);

};
