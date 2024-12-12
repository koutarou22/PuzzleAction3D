#include "ClearScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

//�R���X�g���N�^
ClearScene::ClearScene(GameObject* parent)
	: GameObject(parent, "ClearScene")
{

}

//������
void ClearScene::Initialize()
{
	hImage_ = Image::Load("Clear.png");
	assert(hImage_ < 1);
}

//�X�V
void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

//�`��
void ClearScene::Draw()
{
	Image::Draw(hImage_);
}

//�J��
void ClearScene::Release()
{
	//��������͌��
}
