#include <iostream>
#include <fstream>
#include "PlayScence.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_Flower				4
#define OBJECT_TYPE_Ground				5
#define OBJECT_TYPE_ColorBox			6
#define OBJECT_TYPE_Pipe				7
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_WeakBrick			9
#define OBJECT_TYPE_CloudBrick			10
#define OBJECT_TYPE_Coin				11

#define OBJECT_TYPE_PORTAL				50

#define MAX_SCENE_LINE					1024

Map* mapScence;

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_CloudBrick:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new CloudBrick(width, height);
		break;
	}
	case OBJECT_TYPE_BRICK:
		{
			int width = atof(tokens[4].c_str());
			int height = atof(tokens[5].c_str());
			obj = new CBrick(width, height);
			break;
		}
		case OBJECT_TYPE_WeakBrick:
		{
			int width = atof(tokens[4].c_str());
			int height = atof(tokens[5].c_str());
			obj = new WeakBrick(width, height);
			break;
		}
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_Flower: obj = new CFlower(); break;
	case OBJECT_TYPE_Ground: 
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new Ground(width, height);
		break;
	}
	case OBJECT_TYPE_ColorBox:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new ColorBox(width, height);
		break;
	}
	case OBJECT_TYPE_Pipe:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new Pipe(width, height);
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new QuestionBrick(x,y,width, height);
		break;
	}
	case OBJECT_TYPE_Coin:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		int typeItem = atof(tokens[6].c_str());
		obj = new Item(width, height,typeItem,x,y);
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	mapScence = new Map();
	ifstream f;
	f.open(sceneFilePath);
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	objects.at(0)->Update(dt,&coObjects);

	/*for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}*/

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 3;
	cy -= game->GetScreenHeight() / 3;

	if (cx < 0) cx = 0;
	
	CGame::GetInstance()->SetCamPos(cx, 0 /*cy*/);
}

void CPlayScene::Render()
{
	
	//mapScence->DrawMap();

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	
	switch (KeyCode)
	{
	case (DIK_DOWN):
		mario->SetState(MARIO_STATE_STOP_SITTING);
		break;
	case (DIK_SPACE):
		mario->SetState(MARIO_ANI_SHORT_JUMP);
		break;
	case (DIK_LSHIFT):
		mario->SetState(MARIO_STATE_STOP_RUNNING);
		break;
	case (DIK_RIGHT):
		mario->SetState(MARIO_STATE_NOT_WALKING);
		break;
	case(DIK_LEFT):
		mario->SetState(MARIO_STATE_NOT_WALKING);
		break;
	case (DIK_Z):
		//mario->isAttacking = false;
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_RIGHT:
		mario->SetState(MARIO_STATE_WALKING);
		break;
	case DIK_LEFT:
		mario->SetState(MARIO_STATE_WALKING);
		break;
	case DIK_SPACE:
		if (mario->level == MARIO_LEVEL_BIG_TAIL)
		{
			mario->SetState(MARIO_STATE_BIG_TAIL_KEEP_JUMP);
		}
		break;
	case DIK_Z:
		if (mario->level == MARIO_LEVEL_BIG_TAIL)
		{
			if (mario->isAttacking == false)
			{
				mario->tDraw = GetTickCount64();
			}
			mario->isAttacking = true;
			mario->SetState(MARIO_STATE_BIG_TAIL_ATTACK);
		}
		break;
	case DIK_A: 
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	
	if (mario->isAttacking && GetTickCount64() - mario->tDraw < 500 && mario->	tDraw != 0)
	{
		return;
	}
	else if (mario->isAttacking && GetTickCount64() - mario->tDraw > 500) {
		mario->isAttacking = false;
		mario->tDraw = 0;
		return;
	}

	if (game->IsKeyDown(DIK_SPACE))
	{
		mario->SetState(MARIO_STATE_JUMP);
	}
	if (game->IsKeyDown(DIK_LSHIFT))
	{
		mario->SetState(MARIO_STATE_RUNNING);
	}
	if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if(game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_SITTING);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}