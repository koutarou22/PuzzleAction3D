#include "Remain.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/SceneManager.h"


namespace
{
	const float ALPHA = 1.0f; // �A���t�@�l�̏����l
	const float offsetX = 0.2f; // �����p�̃I�t�Z�b�gX�l
	const int REMAIN_NUMBER_MIN = 1; // �c�@�̍ŏ��l

	//�����ʒu
	const float INITIALIZE_POS_X = -0.9f;  //�摜�̏����ʒuX���W
	const float INITIALIZE_POS_Y =  0.9f;  //�摜�̏����ʒuY���W
	const float INITIALIZE_POS_Z =  0.0f;  //�摜�̏����ʒuZ���W


	//�摜���̂̃T�C�Y
	const int IMAGE_SIZE_X = 1024 / 5;
	const int IMAGE_SIZE_Y = 821  / 3;

	//�摜�̐؂蔲��
	float IMAGE_CUT_X = IMAGE_SIZE_X; //�؂��蕝
	float IMAGE_CUT_Y = IMAGE_SIZE_Y; //�؂��荂��

	//�摜�̃X�P�[���T�C�Y
	const float REMAIN_IMAGE_SCALE = 0.6;//�摜�̑傫���̒l
	const float NUMBER_IMAGE_SCALE = 0.3;//�����摜�̑傫���̒l

	const XMFLOAT3 RemeinUI_Scale = { REMAIN_IMAGE_SCALE, REMAIN_IMAGE_SCALE, REMAIN_IMAGE_SCALE };// �c�@UI�̏����X�P�[��
	const XMFLOAT3 Number_Scale   = { NUMBER_IMAGE_SCALE,NUMBER_IMAGE_SCALE,NUMBER_IMAGE_SCALE };  // �����̃X�P�[���̏����l
}

Remain::Remain(GameObject* parent):GameObject(parent,"Remain")
,posX(INITIALIZE_POS_X),posY(INITIALIZE_POS_Y),posZ(INITIALIZE_POS_X)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	// SceneManager ����c�@���������Ă���
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

	// �摜�̖��O
	ImageName_[REMAIN_UI_TYPE]     =  "RemainIcon.png"; //�c�@�A�C�R��UI
	ImageName_[REMAIN_NUMBER_TYPE] =  "Number.png";     //����UI

	//�c�@UI�̓����x�𑀍삷��
	RemaindueAlpha_ = ALPHA;

	//�o�^
	hRemainImage_[REMAIN_UI_TYPE]     = Image::Load("Image/RemainIcon.png");  //�c�@�A�C�R��UI��\��
	hRemainImage_[REMAIN_NUMBER_TYPE] = Image::Load("Image/Number.png");//����UI��\��

	//�Ԉ���Ă���A���݂��Ȃ���΃G���[��Ԃ�
	assert(hRemainImage_[REMAIN_UI_TYPE] >= 0);
	assert(hRemainImage_[REMAIN_NUMBER_TYPE] >= 0);


	//����UI�̈ʒu����
	Number_transform_.position_.x = posX + offsetX;
	Number_transform_.position_.y = posY;

	//�����X�P�[��
	transform_.scale_        = { RemeinUI_Scale};
	Number_transform_.scale_ = { Number_Scale };

	//�����ʒu
	transform_.position_ = { posX,posY,posZ };
}

void Remain::Update()
{}

void Remain::Draw()
{

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	//�摜���w�肵�Đ؂���
	int spriteWidth = IMAGE_CUT_X;
	int spriteHeight = IMAGE_CUT_Y;

	//�c�@���擾
	int PlayerRemain = pSceneManager->GetPlayerRemain();

	//�c�@�� 1 �����̏ꍇ�́u1�v��\��**
	int displayRemain;

	// 0 �ȉ��̎c�@�͕\�����Ȃ�
	if (PlayerRemain < REMAIN_NUMBER_MIN)
	{
		displayRemain = REMAIN_NUMBER_MIN;
	}
	else
	{
		displayRemain = PlayerRemain;
	}

	//�X�v���C�g�̈ʒu�v�Z
	int spriteX = spriteWidth * (displayRemain - 1);
	int spriteY = 0;
	
	//�A�C�R��UI��\��
	Image::SetTransform(hRemainImage_[REMAIN_UI_TYPE], transform_);
	Image::Draw(hRemainImage_[REMAIN_UI_TYPE]);

	//����UI��\��
	Image::SetRect(hRemainImage_[REMAIN_NUMBER_TYPE], spriteX, spriteY, spriteWidth, spriteHeight);
	Image::SetTransform(hRemainImage_[REMAIN_NUMBER_TYPE], Number_transform_);
	Image::Draw(hRemainImage_[REMAIN_NUMBER_TYPE]);


	//�A���t�@�l ���[�h�V�[����UI���Ó](����)�����邽��
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