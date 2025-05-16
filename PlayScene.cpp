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
#include "ResidueItem.h"
#include "Shadow.h"
#include "Engine/Audio.h"

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
	Instantiate<ResidueItem>(this);
	Instantiate<GoalDoor>(this);
	Instantiate<KeyFlag>(this);
	//Instantiate<BulletEnemy>(this);
	Instantiate<MoveEnemy>(this);

	Instantiate<Shadow>(this);

	Instantiate<UI>(this);

	hBackImage_ = Image::Load("Scene//cloudDSC03662_TP_V.jpg");
	assert(hBackImage_ >= 0);

	hPlaySound_ = Audio::Load("Sound//loop100612.wav", true, 1);
	assert(hPlaySound_ < 1);

}

void PlayScene::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	Audio::Play(hPlaySound_);
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
