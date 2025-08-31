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
	//�����݂̓v���C���[�̉e�̂ݎ�����
	Player* player = (Player*)FindObject("Player");
	if (!player) return;

	XMFLOAT3 pos = player->GetPosition();
	player->StandingStage(pos);

	//�e�̈ʒu���v���C���[�̈ʒu�ɍ��킹��
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
