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
	//Œ»İ‚ÍƒvƒŒƒCƒ„[‚Ì‚İ‚Ì‰e‚Ì‚İÀ‘•’†

	

}

void Shadow::Draw()
{
	Model::SetTransform(hShadowModel_, transform_);
	Model::Draw(hShadowModel_);
}

void Shadow::Release()
{
}
