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

#include "Ghost.h"
#include "TurretEnemy.h"
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
	Instantiate<Player>(this);
	Instantiate<CameraController>(this);
	
	Instantiate<Stage>(this);
	Instantiate<Shadow>(this);

	Instantiate<UI>(this);

	hBackImage_ = Image::Load("Scene//cloudDSC03662_TP_V.jpg");
	assert(hBackImage_ >= 0);

	hPlaySound_ = Audio::Load("Sound//BGM//loop100612.wav", true, 1);
	assert(hPlaySound_ >= 0);

	CameraController* pCamera = (CameraController*)FindObject("CameraController");

	if (pCamera != nullptr)
	{
		pCamera->ResetCamera();
	}

}

void PlayScene::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");



	//Audio::Play(hPlaySound_);
}

void PlayScene::Draw()
{
	Image::SetTransform(hBackImage_, transform_);
	Image::Draw(hBackImage_);
}

void PlayScene::Release()
{
}
