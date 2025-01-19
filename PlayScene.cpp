#include "PlayScene.h"
#include "Player.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Stage.h"
#include "CameraController.h"
#include "GoalFlag.h"
#include "Enemy.h"
#include "UI.h"
#include "KeyFlag.h"

PlayScene::PlayScene(GameObject* parent) :GameObject(parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Instantiate<CameraController>(this);
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<GoalFlag>(this);
	Instantiate<KeyFlag>(this);
	Instantiate<UI>(this);
}

void PlayScene::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer == nullptr)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
