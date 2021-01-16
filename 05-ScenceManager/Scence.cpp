#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath,bool isWorldSeletion, int typeCamera)
{
	this->id = id;
	this->sceneFilePath = filePath;
	this->isWorldSeletion = isWorldSeletion;
	this->typeCamera = typeCamera;
}



