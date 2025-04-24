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

	hModel_ = Model::Load("Shadow.fbx");
	assert(hModel_ >= 0);

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
