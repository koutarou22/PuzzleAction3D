#include "Shadow.h"
#include "Engine/Model.h"

Shadow::Shadow(GameObject* parent)
{
}

Shadow::~Shadow()
{
}

void Shadow::Initialize()
{

	hModel_ = Model::Load("Shadow1.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = { 1.0,1.1,1.0 };

	transform_.scale_ = { 1.5,1.5,1.5 };
}

void Shadow::Update()
{
}

void Shadow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Shadow::Release()
{
}
