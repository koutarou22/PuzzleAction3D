#include "TextImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Player.h"

namespace
{
	const XMFLOAT3  SCALE_SIZE = { 0.5,0.5,0.5 };//画像サイズ
	const XMFLOAT3  FAST_POSITION = { 0.0,0.85,0.0 };//画像の初期位置
}


TextImage::TextImage(GameObject* parent) :GameObject(parent, "TextImage"), GetChangeImageFlag(false), GetClearFlag(false)
{
}

TextImage::~TextImage()
{
}

void TextImage::Initialize()
{

	//画像の登録
	hTextImage_[0] = Image::Load("Image/Text/NoItem.png");//アイテムを取得していない時の画像
	hTextImage_[1] = Image::Load("Image/Text/GetItem.png");//アイテムを取得した時の画像
	hTextImage_[2] = Image::Load("Image/Text/StageClear1.png");//クリア条件を満たした時の画像

	//画像の読み込みに失敗したらエラーを出す
	assert(hTextImage_[0] >= 0);
	assert(hTextImage_[1] >= 0);
	assert(hTextImage_[2] >= 0);


	//画像の初期位置とサイズを設定
	transform_.scale_ = SCALE_SIZE;
	transform_.position_ = FAST_POSITION;
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
		GetChangeImageFlag = pPlayer->GetClearFlag();

		//クリア条件を満たしたかどうかのフラグを取得
		GetClearFlag = pPlayer->GetOpenGoalFlag();

		if (!GetChangeImageFlag)
		{
			Image::SetTransform(hTextImage_[0], transform_);
			Image::Draw(hTextImage_[0]);
		}
		else if (GetChangeImageFlag && GetClearFlag)
		{
			Image::SetTransform(hTextImage_[2], transform_);
			Image::Draw(hTextImage_[2]);
		}
		else
		{
			Image::SetTransform(hTextImage_[1], transform_);
			Image::Draw(hTextImage_[1]);
		}
	    
		
	}

}

void TextImage::Release()
{
}

