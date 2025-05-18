#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"


#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


//���݂̈ʒu�@���@�J�n�n�_�@+  (�J�n�n�_-�ڕW�n�_) * �i����
using namespace Camera;

namespace
{
    int TIMER = 20;
    const int FRAME_COOLDOWN = 60; // �؂�ւ����b�N�̃t���[���� 
}

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
    currentFace = GetCurrentFace();

    static int switchCooldownTimer = 0; // �t���[���^�C�}�[�i�N�[���_�E���p�j 


    // �N�[���_�E���^�C�}�[���[���Ȃ�؂�ւ��\ 
    if (switchCooldownTimer == 0)
    {
        if (pPlayer != nullptr)
        {

            //�E��]
            if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -0.3f)
            {


                pPlayer->SetMoveCamera(true);
                int nextFace = (currentFace - 1 + 4) % 4;

                // �ڕW��]�p�Ƃ̍��������߂�i���݂̊p�x����X���[�Y�ɉ�]�j
                float diffRotation = XMConvertToRadians((nextFace - currentFace) * 90.0f);

                // ��O����: FRONT(0�x) -> LEFT(270�x) �̏ꍇ�A360�x�W�����v�ł͂Ȃ� -90�x��
                if (currentFace == CAMERA_FACE::FRONT && nextFace == CAMERA_FACE::LEFT)
                {
                    diffRotation = XMConvertToRadians(-90.0f);
                }

                targetRotationY += diffRotation; 
                currentFace = nextFace;
                switchCooldownTimer = FRAME_COOLDOWN;
               // RotateProgress = 0.0f;
            }
        }
        else
        {
            pPlayer->SetMoveCamera(false);
        }

        //����]
        if (Input::IsKey(DIK_LEFT) || RightStick.x >= 0.3f)
        {
            pPlayer->SetMoveCamera(true);
            int nextFace = (currentFace + 1) % 4;

            // �ڕW��]�p�Ƃ̍��������߂�
            float diffRotation = XMConvertToRadians((nextFace - currentFace) * 90.0f);

            // ���ʏ���: LEFT(270�x) -> FRONT(0�x) �̏ꍇ�A360�x�W�����v�ł͂Ȃ� +90�x��
            if (currentFace == CAMERA_FACE::LEFT && nextFace == CAMERA_FACE::FRONT)
            {
                diffRotation = XMConvertToRadians(90.0f);
            }

            targetRotationY += diffRotation; 
            currentFace = nextFace;
            switchCooldownTimer = FRAME_COOLDOWN;
           // RotateProgress = 0.0f;
        }
        else
        {
            pPlayer->SetMoveCamera(false);
        }

        //���`��ԂŃX���[�Y�ɉ�]�i�������J�n�͑����I)
        RotateProgress += RotateSpeed;
        if (RotateProgress > 0.01f) 
        {
            RotateProgress = 0.01f;
        }
        //��ԏ���
        transform_.rotate_.y = (1.0f - RotateProgress) * transform_.rotate_.y + RotateProgress * targetRotationY;
    }



    // �^�C�}�[�̌������� 
    if (switchCooldownTimer > 0)
    {
        switchCooldownTimer--;
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
    ImGui::Begin("Debug Window");
    ImGui::Text("Current Face: %d", currentFace);
    ImGui::Text("Target Rotate: %5.2f", XMConvertToDegrees(targetRotationY));
    ImGui::Text("Current Rotate: %5.2f", XMConvertToDegrees(transform_.rotate_.y));
    ImGui::Text("Rotate Progress: %5.2f", RotateProgress);
    ImGui::End();

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

    // �p�x�� 0�`360 �Ɏ��߂�i���̒l�������������j
    cameraYRotation = fmod(cameraYRotation + 360.0f, 360.0f);

    // �ʂ𔻒�i���E�l�𐳂��������j
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
    return CAMERA_FACE::FRONT; // ���ʁi0�`45�x & 315�`360�x�j
}


XMVECTOR CameraController::GetForwardVector() const
{
    // �J�����̉�]�s����擾
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z�������i�J�����̑O�����j���擾
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

