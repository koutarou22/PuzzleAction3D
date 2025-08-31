#include "Shadow.h"
#include "Engine/Model.h"
#include "Player.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	XMFLOAT3 SCALE = { 1.5f, 1.5f, 1.5f };
}

Shadow::Shadow(GameObject* parent):GameObject(parent, "Shadow")
{
}

Shadow::~Shadow()
{
}

void Shadow::Initialize()
{
	
	hShadowModel_ = Model::Load("Model//Shadow1.fbx");
	assert(hShadowModel_ >= 0);

	transform_.scale_ = { SCALE };
}

void Shadow::Update()
{
	//※現在はプレイヤーの影のみ実装中
	Player* player = (Player*)FindObject("Player");
	if (!player) return;

	XMFLOAT3 pos = player->GetPosition();
	player->StandingStage(pos);

	//影の位置をプレイヤーの位置に合わせる
	transform_.position_ = {
		pos.x,
		static_cast<float>(player->GetGroundHeight()),
		pos.z
	};

}

void Shadow::Draw()
{
	Model::SetTransform(hShadowModel_, transform_);
	Model::Draw(hShadowModel_);
}

void Shadow::Release()
{
}
