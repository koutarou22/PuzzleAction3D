#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

//���݂̈ʒu�@���@�J�n�n�_�@+  (�J�n�n�_-�ڕW�n�_) * �i����
using namespace Camera;

enum CAMERA_TYPE
{
    DEFAULT_TYPE,
    //FPS_TYPE,
    OVERLOOK_TYPE,
    MAX_TYPE,
};

//Camera���l�̖ʂƂ��čl����
enum CAMERA_FACE
{
    FRONT = 0,
    RIGHT,
    BACK,
    LEFT,
};

namespace
{
    float CAMERA_MOVE_SPEED = 0.02f;
}
CameraController::CameraController(GameObject* parent) :GameObject(parent, "CameraController")
{

}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
}

void CameraController::Update()
{

    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 RightStick = Input::GetPadStickR(0);

    // ���݂̃J�����ʁiFRONT, RIGHT, BACK, LEFT�j���擾
    int currentFace = GetCurrentFace();

    const int frameCooldown = 20; // �؂�ւ����b�N�̃t���[���� 
    static int switchCooldownTimer = 0; // �t���[���^�C�}�[�i�N�[���_�E���p�j 

    // �^�C�}�[�̌������� 
    if (switchCooldownTimer > 0) 
    { 
        switchCooldownTimer--; 
    }

    // �N�[���_�E���^�C�}�[���[���Ȃ�؂�ւ��\ 
    if (switchCooldownTimer == 0)
    {

        float t = 0.0f;
        if (pPlayer != nullptr)
        {

            float progres = transform_.rotate_.y + ((float)currentFace - currentFace * 90.0f) * t;
            // �J�����̉�]�����F�l�����ɃX�i�b�v
            if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -0.3f)
            {
              
                
                pPlayer->SetMoveCamera(true);


                currentFace = (currentFace + 1) % 4; // ���̖ʂɐ؂�ւ��i�E��]�j
                transform_.rotate_.y = XMConvertToRadians(currentFace * 90.0f); // 90�x���݂ɌŒ�
                switchCooldownTimer = frameCooldown; // �^�C�}�[�����Z�b�g


            }
            else
            {
                pPlayer->SetMoveCamera(false);
            }

            if (Input::IsKey(DIK_LEFT) || RightStick.x >= 0.3f)
            {
              //  transform_.rotate_.y -= CAMERA_MOVE_SPEED;
                pPlayer->SetMoveCamera(true);
                currentFace = (currentFace - 1 + 4) % 4; // �O�̖ʂɐ؂�ւ��i����]�j
                transform_.rotate_.y = XMConvertToRadians(currentFace * 90.0f); // 90�x���݂ɌŒ�
                switchCooldownTimer = frameCooldown; // �^�C�}�[�����Z�b�g


            }
            else
            {
                pPlayer->SetMoveCamera(false);
            }
        }

    

    }


    // �J�����̈ʒu�ݒ�
    switch (CamState_)
    {
    case CAMERA_TYPE::DEFAULT_TYPE:
        transform_.position_ = { 4.5f, 10.0f, -13.0f };
        target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);
        break;

    //case CAMERA_TYPE::FPS_TYPE:
    //    transform_.position_ = { 0.5f, 10.0f, -13.0f };
    //    target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);
    //    break;

    case CAMERA_TYPE::OVERLOOK_TYPE:
        transform_.position_ = { 4.5f, 18.0f, 4.0f };
        target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);
        break;

    default:
        break;
    }

    // �J���������X�V
    DefaultComera();

    // �J�����^�C�v�̐؂�ւ��iZ�L�[�j
    if (Input::IsKeyDown(DIK_Z))
    {
        CamState_++;
        if (CamState_ == CAMERA_TYPE::MAX_TYPE)
            CamState_ = CAMERA_TYPE::DEFAULT_TYPE;
    }
}


void CameraController::Draw()
{
}

void CameraController::Release()
{
}

void CameraController::DefaultComera()
{
    XMVECTOR Position = XMLoadFloat3(&transform_.position_);

    XMVECTOR DistancePos = Position - target_;

    XMMATRIX RotationMatrix = XMMatrixRotationY(transform_.rotate_.y);

    DistancePos = XMVector3TransformCoord(DistancePos, RotationMatrix);

    XMVECTOR CamPos = DistancePos + target_;

    XMFLOAT3 SetPos;
    XMStoreFloat3(&SetPos, CamPos);
    XMFLOAT3 SetTag;
    XMStoreFloat3(&SetTag, target_);

    Camera::SetPosition(SetPos);
    Camera::SetTarget(SetTag);
}

XMMATRIX CameraController::GetRotationMatrix() const
{
    return XMMatrixRotationY(transform_.rotate_.y); // Y����]
}


int CameraController::GetCurrentFace() const
{
    // �J������Y����]�p�x���擾
    float cameraYRotation = XMConvertToDegrees(transform_.rotate_.y);

    // �p�x��0~360�Ɏ��߂�
    //if (cameraYRotation < 0)
    //{
    //    cameraYRotation += 360.0f;
    //}

    // �ʂ𔻒�
    if (cameraYRotation >= 45.0f && cameraYRotation < 135.0f)
    {
        return CAMERA_FACE::RIGHT; // �E��
    }
    else if (cameraYRotation >= 135.0f && cameraYRotation < 225.0f)
    {
        return CAMERA_FACE::BACK; // �t��
    }
    else if (cameraYRotation >= 225.0f && cameraYRotation < 315.0f)
    {
        return CAMERA_FACE::LEFT; // ����
    }
    return CAMERA_FACE::FRONT; // ����
}

XMVECTOR CameraController::GetForwardVector() const
{
    // �J�����̉�]�s����擾
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z�������i�J�����̑O�����j���擾
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

