#include "GoalDoor.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Engine/Collider.h"
#include "Player.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


GoalDoor::GoalDoor(GameObject* parent) :GameObject(parent, "GoalDoor"),hModel_(-1),Random(rand() % 3)
{

}

GoalDoor::~GoalDoor()
{
}

void GoalDoor::Initialize()
{
	hModel_ = Model::Load("GoalFlag.fbx");
	assert(hModel_ >= 0);

	//transform_.rotate_.y = 180.0f;

	switch (Random)
	{
	case 0:
		GoalPos[0][0][0] = { 4.0 ,6.4,9.2 };
		transform_.position_ = GoalPos[0][0][0];
		break;

	case 1:
		GoalPos[1][1][1] = { 5.0 ,1.4 ,7.2 };
		transform_.position_ = GoalPos[1][1][1];
		break;

	case 2:
		GoalPos[2][2][2] = { 6.0 ,1.4, 4.2 };
		transform_.position_ = GoalPos[2][2][2];
		break;

	default:



		break;
	}
	

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

		if (GoalFlag_ && !isRotationComplete) // ��]�������Ă��Ȃ��ꍇ�̂ݏ���
		{
			transform_.position_.x -= 0.02f;
			transform_.rotate_.y += 5.0f;

			if (transform_.rotate_.y >= 90.0f) // ��]��90�x�𒴂����ꍇ
			{
				transform_.rotate_.y = 90.0f; 
				transform_.rotate_.x = 4.0f; 
				isRotationComplete = true;   // ��]�����t���O�𗧂Ă�
			}
		}
	}

}

void GoalDoor::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//{
	//	static float pos[3] = { posX,posY,posZ };
	//	ImGui::Separator();

	//	if (ImGui::InputFloat3("Goal_Position", pos, "%.3f"))
	//	{
	//		transform_.position_ = { pos[0],pos[1], pos[2] };
	//	}
	//}
}

void GoalDoor::Release()
{
}

void GoalDoor::OnCollision(GameObject* parent)
{
}
