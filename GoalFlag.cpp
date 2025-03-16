#include "GoalFlag.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Engine/Collider.h"
#include "Player.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


GoalFlag::GoalFlag(GameObject* parent) :GameObject(parent, "GoalFlag"),hModel_(-1), posX(4.0), posY(6.4), posZ(9.2)
{
}

GoalFlag::~GoalFlag()
{
}

void GoalFlag::Initialize()
{
	hModel_ = Model::Load("GoalFlag.fbx");
	assert(hModel_ >= 0);

	//transform_.rotate_.y = 180.0f;

	transform_.position_ = { posX,posY,posZ };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0}, { 1, 1, 1});
	AddCollider(collision);
}

bool isRotationComplete = false; // 回転完了フラグを追加

void GoalFlag::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
	{
		SetGoalFlag_ = pPlayer->GetClearFlag();

		if (SetGoalFlag_ && !isRotationComplete) // 回転完了していない場合のみ処理
		{
			transform_.position_.x -= 0.02f;
			transform_.rotate_.y += 5.0f;

			if (transform_.rotate_.y >= 90.0f) // 回転が90度を超えた場合
			{
				transform_.rotate_.y = 90.0f; // 90度に固定
				transform_.rotate_.x = 3.9f; // 90度に固定
				isRotationComplete = true;   // 回転完了フラグを立てる
			}
		}
	}

}

void GoalFlag::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("Goal_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}
}

void GoalFlag::Release()
{
}

void GoalFlag::OnCollision(GameObject* parent)
{
}
