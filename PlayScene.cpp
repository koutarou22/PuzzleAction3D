#include "PlayScene.h"
#include "Player.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Stage.h"
#include "CameraController.h"
#include "GoalDoor.h"

#include "UI.h"
#include "KeyFlag.h"
#include "Ladder.h"
#include "MoveEnemy.h"
#include "BulletEnemy.h"
#include "ScoreItem.h"

PlayScene::PlayScene(GameObject* parent) :GameObject(parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Instantiate<CameraController>(this);
	Instantiate<Player>(this);
	Instantiate<Stage>(this);

	Instantiate<GoalDoor>(this);
	Instantiate<KeyFlag>(this);
	Instantiate<BulletEnemy>(this);
	Instantiate<MoveEnemy>(this);

	Instantiate<UI>(this);

	hBackImage_ = Image::Load("Scene//cloudDSC03662_TP_V.jpg");
	assert(hBackImage_ >= 0);

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
	Image::SetTransform(hBackImage_, transform_);
	Image::Draw(hBackImage_);
}

void PlayScene::Release()
{
}
