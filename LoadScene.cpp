#include "LoadScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Residue.h"

namespace
{
	static float fadeAlpha = 0.00f; // 黒画像の透明度
	const float fadeSpeed = 0.02f; // フェードの速度
	bool isFadingOut;
    bool isLoading;
	int loadWaitTimer;
}


LoadScene::LoadScene(GameObject* parent)
	: GameObject(parent, "LoadScene")
{
}

void LoadScene::Initialize()
{
    fadeAlpha = 0.0f;
    isFadingOut = false;
    isLoading = false;
    loadWaitTimer = 180;

    StageImage_ = Image::Load("Image//49jof.png");
    hBlackScreen_ = Image::Load("Scene//Black.png");
    hImage_ = Image::Load("Scene//forest-path4.jpg");

    assert(StageImage_ >= 0);
    assert(hBlackScreen_ >= 0);
    assert(hImage_ >= 0);
}



void LoadScene::Update()
{
    if (TimeFlame_ > 0)
    {
        TimeFlame_--;
    }

    if (TimeFlame_ == 0 && !isFadingOut)
    {
        isFadingOut = true; 
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
}

void LoadScene::Draw()
{
	Image::Draw(hImage_);
    Image::Draw(StageImage_);
	Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);
	Image::Draw(hBlackScreen_);
}

void LoadScene::Release()
{
    fadeAlpha = 0.0f; 
}
