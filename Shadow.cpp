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
	
	hModel_ = Model::Load("Shadow1.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = { 1.5,1.5,1.5 };
}

void Shadow::Update()
{
}

void Shadow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		ImGui::Text("Shadow Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
	}
}

void Shadow::Release()
{
}
