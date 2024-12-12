#include "ClearScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

//コンストラクタ
ClearScene::ClearScene(GameObject* parent)
	: GameObject(parent, "ClearScene")
{

}

//初期化
void ClearScene::Initialize()
{
	hImage_ = Image::Load("Clear.png");
	assert(hImage_ < 1);
}

//更新
void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

//描画
void ClearScene::Draw()
{
	Image::Draw(hImage_);
}

//開放
void ClearScene::Release()
{
	//解放処理は後程
}
