#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

using namespace Camera;

CameraController::CameraController(GameObject* parent) :GameObject(parent, "CameraController")
{
	transform_.position_ = { 4.5f, 10.0f, -13.0f };
	target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);
}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
}

void CameraController::Update()
{

    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += 0.02f;
    }
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -= 0.02f;
    }

    //1.�ʒu���X�V����(����n��)
    XMVECTOR Position = XMLoadFloat3(&transform_.position_);

    //2.�ʒu�ƃ^�[�Q�b�g�̈ʒu���v�Z �����𗣂����̂ŃI�u�W�F�N�g�̍��W������
    XMVECTOR DistancePos = Position - target_;

    //3.Y���̉�]�s������
    XMMATRIX RotationMatrix = XMMatrixRotationY(transform_.rotate_.y);

    //4.�J�����̈ʒu����]������
    DistancePos = XMVector3TransformCoord(DistancePos, RotationMatrix);

    //5.��]��̃J�����̈ʒu���v�Z�@��������Ȃ��Ǝ��_���߂Â����藣�ꂽ��u���u���ɂȂ�
    XMVECTOR CamPos = DistancePos + target_;

    //6. XMVECTOR����XMFLOAT3�ɕϊ�����
    XMFLOAT3 SetPos;
    XMStoreFloat3(&SetPos, CamPos);
    XMFLOAT3 SetTag;
    XMStoreFloat3(&SetTag, target_);

    //7.�Ō�ɐݒ肷��
    Camera::SetPosition(SetPos);
    Camera::SetTarget(SetTag);
}

void CameraController::Draw()
{
}

void CameraController::Release()
{
}
