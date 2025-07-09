#include "ResidueItem.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

ResidueItem::ResidueItem(GameObject* parent) :GameObject(parent, "ResidueItem"), hResidue_(-1),
posX(0.0), posY(3.5), posZ(9.0)
{
	frequency_ = 0.5f;   
	amplitude_ = 0.2f;
}

ResidueItem::~ResidueItem()
{
}

void ResidueItem::Initialize()
{

	hResidue_ = Model::Load("RubyPosZero.fbx");
	assert(hResidue_ >= 0);

	transform_.position_ = { posX,posY,posZ };
	transform_.scale_ = { FAST_SCALE_X,FAST_SCALE_Y,FAST_SCALE_Z };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.3, 0.3, 0.3 });
	AddCollider(collision);
}

void ResidueItem::Update()
{
	ResidueVibrationAnimation();
}

void ResidueItem::Draw()
{
	Model::SetTransform(hResidue_, transform_);
	Model::Draw(hResidue_);
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

void ResidueItem::ResidueVibrationAnimation()
{
	deltaTime  =  DELTATIME;   // 1フレームの時間（秒換算）
	totalTime_ += deltaTime;   // 毎フレーム時間

	offsetY_ = sin(totalTime_ * frequency_) * amplitude_;

	transform_.rotate_.y += OBJECT_ROTATE_Y;  
	transform_.position_.y = posY + offsetY_;
}

void ResidueItem::SetBasePosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	transform_.position_ = { x, y, z };
}
