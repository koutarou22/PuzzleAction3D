#include "KeyFlag.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

KeyFlag::KeyFlag(GameObject* parent) :GameObject(parent, "KeyFlag"), hModel_(-1),posX(7.0),posY(4.5),posZ(5.0),Random(rand()%3)
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
    
	switch (Random)
	{
	case 0:
		KeyPos[0][0][0] = { 4.0, 6.5, 9.0 };
		transform_.position_ = KeyPos[0][0][0];
		break;
	case 1:
		KeyPos[1][1][1] = { 4.0, 2.5, 6.0 };
		transform_.position_ = KeyPos[1][1][1];
		break;
	case 2:
		KeyPos[2][2][2] = { 9.0, 2.5, 9.0 };
		transform_.position_ = KeyPos[2][2][2];
		break;

	default:
		break;
	}

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
	AddCollider(collision);
}

void KeyFlag::Update()
{

}

void KeyFlag::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//{
 //       static float pos[3] = { posX,posY,posZ };
 //       ImGui::Separator();
	//	
 //       if (ImGui::InputFloat3("Key_Position", pos, "%.3f"))
 //       {
 //           transform_.position_ = { pos[0],pos[1], pos[2] };
 //       }
	//}

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
