#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Utils.h"

class Grid
{	
	int map_width, map_height;
	int cell_width, cell_height;
	vector<vector<vector<LPGAMEOBJECT>>> allcells;
	vector<vector<vector<LPGAMEOBJECT>>> testCells;
	unordered_map<int, vector<unordered_map<int, int>>> objects_cell;

public:
	Grid(int cell_width = 195, int cell_height = 328);
	void storeCell_Size(int x, int y);
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
