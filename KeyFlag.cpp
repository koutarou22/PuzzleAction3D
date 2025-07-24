#include "KeyFlag.h"
#include "Engine/Model.h"
#include "Engine/SceneManager.h"
#include"Engine/Collider.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Player.h"
#include "Engine/VFX.h"
#include "Engine/Debug.h"

//�U������l�E�����ʒu�̊Ǘ�
namespace 
{
	const float MOVE_AMPLITUDE = 0.1f;        //�U�ꕝ
	const float MOVE_FREQUENCY_SPEED = 1.0f;  // �h��̑����𒲐�
	const float DELTATIME = 1.0f / 60.0f;
		
	//�����p�x
	const float OBJECT_ROTATE_X = 30;
	const float OBJECT_ROTATE_Y = 0.3f;

	//�I�u�W�F�N�g�̍ŏ��̈ʒu
	const float FAST_POSITION_X = 9.0f;
	const float FAST_POSITION_Y = 6.5f;
	const float FAST_POSITION_Z = 9.0f;

	//�I�u�W�F�N�g�̍ŏ��̊p�x
	const float FAST_ROTATE_Y = 90.0f;


	//�����E�����瓪�̏�ɏ悹�Ă鍂��
	const float PLAYER_ABOVE_ON_THE_KEY = 1.85f;

	const int STAGE_OFFSET_X = 5;
	const int STAGE_OFFSET_Z = 4;

	XMFLOAT3 ColliderSize = { 0.3f, 0.3f, 0.3f }; // �R���C�_�[�̃T�C�Y
	
}



KeyFlag::KeyFlag(GameObject* parent) :GameObject(parent, "KeyFlag"), 
hKey_(-1),posX(FAST_POSITION_X),posY(FAST_POSITION_Y),posZ(FAST_POSITION_Z),
amplitude_(MOVE_AMPLITUDE),frequency_(MOVE_FREQUENCY_SPEED)
{
	isGetKey_ = false;
}

KeyFlag::~KeyFlag()
{
}

void KeyFlag::Initialize()
{
	hKey_ = Model::Load("Key.fbx");
	assert(hKey_ >= 0);

	transform_.rotate_.y = FAST_ROTATE_Y;

	transform_.position_ = { posX,posY,posZ };
	
	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { ColliderSize });
	AddCollider(collision);




}

void KeyFlag::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");
	KeyVibrationAnimation();

	if (isGetKey_)
	{
		XMFLOAT3 PlayerPosition = pPlayer->GetPosition();
		PlayerPosition.y += PLAYER_ABOVE_ON_THE_KEY;
		transform_.position_ = PlayerPosition;
	}
}

void KeyFlag::Draw()
{
	Model::SetTransform(hKey_, transform_);
	Model::Draw(hKey_);
}

void KeyFlag::Release()
{
}

void KeyFlag::OnCollision(GameObject* parent)
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (parent->GetObjectName() == "Player")
	{
		isGetKey_ = true;
	}
}

void KeyFlag::KeyVibrationAnimation()
{
	deltaTime  =  DELTATIME;     // 1�t���[���̎���
	totalTime_ += deltaTime;     // ���t���[������

	//sinf�֐�
	offsetY_ = sinf(totalTime_* frequency_) * amplitude_;

	transform_.rotate_.x =  OBJECT_ROTATE_X;  //X������]
	transform_.rotate_.y += OBJECT_ROTATE_Y;  //Y������]

	transform_.position_.y = posY + offsetY_;
}

void KeyFlag::SetBasePosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
	transform_.position_ = { x, y, z };
}
