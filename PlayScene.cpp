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
#include "RemainItem.h"
#include "Shadow.h"
#include "Engine/Audio.h"
#include "Fadein.h"

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

	hPlaySound_ = Audio::Load("Sound//BGM//loop100612_volume_-64pct.wav", true, 1);
	assert(hPlaySound_ >= 0);
}

void PlayScene::Update()
{
	Audio::Play(hPlaySound_);
}

void PlayScene::Draw()
{
	Image::SetTransform(hBackImage_, transform_);
	Image::Draw(hBackImage_);
}

void PlayScene::Release()
{
}
