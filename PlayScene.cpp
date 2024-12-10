#include "PlayScene.h"
#include "Player.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Stage.h"
#include "CameraController.h"

PlayScene::PlayScene(GameObject* parent) :GameObject(parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//Instantiate<Controller>(this);
	Instantiate<CameraController>(this);
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
