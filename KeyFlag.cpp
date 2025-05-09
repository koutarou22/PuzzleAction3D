#include "KeyFlag.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

//振動する値の管理
namespace 
{
	//
	const float MOVE_AMPLITUDE = 0.1f;//振れ幅
	const float MOVE_FREQUENCY_SPEED = 1.0f;// 揺れの速さを調整
	const float DELTATIME = 1.0f / 60.0f;
		
	//初期
	const float OBJECT_ROTATE_X = 30;

	const float OBJECT_ROTATE_Y = 0.3f;
	//

	//オブジェクトの最初の位置
	const float FAST_POSITION_X = 9.0f;
	const float FAST_POSITION_Y = 6.5f;
	const float FAST_POSITION_Z = 9.0f;

}
KeyFlag::KeyFlag(GameObject* parent) :GameObject(parent, "KeyFlag"), 
hModel_(-1),posX(FAST_POSITION_X),posY(FAST_POSITION_Y),posZ(FAST_POSITION_Z),
Random(rand()%3),
amplitude_(MOVE_AMPLITUDE),frequency_(MOVE_FREQUENCY_SPEED)
{
}

KeyFlag::~KeyFlag()
{
}

void KeyFlag::Initialize()
{
    
	hModel_ = Model::Load("Key.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = 90.0f;

	transform_.position_ = { posX,posY,posZ };
	
	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.3, 0.3, 0.3 });
	AddCollider(collision);
}

void KeyFlag::Update()
{
	VibrationAnimation();
}

void KeyFlag::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
        static float pos[3] = { posX,posY,posZ };
        ImGui::Separator();
		
        if (ImGui::InputFloat3("Key_Position", pos, "%.3f"))
        {
            transform_.position_ = { pos[0],pos[1], pos[2] };
        }
	}
}

void KeyFlag::Release()
{
}

void KeyFlag::OnCollision(GameObject* parent)
{
	if (parent->GetObjectName() == "Player")
	{
		KillMe();
	}
}

void KeyFlag::VibrationAnimation()
{
	deltaTime = DELTATIME;     // 1フレームの時間（秒換算）
	totalTime_ += deltaTime;   // 毎フレーム時間

	//sinf関数を使って計算
	float yOffset = sinf(totalTime_* frequency_) * amplitude_;

	transform_.rotate_.x =  OBJECT_ROTATE_X;//X軸を回転
	transform_.rotate_.y += OBJECT_ROTATE_Y;//Y軸を回転

	transform_.position_.y = posY + yOffset;  //位置に渡す
}
