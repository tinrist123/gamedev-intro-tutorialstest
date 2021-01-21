#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameObject.h"
#include <fstream>
#include <string>
#include "Utils.h"
#include <utility>

using namespace std;

#define MAX_SCENE_LINE					1024
#define SCENE_SECTION_MAP				1
#define SCENE_SECTION_OBJECT			2
#define SCENE_SECTION_UNKNOWN			-1

#define MIN_CELL_WIDTH_COLLECTED		1
#define MAX_CELL_WIDTH_COLLECTED		2

#define MIN_CELL_HEIGHT_COLLECTED		1
#define MAX_CELL_HEIGHT_COLLECTED		1


int cell_width;
int cell_height;

int map_screen_width;
int map_screen_height;

int Screen_Game_Width;
int Screen_Game_Height;



struct object_value_Id
{
	string typeObj;
	int posX;
	int posY;
	int ID_Grid;

	void StoreValue(string typeObj, int x, int y, int ID_Grid)
	{
		this->typeObj = typeObj;
		this->posX = x;
		this->posY = y;
		this->ID_Grid = ID_Grid;
	}

};

vector<object_value_Id> listObject_value;

// Format CELL POSITION - OBJECTS IN CELL
unordered_map<int, unordered_map<int, vector<string>>> allCells;

// Type--- List<X-Y>
unordered_map<string,vector<unordered_map<int,int>>> allObjKey_Value;


unordered_map<int,int> listNumber_store;

bool checkExistObjInCell(string typeObj, int posX, int posY)
{
	for (unordered_map<int, int> list : allObjKey_Value[typeObj])
	{
		if (list[posX] == posY)
		{
			return true;
		}
	}
	return false;
}
void storeObjKey_value(vector <string> tokens)
{
	string typeObj = tokens[0];

	unordered_map<int, int> positionObj({
		{atoi(tokens[1].c_str()),
		atoi(tokens[2].c_str())}
		});

	allObjKey_Value[typeObj].push_back(positionObj);
}

bool checkObjValue(string typeObj, int x, int y)
{
	for (int i = 0; i < listObject_value.size(); i++)
	{
		if (listObject_value[i].posX == x
			&&
			listObject_value[i].posY == y
			&&
			listObject_value[i].typeObj == typeObj
			)
		{
			return true;
		}
	}
	return false;
}

int getObjValue_Id(string typeObj, int x, int y)
{
	for (int i = 0; i < listObject_value.size(); i++)
	{
		if (listObject_value[i].posX == x
			&&
			listObject_value[i].posY == y
			&&
			listObject_value[i].typeObj == typeObj
			)
		{
			return listObject_value[i].ID_Grid;
		}
	}

	return -1;
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

string join(const vector<string>& lst, const string& delim)
{
	string ret;
	for (const auto& s : lst) {
		if (!ret.empty())
			ret += delim;
		ret += s;
	}
	return ret;
}

// RETURN WIDTH OR SIZE OF CELL Use for Grid
int DetectCell_Size(int map_screen, int min_rage,int max_range,int map_screen_game)
{
	int range_of_cell_min = floor((min_rage * map_screen) / map_screen_game);
	int range_of_cell_max = floor((max_range * map_screen) / map_screen_game);
	
	int direct_cell = range_of_cell_min +(range_of_cell_max - range_of_cell_min) / 2 ;

	return floor(map_screen / direct_cell); 
}

void DetectCell_Grid()
{
	cell_width  = DetectCell_Size(map_screen_width, MIN_CELL_WIDTH_COLLECTED, MAX_CELL_WIDTH_COLLECTED, Screen_Game_Width);
	cell_height = DetectCell_Size(map_screen_height, MIN_CELL_HEIGHT_COLLECTED, MAX_CELL_HEIGHT_COLLECTED, Screen_Game_Height);
}

void _ParseSection_Map(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2) return;

	map_screen_width = atoi(tokens[0].c_str());
	map_screen_height = atoi(tokens[1].c_str());
	Screen_Game_Width = atoi(tokens[2].c_str());
	Screen_Game_Height = atoi(tokens[3].c_str());

	DetectCell_Grid();
}

void TinNgoGetBoundingBox(float& l, float& t, float& r,float &b, vector<string> tokens)
{
	int x = atoi(tokens[1].c_str());
	int y = atoi(tokens[2].c_str());

	int lastIndexString = tokens.size() - 1;
	int preLastIndexString = tokens.size() - 2;

	l = x;
	t = y;
	r = x + atoi(tokens[preLastIndexString].c_str());
	b = y + atoi(tokens[lastIndexString].c_str());


}

void formatLineString(vector<string> tokensLine, int left, int top, int right, int bottom)
{
	int lastIndexString = tokensLine.size() - 1;
	int preLastIndexString = tokensLine.size() - 2;

	for (int row = top; row < bottom; row++)
	{
		for (int col = left; col < right; col++)
		{

			// Clone value to detect last col
			vector<string> tokens = tokensLine;

			string typeObj = tokens[0];
			int posX = atoi(tokens[1].c_str());
			int posY = atoi(tokens[2].c_str());
			int randomId = rand() % 200;

			if (checkExistObjInCell(typeObj,posX,posY))
			{
				if (checkObjValue(typeObj, posX, posY))
				{
					randomId = getObjValue_Id(typeObj,posX,posY);
				}
			}
			else
			{
				while (listNumber_store[randomId] == 1)
				{
					randomId = rand() % 200;
				}
				listNumber_store[randomId] = 1;

				object_value_Id obj;

				obj.StoreValue(typeObj, posX, posY, randomId);
				listObject_value.push_back(obj);
				storeObjKey_value(tokensLine);
			}

			if (randomId == 48)
			{
				int x = 3;
			}

			tokens.push_back(to_string(randomId));
			// Change BBox value to Cell position Value
			tokens[lastIndexString] = to_string(col);
			tokens[preLastIndexString] = to_string(row);

			// Join to string
			string line = join(tokens, "\t");
			allCells[row][col].push_back(line);
		}
	}

}




void _ParseSection_OBJECT(string line)
{
	vector<string> tokens = split(line);

	float l, t,  r,b;
	TinNgoGetBoundingBox(l, t, r, b, tokens);

	int top = int(t / cell_height);
	int bottom = ceil(b / cell_height);
	int left = int(l / cell_width);
	int right = ceil(r / cell_width);

	formatLineString(tokens,left,top,right, bottom);

	

	//koopas->GetBoundingBox(l, t, r, b);
	
}

void formatListStringAscending(unordered_map<int, vector<string>> listString)
{

}

void exportFile_ObjectsAndCell()
{
	string fileName = "cell_scene2.txt";
	ofstream myfile;
	myfile.open(fileName);

	if (myfile.is_open())
	{
		for (int i = 0; i < allCells.size(); i++)
		{
			for (pair<int, vector<string>> element : allCells[i])
			{
				myfile << "#CELL " << i << " " << element.first << endl;
				for (string line : element.second)
				{
					myfile << line << endl;
				}
			}
		}
	}
	myfile.close();
}

int main()
{
	ifstream fileObjPosition("scene2.txt");

	int section = SCENE_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	
	if (fileObjPosition.is_open())
	{
		while (fileObjPosition.getline(str, MAX_SCENE_LINE))
		{
			string line(str);
			
			if (line == "") continue;
			if (line[0] == '#') continue;
			if (line == "[MAP]")
			{
				section = SCENE_SECTION_MAP;
				continue;
			}
			if (line == "[OBJECTS]")
			{
				section = SCENE_SECTION_OBJECT;
				continue;
			}
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

			switch (section)
			{
				case SCENE_SECTION_MAP: _ParseSection_Map(line); break;
				case SCENE_SECTION_OBJECT: _ParseSection_OBJECT(line); break;
			}
		}
		fileObjPosition.close();
	}

	exportFile_ObjectsAndCell();

	return 0;
}




