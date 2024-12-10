#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

//初期化
void TitleScene::Initialize()
{
	hImage_ = Image::Load("Test.png");
	assert(hImage_ < 1);
}

//更新
void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//描画
void TitleScene::Draw()
{
	Image::Draw(hImage_);
}

//開放
void TitleScene::Release()
{
	//解放処理は後程
}
