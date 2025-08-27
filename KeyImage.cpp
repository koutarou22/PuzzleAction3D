#include "KeyImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Player.h"

namespace
{
    const float IMAGE_SCALE = 0.6f;
    float INITIAL_POS_X     = 2.0f;
    float INITIAL_POS_Y     = 0.8f;
    float INITIAL_POS_Z     = 0.0f;

	const float OFFSET_X    = 0.2f;
}

KeyImage::KeyImage(GameObject* parent)
    : GameObject(parent, "KeyImage"),
    posX_(INITIAL_POS_X), posY_(INITIAL_POS_Y), posZ_(INITIAL_POS_Z)
{
}

KeyImage::~KeyImage()
{
}

void KeyImage::Initialize()
{
	//�摜�̃p�X�Ɩ��O
	ImagePath_ = "Image/";
	ImageName_[KEY_IMAGE_NO_KEY] = "Image_Key.png";        //���������Ă��Ȃ����
	ImageName_[KEY_IMAGE_KEY]    = "Image_GetKey.png";     //���������Ă�����

	//�摜�̓o�^
    hKeyImage_[KEY_IMAGE_NO_KEY] = Image::Load(ImagePath_ + ImageName_[KEY_IMAGE_NO_KEY]);
    hKeyImage_[KEY_IMAGE_KEY]    = Image::Load(ImagePath_ + ImageName_[KEY_IMAGE_KEY])   ;

	//�o�^����Ă��邩�m�F
    assert(hKeyImage_[KEY_IMAGE_NO_KEY] >= 0);
	assert(hKeyImage_[KEY_IMAGE_KEY] >= 0);

	//�����ʒu�ƃT�C�Y��ݒ�
    transform_.scale_ = { IMAGE_SCALE, IMAGE_SCALE, IMAGE_SCALE };
    transform_.position_ = { posX_, posY_, posZ_ };
}

void KeyImage::Update()
{
}

void KeyImage::Draw()
{
	Player* pPlayer = (Player*)FindObject("Player");

	//���������Ă��邩�ǂ����̃t���O���󂯎��
    bool IsKey = pPlayer->GetClearFlag();

	// �擾���Ă��� or �擾���Ă��Ȃ��ŉ摜��؂�ւ���
    if (IsKey)
    {
		Image::Draw(hKeyImage_[KEY_IMAGE_KEY]);
        Image::SetTransform(hKeyImage_[KEY_IMAGE_KEY],transform_);
		assert(hKeyImage_[KEY_IMAGE_KEY] >= 0);
    }
    else
    {
		Image::Draw(hKeyImage_[KEY_IMAGE_NO_KEY]);
        Image::SetTransform(hKeyImage_[KEY_IMAGE_NO_KEY], transform_);
		assert(hKeyImage_[KEY_IMAGE_NO_KEY] >= 0);
    }
}

void KeyImage::Release()
{}
