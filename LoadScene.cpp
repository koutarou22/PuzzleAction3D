#include "LoadScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

namespace
{
	static float fadeAlpha = 0.00f; // 黒画像の透明度
	const float fadeSpeed = 0.02f; // フェードの速度
	bool isFadingOut = false;
    bool isLoading = false;
	int loadWaitTimer = 60;
}


LoadScene::LoadScene(GameObject* parent)
	: GameObject(parent, "LoadScene")
{
}

void LoadScene::Initialize()
{

	hBlackScreen_ = Image::Load("Scene//Black.png");
	hImage_ = Image::Load("Scene//LoadTest.png");
	//assert(hImage_ < 1);
}

void LoadScene::Update()
{
    if (TimeFlame_ > 0)
    {
        TimeFlame_--;
    }

    if (TimeFlame_ == 0 && !isFadingOut)
    {
        isFadingOut = true; // 暗転開始
    }

    // フェードアウト処理
    if (isFadingOut && !isLoading)
    {
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 1.0f)
        {
            fadeAlpha = 1.0f;
            isLoading = true; 
        }
    }

    if (isLoading)
    {
        loadWaitTimer--;
        if (loadWaitTimer <= 0)
        {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_PLAY); // シーン切り替え
        }
    }

	/*if (Input::IsKeyDown(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}*/
}

void LoadScene::Draw()
{
	Image::Draw(hImage_);

	Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);
	Image::Draw(hBlackScreen_);
}

void LoadScene::Release()
{
}
