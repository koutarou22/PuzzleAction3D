#include "Ladder.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include "Engine/Collider.h"
#include "Player.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


Ladder::Ladder(GameObject* parent) :GameObject(parent, "Ladder"), hModel_(-1), posX(3.6), posY(1.8), posZ(2.0)
{
}

Ladder::~Ladder()
{
}

void Ladder::Initialize()
{
	hModel_ = Model::Load("Ladder.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = 90.0f;

	transform_.position_ = { posX,posY,posZ };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
	AddCollider(collision);
}

void Ladder::Update()
{
}

void Ladder::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("Ladder_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}
}

void Ladder::Release()
{
}

void Ladder::OnCollision(GameObject* parent)
{
	Player* pPlayer = (Player*)FindObject("Player");
	if (parent->GetObjectName() == "Player")
	{
		pPlayer->GetPosition().y += 0.10;
	}
}
