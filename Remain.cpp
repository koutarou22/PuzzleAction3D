#include "Remain.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/SceneManager.h"


namespace
{
	const float ALPHA = 1.0f; // アルファ値の初期値
	const float offsetX = 0.2f; // 数字用のオフセットX値
	const int REMAIN_NUMBER_MIN = 1; // 残機の最小値

	//初期位置
	const float INITIALIZE_POS_X = -0.9f;  //画像の初期位置X座標
	const float INITIALIZE_POS_Y =  0.9f;  //画像の初期位置Y座標
	const float INITIALIZE_POS_Z =  0.0f;  //画像の初期位置Z座標


	//画像自体のサイズ
	const int IMAGE_SIZE_X = 1024 / 5;
	const int IMAGE_SIZE_Y = 821  / 3;

	//画像の切り抜き
	float IMAGE_CUT_X = IMAGE_SIZE_X; //切り取り幅
	float IMAGE_CUT_Y = IMAGE_SIZE_Y; //切り取り高さ

	//画像のスケールサイズ
	const float REMAIN_IMAGE_SCALE = 0.6;//画像の大きさの値
	const float NUMBER_IMAGE_SCALE = 0.3;//数字画像の大きさの値

	const XMFLOAT3 RemeinUI_Scale = { REMAIN_IMAGE_SCALE, REMAIN_IMAGE_SCALE, REMAIN_IMAGE_SCALE };// 残機UIの初期スケール
	const XMFLOAT3 Number_Scale   = { NUMBER_IMAGE_SCALE,NUMBER_IMAGE_SCALE,NUMBER_IMAGE_SCALE };  // 数字のスケールの初期値
}

Remain::Remain(GameObject* parent):GameObject(parent,"Remain")
,posX(INITIALIZE_POS_X),posY(INITIALIZE_POS_Y),posZ(INITIALIZE_POS_X)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	// SceneManager から残機情報を持ってくる
	if (pSceneManager != nullptr)
	{
		LIFE_ = pSceneManager->GetPlayerRemain();
	}
}

Remain::~Remain()
{}

void Remain::Initialize()
{

	ImagePath_ = "Image/";

	// 画像の名前
	ImageName_[REMAIN_UI_TYPE]     =  "RemainIcon.png"; //残機アイコンUI
	ImageName_[REMAIN_NUMBER_TYPE] =  "Number.png";     //数字UI

	//残機UIの透明度を操作する
	RemaindueAlpha_ = ALPHA;

	//登録
	hRemainImage_[REMAIN_UI_TYPE]     = Image::Load("Image/RemainIcon.png");  //残機アイコンUIを表示
	hRemainImage_[REMAIN_NUMBER_TYPE] = Image::Load("Image/Number.png");//数字UIを表示

	//間違っている、存在しなければエラーを返す
	assert(hRemainImage_[REMAIN_UI_TYPE] >= 0);
	assert(hRemainImage_[REMAIN_NUMBER_TYPE] >= 0);


	//数字UIの位置調整
	Number_transform_.position_.x = posX + offsetX;
	Number_transform_.position_.y = posY;

	//初期スケール
	transform_.scale_        = { RemeinUI_Scale};
	Number_transform_.scale_ = { Number_Scale };

	//初期位置
	transform_.position_ = { posX,posY,posZ };
}

void Remain::Update()
{}

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
	if (PlayerRemain < REMAIN_NUMBER_MIN)
	{
		displayRemain = REMAIN_NUMBER_MIN;
	}
	else
	{
		displayRemain = PlayerRemain;
	}

	//スプライトの位置計算
	int spriteX = spriteWidth * (displayRemain - 1);
	int spriteY = 0;
	
	//アイコンUIを表示
	Image::SetTransform(hRemainImage_[REMAIN_UI_TYPE], transform_);
	Image::Draw(hRemainImage_[REMAIN_UI_TYPE]);

	//数字UIを表示
	Image::SetRect(hRemainImage_[REMAIN_NUMBER_TYPE], spriteX, spriteY, spriteWidth, spriteHeight);
	Image::SetTransform(hRemainImage_[REMAIN_NUMBER_TYPE], Number_transform_);
	Image::Draw(hRemainImage_[REMAIN_NUMBER_TYPE]);


	//アルファ値 ロードシーンでUIも暗転(透明)させるため
	Image::SetAlpha(hRemainImage_[REMAIN_UI_TYPE]    , RemaindueAlpha_*255);
	Image::SetAlpha(hRemainImage_[REMAIN_NUMBER_TYPE], RemaindueAlpha_*255);
}

void Remain::Release()
{}

void Remain::SetPosition(float x, float y, float z)
{
	transform_.position_ = { x, y, z };
	Number_transform_.position_ = { x + offsetX, y, z };
}

void Remain::SetAlpha(float alpha)
{
   RemaindueAlpha_ = alpha;
}