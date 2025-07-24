#include "Remain.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/SceneManager.h"


namespace
{
	XMFLOAT3 SCALE = { 0.6f, 0.6f, 0.6f };// �X�P�[���̏����l
	XMFLOAT3 NUM_SCALE = { 0.3,0.3,0.3 };// �����̃X�P�[���̏����l

	float ALPHA = 1.0f; // �A���t�@�l�̏����l

	float offsetX = 0.2f; // �����̃I�t�Z�b�gX�l

	float IMAGE_CUT_X = 1024 / 5;// �摜�̐؂��蕝
	float IMAGE_CUT_Y = 821 / 3; // �摜�̐؂��荂��

	const int REMAIN_MIN = 1; // �c�@�̍ŏ��l
}

Remain::Remain(GameObject* parent):GameObject(parent,"Remain")
,posX(-0.9),posY(0.9),posZ(0.0)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		// SceneManager ����c�@�������Ă���
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

	//�摜���w�肵�Đ؂���
	int spriteWidth = IMAGE_CUT_X;
	int spriteHeight = IMAGE_CUT_Y;

	//�c�@���擾
	int PlayerRemain = pSceneManager->GetPlayerRemain();

	//�c�@�� 1 �����̏ꍇ�́u1�v��\��**
	int displayRemain;

	// 0 �ȉ��̎c�@�͕\�����Ȃ�
	if (PlayerRemain < REMAIN_MIN)
	{
		displayRemain = REMAIN_MIN;
	}
	else
	{
		displayRemain = PlayerRemain;
	}


	//�X�v���C�g�̈ʒu�v�Z
	int spriteX = spriteWidth * (displayRemain - 1);
	int spriteY = 0;


	
	Image::SetTransform(hRemainImage_, transform_);
	Image::Draw(hRemainImage_);


	Image::SetRect(hNum_, spriteX, spriteY, spriteWidth, spriteHeight);
	Image::SetTransform(hNum_, trs);
	Image::Draw(hNum_);


	//�A���t�@�l
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
