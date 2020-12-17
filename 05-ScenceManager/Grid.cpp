#include "Grid.h"

Grid::Grid(int cell_width, int cell_height)
{
	this->cell_width = cell_width;
	this->cell_height = cell_height;
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
		for (int j = 0; j < nums_col + 1; j++)
			allcells[i][j].clear();
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

void Grid::PushGridStart(LPGAMEOBJECT obj, int row, int col)
{
	allcells[row][col].push_back(obj);
}

void Grid::ResetGrid(vector<LPGAMEOBJECT> list)
{
	ClearGrid((int)this->map_height / cell_height, (int)this->map_width / cell_width);

	PushGrid(list);
}

void Grid::CheckCamGrid(vector<LPGAMEOBJECT>& list)
{
	for (int i = 0; i < list.size(); i++)
		list[i]->isInCamera = false;
}


void Grid::GetGrid(vector<LPGAMEOBJECT>& list)
{
	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / cell_width);
	int lastCol = ceil((CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH)  / cell_width);
	int totalRow = this->map_height / cell_height;

	DebugOut(L"%f\n", CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH);

	for (int i = 0; i < totalRow; i++)
	{
		for (int j = firstCol; j < lastCol; j++)	
		{
			for (int k = 0; k < allcells[i][j].size(); k++)
			{
				if (!allcells[i][j][k]->isInCamera && !allcells[i][j][k]->objectDisappear())
				{
					allcells[i][j][k]->isInCamera = true;
					list.push_back(allcells[i][j][k]);
				}
			}
		}
	}
}

Grid::~Grid()
{
}