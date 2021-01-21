#include "Grid.h"

Grid::Grid(int cell_width, int cell_height)
{
	this->cell_width = cell_width;
	this->cell_height = cell_height;
	this->objects_cell = objects_cell;
}

void Grid::storeGrid_Object_FromFile(unordered_map<int, vector<unordered_map<int, int>>> objects_cell)
{
	this->objects_cell = objects_cell;
}

void Grid::Resize(int map_width, int map_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	int totalCol = this->map_width / this->cell_width;
	int totalRow = this->map_height / this->cell_height;

	allcells.resize(totalRow + 1);
	for (int i = 0; i < totalRow + 1; i++)
		allcells[i].resize(totalCol + 1);

	ClearGrid(totalRow, totalCol);
}

void Grid::ClearGrid(int nums_row, int nums_col)
{
	for (int i = 0; i < nums_row + 1; i++)
	{
		for (int j = 0; j < nums_col + 1; j++)
		{
			for (size_t k = 0; k < allcells[i][j].size(); k++)
			{
				if (allcells[i][j][k]->getCategoryObject() == Category::ENEMY)
				{
					allcells[i][j].erase(allcells[i][j].begin() + k);
				}
			}
		}
	}
		
}

void Grid::PushGrid(vector<LPGAMEOBJECT> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		float l, t, r, b;
		list[i]->GetBoundingBox(l, t, r, b);
		int top = int(t / cell_height);
		int bottom = ceil(b / cell_height);
		int left = int(l / cell_width);
		int right = ceil(r / cell_width);

		if (!list[i]->objectDisappear())
			for (int row = top; row < bottom; row++)
				for (int col = left; col < right; col++)
					allcells[row][col].push_back(list[i]);
	}
}

void Grid::ResetGrid(vector<LPGAMEOBJECT> listStaticObj, vector<LPGAMEOBJECT> listDynamicObj)
{
	ClearGrid((int)this->map_height / cell_height, (int)this->map_width / cell_width);

	/*for (int i = 0; i < listStaticObj.size(); i++)
	{
		float l, t, r, b;
		listStaticObj[i]->GetBoundingBox(l, t, r, b);
		int top = int(t / cell_height);
		int bottom = ceil(b / cell_height);
		int left = int(l / cell_width);
		int right = ceil(r / cell_width);

		if (!listStaticObj[i]->objectDisappear())
			for (int row = top; row < bottom; row++)
				for (int col = left; col < right; col++)
					allcells[row][col].push_back(listStaticObj[i]);
	}*/

	for (int i = 0; i < listDynamicObj.size(); i++)
	{
		float l, t, r, b;
		listDynamicObj[i]->GetBoundingBox(l, t, r, b);
		int top = int(t / cell_height);
		int bottom = ceil(b / cell_height);
		int left = int(l / cell_width);
		int right = ceil(r / cell_width);

		if (!listDynamicObj[i]->objectDisappear())
			for (int row = top; row < bottom; row++)
				for (int col = left; col < right; col++)
					allcells[row][col].push_back(listDynamicObj[i]);
	}

}

void Grid::PushGridStart(LPGAMEOBJECT obj, int row, int col)
{
	allcells[row][col].push_back(obj);
}


void Grid::ResetCamGrid(vector<LPGAMEOBJECT>& list)
{
	for (int i = 0; i < list.size(); i++)
		list[i]->isInCamera = false;
}

void Grid::GetObjInGrid(vector<LPGAMEOBJECT>& listStaticObj, vector<LPGAMEOBJECT>& listDynamicObj)
{
	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / cell_width);
	int lastCol = ceil((CGame::GetInstance()->GetCamPosX()  + CGame::GetInstance()->GetScreenWidth())  / cell_width);

	int firstRow = (int)(CGame::GetInstance()->GetCamPosY() / cell_height);
	int totalRow = ceil((CGame::GetInstance()->GetCamPosY() + (CGame::GetInstance()->GetScreenHeight())) / cell_height) ;

	for (int i = firstRow; i < totalRow; i++)
	{ 
		for (int j = firstCol; j < lastCol; j++)	
		{
			for (int k = 0; k < allcells[i][j].size(); k++)
			{
				if (!allcells[i][j][k]->isInCamera && !allcells[i][j][k]->objectDisappear())
				{
					if (allcells[i][j][k]->getCategoryObject() == Category::GROUND 
							|| allcells[i][j][k]->getCategoryObject() == Category::ITEM
						|| allcells[i][j][k]->getCategoryObject() == Category::BRICK
						)
					{
						allcells[i][j][k]->isInCamera = true;
						listStaticObj.push_back(allcells[i][j][k]);
					}
					else if (allcells[i][j][k]->getCategoryObject() == Category::ENEMY)
					{
						allcells[i][j][k]->isInCamera = true;
						listDynamicObj.push_back(allcells[i][j][k]);
					}
				}
			}
		}
	}
}

Grid::~Grid()
{
}

void Grid::Firstly_PushObjects(unordered_map<int, LPGAMEOBJECT> hashmapObjects)
{
	//for (size_t i = 0; i < objects_cell.size(); i++)
	{
		for (pair<int, vector<unordered_map<int, int>>> posCell : objects_cell)
		{
			int id_object = posCell.first;
			for (size_t i = 0; i < posCell.second.size(); i++)
			{
				if (id_object == 190)
				{
					int x = 3;
				}
				LPGAMEOBJECT obj = hashmapObjects[id_object];
				if (obj)
				{
					for (pair<int, int> positionCell  : posCell.second[i])
					{
						allcells[positionCell.second][positionCell.first].push_back(obj);
					}
				}
			}
		}
		
	}
}
