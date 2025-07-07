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



    StageImage_[LOAD_SCENE_STAGE1] = Image::Load("Image//StageNum//1-1.png");
    StageImage_[LOAD_SCENE_STAGE2] = Image::Load("Image//StageNum//1-2.png");
    StageImage_[LOAD_SCENE_STAGE3] = Image::Load("Image//StageNum//1-3.png");
	StageImage_[LOAD_SCENE_STAGE4] = Image::Load("Image//StageNum//1-4.png");

	assert(StageImage_[LOAD_SCENE_STAGE1] >= 0);
	assert(StageImage_[LOAD_SCENE_STAGE2] >= 0);
	assert(StageImage_[LOAD_SCENE_STAGE3] >= 0);
	assert(StageImage_[LOAD_SCENE_STAGE4] >= 0);

    hBlackScreen_ = Image::Load("Scene//Black.png");
    hImage_ = Image::Load("Scene//forest-path4.jpg");

    assert(hBlackScreen_ >= 0);
    assert(hImage_ >= 0);
}



void LoadScene::Update()
{

    Residue* pResidue = (Residue*)FindObject("Residue");
    pResidue = Instantiate<Residue>(this);

    //LoadScene用に位置を調整
    pResidue->SetPosition(-0.1, -0.2, 0);


    SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

    StageNumber_ = pSceneManager->GetStageNumber(); // ステージ番号を取得
    switch (StageNumber_)
    {
    case LOAD_SCENE_STAGE1:
	     StageImage_[LOAD_SCENE_STAGE1]; 
		break;
	case LOAD_SCENE_STAGE2:
		StageImage_[LOAD_SCENE_STAGE2]; 
        break;
	case LOAD_SCENE_STAGE3:
		StageImage_[LOAD_SCENE_STAGE3]; 
        break;
    default:
        StageImage_[LOAD_SCENE_STAGE1];
        break;
    }

	StageNumberCurrent_ = StageImage_[StageNumber_]; // 現在のステージ画像を設定




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
            pSceneManager->ChangeScene(SCENE_ID_PLAY); // シーン切り替え
        }
    }
}

void LoadScene::Draw()
{
	Image::Draw(hImage_);
    Image::Draw(StageNumberCurrent_);
	Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);
	Image::Draw(hBlackScreen_);
}

void LoadScene::Release()
{
    fadeAlpha = 0.0f; 
}
