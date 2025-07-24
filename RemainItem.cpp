#include "RemainItem.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//�U������l�̊Ǘ�
namespace
{
	//�A�j���[�V�����p�萔
	const float MOVE_AMPLITUDE = 0.1f;//�U�ꕝ
	const float MOVE_FREQUENCY_SPEED = 1.0f;// �h��̑����𒲐�
	const float DELTATIME = 1.0f / 60.0f;

	//�����p�x�̎w��
	const float OBJECT_ROTATE_X = 30;
	const float OBJECT_ROTATE_Y = 0.3f;

	//�I�u�W�F�N�g�̍ŏ��̈ʒu
	const float FAST_POSITION_X = 9.0f;
	const float FAST_POSITION_Y = 6.5f;
	const float FAST_POSITION_Z = 9.0f;

	//�I�u�W�F�N�g�̍ŏ��̃T�C�Y
	const float FAST_SCALE_X = 0.5f;
	const float FAST_SCALE_Y = 0.5f;
	const float FAST_SCALE_Z = 0.5f;

	const float FREQUENCY = 0.5f; // �U���̎��g��
	const float AMPLITUDE = 0.2f; // �U���̐U��

	XMFLOAT3 ColliderSize = { 0.3f, 0.3f, 0.3f }; // �R���C�_�[�̃T�C�Y

}

RemainItem::RemainItem(GameObject* parent) :GameObject(parent, "RemainItem"), hRemain_(-1),
posX(FAST_POSITION_X), posY(FAST_POSITION_Y), posZ(FAST_POSITION_Z)
{
	frequency_ = FREQUENCY;
	amplitude_ = AMPLITUDE;
}

RemainItem::~RemainItem()
{
}

void RemainItem::Initialize()
{

	hRemain_ = Model::Load("RubyPosZero.fbx");
	assert(hRemain_ >= 0);

	transform_.position_ = { posX,posY,posZ };
	transform_.scale_ = { FAST_SCALE_X,FAST_SCALE_Y,FAST_SCALE_Z };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { ColliderSize });
	AddCollider(collision);
}

void RemainItem::Update()
{
	RemainVibrationAnimation();
}

void RemainItem::Draw()
{
	Model::SetTransform(hRemain_, transform_);
	Model::Draw(hRemain_);
}

void RemainItem::Release()
{
}

void RemainItem::OnCollision(GameObject* parent)
{
	if (parent->GetObjectName() == "Player")
	{
		KillMe();
	}
}

void RemainItem::RemainVibrationAnimation()
{
	deltaTime  =  DELTATIME;   // 1�t���[���̎��ԁi�b���Z�j
	totalTime_ += deltaTime;   // ���t���[������

	offsetY_ = sin(totalTime_ * frequency_) * amplitude_;

	transform_.rotate_.y += OBJECT_ROTATE_Y;  
	transform_.position_.y = posY + offsetY_;
}

void RemainItem::SetBasePosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	transform_.position_ = { x, y, z };
}
