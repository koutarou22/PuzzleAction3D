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
	const XMFLOAT3  SCALE_SIZE = { 0.5,0.5,0.5 };//�摜�T�C�Y
	const XMFLOAT3  FAST_POSITION = { 0.0,0.85,0.0 };//�摜�̏����ʒu
}


TextImage::TextImage(GameObject* parent) :GameObject(parent, "TextImage"), GetChangeImageFlag(false), GetClearFlag(false)
{
}

TextImage::~TextImage()
{
}

void TextImage::Initialize()
{

	//�摜�̓o�^
	hTextImage_[0] = Image::Load("Image/Text/NoItem.png");//�A�C�e�����擾���Ă��Ȃ����̉摜
	hTextImage_[1] = Image::Load("Image/Text/GetItem.png");//�A�C�e�����擾�������̉摜
	hTextImage_[2] = Image::Load("Image/Text/StageClear1.png");//�N���A�����𖞂��������̉摜

	//�摜�̓ǂݍ��݂Ɏ��s������G���[���o��
	assert(hTextImage_[0] >= 0);
	assert(hTextImage_[1] >= 0);
	assert(hTextImage_[2] >= 0);


	//�摜�̏����ʒu�ƃT�C�Y��ݒ�
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
		//�A�C�e�����擾���Ă��邩�ǂ����̃t���O���擾
		GetChangeImageFlag = pPlayer->GetClearFlag();

		//�N���A�����𖞂��������ǂ����̃t���O���擾
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

