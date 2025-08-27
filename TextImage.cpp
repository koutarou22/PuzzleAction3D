#include "TextImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Player.h"

namespace
{
	const float IMAGE_SIZE = 0.5;//�摜�̑傫���̒l

	const float INITIALIZE_POS_X = 0.0f; //�����ʒuX���W
	const float INITIALIZE_POS_Y = 0.85f;//�����ʒuY���W
	const float INITIALIZE_POS_Z = 0.0f; //�����ʒuZ���W

	const XMFLOAT3  ImageScale = { IMAGE_SIZE,IMAGE_SIZE,IMAGE_SIZE };                           //�摜�T�C�Y
	const XMFLOAT3  InitializePosition = { INITIALIZE_POS_X,INITIALIZE_POS_Y, INITIALIZE_POS_Z };//�摜�̏����ʒu	
}


TextImage::TextImage(GameObject* parent) :GameObject(parent, "TextImage"), GetChangeImageFlag_(false), GetClearFlag_(false)
{}

TextImage::~TextImage()
{}

void TextImage::Initialize()
{
	//�摜�̃p�X
	ImagePath_ = "Image/Text/";

	//�摜�̖��O
	ImageName_[NO_KEY_TEXT      ]  = "NoItem.png";     //�A�C�e�����擾���Ă��Ȃ����̉摜
	ImageName_[GET_KEY_TEXT     ]  = "GetItem.png";    //�A�C�e�����擾�������̉摜
	ImageName_[STAGE_CREAR_TEXT ]  = "StageClear.png";//�N���A�����𖞂��������̉摜

	//�摜�̓o�^
	hTextImage_[NO_KEY_TEXT     ]  = Image::Load (ImagePath_ + ImageName_[NO_KEY_TEXT]     );//�A�C�e�����擾���Ă��Ȃ����̉摜
	hTextImage_[GET_KEY_TEXT    ]  = Image::Load (ImagePath_ + ImageName_[GET_KEY_TEXT]    );//�A�C�e�����擾�������̉摜
	hTextImage_[STAGE_CREAR_TEXT]  = Image::Load (ImagePath_ + ImageName_[STAGE_CREAR_TEXT]);//�N���A�����𖞂��������̉摜

	//�摜�̓ǂݍ��݂Ɏ��s������G���[���o��
	assert(hTextImage_[NO_KEY_TEXT]      >= 0);
	assert(hTextImage_[GET_KEY_TEXT]     >= 0);
	assert(hTextImage_[STAGE_CREAR_TEXT] >= 0);


	//�摜�̏����ʒu�ƃT�C�Y��ݒ�
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
		//�A�C�e�����擾���Ă��邩�ǂ����̃t���O���擾
		GetChangeImageFlag_ = pPlayer->GetClearFlag();

		//�N���A�����𖞂��������ǂ����̃t���O���擾
		GetClearFlag_ = pPlayer->GetOpenGoalFlag();

		//�A�C�e�����擾���Ă��Ȃ���
		if (!GetChangeImageFlag_)
		{
			Image::SetTransform(hTextImage_[NO_KEY_TEXT], transform_);
			Image::Draw(hTextImage_[NO_KEY_TEXT]);
		}

		//�N���A�����𖞂����A�S�[���ɐڐG�����Ƃ�
		else if (GetChangeImageFlag_ && GetClearFlag_)
		{
			Image::SetTransform(hTextImage_[STAGE_CREAR_TEXT], transform_);
			Image::Draw(hTextImage_[STAGE_CREAR_TEXT]);
		}

		//�N���A�����𖞂����Ă��鎞
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

