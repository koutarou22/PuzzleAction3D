#include "ScoreItem.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

ScoreItem::ScoreItem(GameObject* parent) :GameObject(parent, "ScoreItem"), hModel_(-1), posX(3.0), posY(1.5), posZ(5.0)
{
}

ScoreItem::~ScoreItem()
{
}

void ScoreItem::Initialize()
{

	hModel_ = Model::Load("RubyPosZero.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = { posX,posY,posZ };
	transform_.scale_ = { 0.5,0.5,0.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.8, 0.8, 0.8 });
	AddCollider(collision);
}

void ScoreItem::Update()
{

}

void ScoreItem::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("ScoreItem_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}

}

void ScoreItem::Release()
{
}

void ScoreItem::OnCollision(GameObject* parent)
{
	if (parent->GetObjectName() == "Player")
	{
		KillMe();
	}
}
