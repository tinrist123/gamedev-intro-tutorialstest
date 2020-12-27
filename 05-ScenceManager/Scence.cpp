#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath,bool isWorldSeletion)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->isWorldSeletion = isWorldSeletion;
}



