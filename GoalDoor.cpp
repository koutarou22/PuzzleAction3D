#include "GoalDoor.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Engine/Collider.h"
#include "Engine/Audio.h"

#include "Player.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


namespace
{
	const int STAGE_WIDTH = 10;      // ステージの横幅（X軸）
	const int STAGE_HEIGHT = 10;      // ステージの奥行き（Y軸）
	const int STAGE_LEVEL = 10;      // ステージの高さ（Z軸）

	const float BLOCK_POSITION_OFFSET_Y = 0.5f;          // Y軸補正（AdjustLevel_）
	const float BLOCK_SCALE_Y = 1.0f;          // Zレイヤー毎の高さスケール
	const float CENTER_OFFSET_X = STAGE_WIDTH / 2.0f;
	const float CENTER_OFFSET_Y = STAGE_HEIGHT / 2.0f;
}

GoalDoor::GoalDoor(GameObject* parent) :GameObject(parent, "GoalDoor"), hDoorModel_(-1), hOpenLight_(-1)
{
	isRotationComplete = false;
}

GoalDoor::~GoalDoor()
{
}

void GoalDoor::Initialize()
{
	hDoorModel_ = Model::Load("GoalFlag.fbx"); 
	assert(hDoorModel_ >= 0);


	/*hOpenLight_ = Model::Load("GoalLight.fbx");
	assert(hOpenLight_ >= 0);*/
	hDoorSound_[DOOR_OPEN_SE] = Audio::Load(DoorPath + "OpenTheGate.wav");
	assert(hDoorSound_[DOOR_OPEN_SE] >= 0);

	transform_.position_ = { 4,2.5,9 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0}, { 1, 1, 1});
	AddCollider(collision);
}



void GoalDoor::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	EmitterData Door;

	if (pPlayer != nullptr)
	{
		GoalFlag_ = pPlayer->GetClearFlag();

		if (GoalFlag_ && !isRotationComplete) 
		{
			Audio::Play(hDoorSound_[DOOR_OPEN_SE]);


			Door.textureFileName = "PaticleAssets//star.png";
			Door.position = transform_.position_;
			Door.positionRnd = XMFLOAT3(0.1, 0, 0.1);
			Door.delay = 0;
			Door.number = 1;
			Door.lifeTime = 60;
			Door.gravity = -0.002f;
			Door.direction = XMFLOAT3(0, 1, 0);
			Door.directionRnd = XMFLOAT3(0, 0, 0);
			Door.speed = 0.001f;
			Door.speedRnd = 0.0;
			Door.size = XMFLOAT2(1.0, 1.0);
			Door.sizeRnd = XMFLOAT2(0.4, 0.4);
			Door.scale = XMFLOAT2(1.01, 1.01);
			Door.color = XMFLOAT4(1, 1, 0, 1);
			Door.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);
			VFX::Start(Door);

			transform_.rotate_.y += 2.0f;
			transform_.position_.x -= 0.01;

			if (transform_.rotate_.y >= 90.0f) 
			{
				transform_.rotate_.y = 90.0f; 
				
				isRotationComplete = true;  
			}
		}
	}

}

void GoalDoor::Draw()
{
	Model::SetTransform(hDoorModel_, transform_);
	Model::Draw(hDoorModel_); 
	
	/*Model::SetTransform(hOpenLight_, lightTrs); 
	Model::Draw(hOpenLight_);*/
}

void GoalDoor::Release()
{
}

void GoalDoor::OnCollision(GameObject* parent)
{
}
