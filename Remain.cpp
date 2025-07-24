#include "Remain.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/SceneManager.h"


namespace
{
	XMFLOAT3 SCALE = { 0.6f, 0.6f, 0.6f };// スケールの初期値
	XMFLOAT3 NUM_SCALE = { 0.3,0.3,0.3 };// 数字のスケールの初期値

	float ALPHA = 1.0f; // アルファ値の初期値

	float offsetX = 0.2f; // 数字のオフセットX値

	float IMAGE_CUT_X = 1024 / 5;// 画像の切り取り幅
	float IMAGE_CUT_Y = 821 / 3; // 画像の切り取り高さ

	const int REMAIN_MIN = 1; // 残機の最小値
}

Remain::Remain(GameObject* parent):GameObject(parent,"Remain")
,posX(-0.9),posY(0.9),posZ(0.0)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		// SceneManager から残機を持ってくる
		LIFE_ = pSceneManager->GetPlayerRemain();
	}
}

Remain::~Remain()
{
}

void Remain::Initialize()
{
	hRemainImage_ = Image::Load("Image/num1.png");
	hNum_ = Image::Load("Image/Image4.png");

	assert(hRemainImage_ >= 0);

	RemaindueAlpha_ = ALPHA;

	trs.position_.x = posX + offsetX;
	trs.position_.y = posY;

	transform_.scale_ = { SCALE };
	trs.scale_ = { NUM_SCALE };
	transform_.position_ = { posX,posY,posZ };
}

void Remain::Update()
{
}

void Remain::Draw()
{

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	//画像を指定して切り取る
	int spriteWidth = IMAGE_CUT_X;
	int spriteHeight = IMAGE_CUT_Y;

	//残機を取得
	int PlayerRemain = pSceneManager->GetPlayerRemain();

	//残機が 1 未満の場合は「1」を表示**
	int displayRemain;

	// 0 以下の残機は表示しない
	if (PlayerRemain < REMAIN_MIN)
	{
		displayRemain = REMAIN_MIN;
	}
	else
	{
		displayRemain = PlayerRemain;
	}


	//スプライトの位置計算
	int spriteX = spriteWidth * (displayRemain - 1);
	int spriteY = 0;


	
	Image::SetTransform(hRemainImage_, transform_);
	Image::Draw(hRemainImage_);


	Image::SetRect(hNum_, spriteX, spriteY, spriteWidth, spriteHeight);
	Image::SetTransform(hNum_, trs);
	Image::Draw(hNum_);


	//アルファ値
	Image::SetAlpha(hRemainImage_, RemaindueAlpha_*255);
	Image::SetAlpha(hNum_, RemaindueAlpha_*255);

}

void Remain::Release()
{
}

void Remain::SetPosition(float x, float y, float z)
{
	transform_.position_ = { x, y, z };
	trs.position_ = { x + offsetX, y, z };
}

void Remain::SetAlpha(float alpha)
{
   RemaindueAlpha_ = alpha;
}
