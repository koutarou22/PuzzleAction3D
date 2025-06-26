#include "ResidueItem.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

ResidueItem::ResidueItem(GameObject* parent) :GameObject(parent, "ResidueItem"), hModel_(-1),
posX(0.0), posY(3.5), posZ(9.0)
{
}

ResidueItem::~ResidueItem()
{
}

void ResidueItem::Initialize()
{

	hModel_ = Model::Load("RubyPosZero.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = { posX,posY,posZ };
	transform_.scale_ = { 0.5,0.5,0.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.3, 0.3, 0.3 });
	AddCollider(collision);
}

void ResidueItem::Update()
{
	deltaTime = DELTATIME;     // 1フレームの時間（秒換算）
	totalTime_ += deltaTime;   // 毎フレーム時間

	//sinf関数を使って計算
	float yOffset = sinf(totalTime_ * frequency_) * amplitude_;

	//transform_.position_.y = posY + yOffset;  //位置に渡す
}

void ResidueItem::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("ResidueItem_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}

}

void ResidueItem::Release()
{
}

void ResidueItem::OnCollision(GameObject* parent)
{
	if (parent->GetObjectName() == "Player")
	{
		KillMe();
	}
}
