#include "TextImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Player.h"

namespace
{
	const float IMAGE_SIZE = 0.5;//画像の大きさの値

	const float INITIALIZE_POS_X = 0.0f; //初期位置X座標
	const float INITIALIZE_POS_Y = 0.85f;//初期位置Y座標
	const float INITIALIZE_POS_Z = 0.0f; //初期位置Z座標

	const XMFLOAT3  ImageScale = { IMAGE_SIZE,IMAGE_SIZE,IMAGE_SIZE };                           //画像サイズ
	const XMFLOAT3  InitializePosition = { INITIALIZE_POS_X,INITIALIZE_POS_Y, INITIALIZE_POS_Z };//画像の初期位置	
}


TextImage::TextImage(GameObject* parent) :GameObject(parent, "TextImage"), GetChangeImageFlag_(false), GetClearFlag_(false)
{}

TextImage::~TextImage()
{}

void TextImage::Initialize()
{
	//画像のパス
	ImagePath_ = "Image/Text/";

	//画像の名前
	ImageName_[NO_KEY_TEXT      ]  = "NoItem.png";     //アイテムを取得していない時の画像
	ImageName_[GET_KEY_TEXT     ]  = "GetItem.png";    //アイテムを取得した時の画像
	ImageName_[STAGE_CREAR_TEXT ]  = "StageClear.png";//クリア条件を満たした時の画像

	//画像の登録
	hTextImage_[NO_KEY_TEXT     ]  = Image::Load (ImagePath_ + ImageName_[NO_KEY_TEXT]     );//アイテムを取得していない時の画像
	hTextImage_[GET_KEY_TEXT    ]  = Image::Load (ImagePath_ + ImageName_[GET_KEY_TEXT]    );//アイテムを取得した時の画像
	hTextImage_[STAGE_CREAR_TEXT]  = Image::Load (ImagePath_ + ImageName_[STAGE_CREAR_TEXT]);//クリア条件を満たした時の画像

	//画像の読み込みに失敗したらエラーを出す
	assert(hTextImage_[NO_KEY_TEXT]      >= 0);
	assert(hTextImage_[GET_KEY_TEXT]     >= 0);
	assert(hTextImage_[STAGE_CREAR_TEXT] >= 0);


	//画像の初期位置とサイズを設定
	transform_.scale_    = ImageScale;
	transform_.position_ = InitializePosition;
}

void TextImage::Update()
{
}

void TextImage::Draw()
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
	{
		//アイテムを取得しているかどうかのフラグを取得
		GetChangeImageFlag_ = pPlayer->GetClearFlag();

		//クリア条件を満たしたかどうかのフラグを取得
		GetClearFlag_ = pPlayer->GetOpenGoalFlag();

		//アイテムを取得していない時
		if (!GetChangeImageFlag_)
		{
			Image::SetTransform(hTextImage_[NO_KEY_TEXT], transform_);
			Image::Draw(hTextImage_[NO_KEY_TEXT]);
		}

		//クリア条件を満たし、ゴールに接触したとき
		else if (GetChangeImageFlag_ && GetClearFlag_)
		{
			Image::SetTransform(hTextImage_[STAGE_CREAR_TEXT], transform_);
			Image::Draw(hTextImage_[STAGE_CREAR_TEXT]);
		}

		//クリア条件を満たしている時
		else
		{
			Image::SetTransform(hTextImage_[GET_KEY_TEXT], transform_);
			Image::Draw(hTextImage_[GET_KEY_TEXT]);
		}
	}
}

void TextImage::Release()
{
}

