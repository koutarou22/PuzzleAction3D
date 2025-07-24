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

	const float ROTATE_SPEED = 2.0f; // 回転速度
	const float ROTATE_MAX = 90.0f; // 最大回転角度

	const float POSITION_SPEED = 0.01f; // X軸の位置オフセット

	XMFLOAT3 ColliderSize = { 1.0f, 1.0f, 1.0f }; // コライダーのサイズ
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

	hDoorSound_[DOOR_OPEN_SE] = Audio::Load(DoorPath + "OpenTheGate.wav");
	assert(hDoorSound_[DOOR_OPEN_SE] >= 0);

	BoxCollider* collision = new BoxCollider({ 0, 0, 0}, { ColliderSize });
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

		

			transform_.rotate_.y += ROTATE_SPEED;
			transform_.position_.x -= POSITION_SPEED;

			if (transform_.rotate_.y >= ROTATE_MAX) 
			{
				transform_.rotate_.y = ROTATE_MAX; 
				
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
