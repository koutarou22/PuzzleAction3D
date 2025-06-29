#include "GoalDoor.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Engine/Collider.h"
#include "Player.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


GoalDoor::GoalDoor(GameObject* parent) :GameObject(parent, "GoalDoor"), hDoorModel_(-1), hOpenLight_(-1)
{

}

GoalDoor::~GoalDoor()
{
}

void GoalDoor::Initialize()
{
	hDoorModel_ = Model::Load("GoalFlag.fbx"); 
	assert(hDoorModel_ >= 0);

	/*hOpenLight_ = Model::Load("GoalLight.fbx");
	assert(hOpenLight_ >= 0);*/

	transform_.position_ = { 4,2.5,9 };
	lightTrs.position_ = { 4,2.5,9 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0}, { 1, 1, 1});
	AddCollider(collision);
}

bool isRotationComplete = false; 

void GoalDoor::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
	{
		GoalFlag_ = pPlayer->GetClearFlag();

		if (GoalFlag_ && !isRotationComplete) 
		{
			transform_.rotate_.y += 2.0f;
			transform_.position_.x -= 0.01;

			if (transform_.rotate_.y >= 90.0f) 
			{
				transform_.rotate_.y = 90.0f; 
				
				isRotationComplete = true;  
			}
		}
	}

}

void GoalDoor::Draw()
{
	Model::SetTransform(hDoorModel_, transform_);
	Model::Draw(hDoorModel_); 
	
	/*Model::SetTransform(hOpenLight_, lightTrs); 
	Model::Draw(hOpenLight_);*/
}

void GoalDoor::Release()
{
}

void GoalDoor::OnCollision(GameObject* parent)
{
}
