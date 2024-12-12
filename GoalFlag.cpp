#include "GoalFlag.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

GoalFlag::GoalFlag(GameObject* parent) :GameObject(parent, "GoalFlag"),hModel_(-1)
{
}

GoalFlag::~GoalFlag()
{
}

void GoalFlag::Initialize()
{
	hModel_ = Model::Load("GoalFlag.fbx");
	//assert(hModel_ < 1);

	transform_.rotate_.y = 180.0f;
	transform_.position_.y = 3.0;
	transform_.position_.x = 4.0;
	transform_.position_.z = 9.2;

	BoxCollider* collision = new BoxCollider({ 0, 0, 0}, { 1, 1, 1});
	AddCollider(collision);
}

void GoalFlag::Update()
{
}

void GoalFlag::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void GoalFlag::Release()
{
}

void GoalFlag::OnCollision(GameObject* parent)
{
	if (parent->GetObjectName() == "Player")
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
}
