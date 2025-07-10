#include "Shadow.h"
#include "Engine/Model.h"
#include "Player.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

Shadow::Shadow(GameObject* parent):GameObject(parent, "Shadow")
{
}

Shadow::~Shadow()
{
}

void Shadow::Initialize()
{
	
	hShadowModel_ = Model::Load("Shadow1.fbx");
	assert(hShadowModel_ >= 0);

	transform_.scale_ = { 1.5,1.5,1.5 };
}

void Shadow::Update()
{
	//Œ»Ý‚ÍƒvƒŒƒCƒ„[‚Ì‰e‚Ì‚ÝŽÀ‘•’†
	Player* player = (Player*)FindObject("Player");
	if (!player) return;

	XMFLOAT3 pos = player->GetPosition();
	player->StandingStage(pos);

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
