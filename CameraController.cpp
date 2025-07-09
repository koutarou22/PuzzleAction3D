#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

using namespace Camera;

namespace
{
    // �J��������֘A�̒萔
    constexpr int CAMERA_FACE_COUNT = 4;
    constexpr int FRAME_COOLDOWN = 40;

    constexpr float STICK_THRESHOLD = 0.3f;
    constexpr float ROTATE_STEP_DEGREES = 90.0f;
    constexpr float ROTATION_BLEND_SPEED = 0.05f;

    // �����ʒu�֘A
    constexpr float DEFAULT_CAM_POS_Y = 10.0f;
    constexpr float DEFAULT_CAM_POS_Z = -18.0f;
    constexpr float DEFAULT_TARGET_Y = 2.0f;
}

enum CAMERA_TYPE
{
    DEFAULT_TYPE,
    //FPS_TYPE,
    OVERLOOK_TYPE,
    MAX_TYPE,
};

// Camera���l�̖ʂɕ�����
enum CAMERA_FACE
{
    FRONT = 0,
    RIGHT,
    BACK,
    LEFT,
};

CameraController::CameraController(GameObject* parent) : GameObject(parent, "CameraController")
{
    transform_.position_ = { 0.0f, 0.0f, 0.0f };
    transform_.rotate_.y = 0.0f;
    targetRotationY = 0.0f;
    currentFace = 0;
    nextFace = 0;
    CamState_ = CAMERA_TYPE::DEFAULT_TYPE;
    isCameraRotating_ = false;
    targetRotationY = transform_.rotate_.y;
    transform_.position_ = { 0, 0, 0 };
    transform_.rotate_.y = 0.0f;
    targetRotationY = 0.0f;
    nextFace = 0;
    currentFace = 0;

    DefaultComera();
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

    int currentFace = GetCurrentFace();
    static int switchCooldownTimer = 0;
    static float targetRotationY = 0.0f;

    if (switchCooldownTimer > 0)
    {
        switchCooldownTimer--;
        isCameraRotating_ = true;
    }
    else
    {
        isCameraRotating_ = false;
    }

    if (switchCooldownTimer == 0 && pPlayer != nullptr)
    {
        int nextFace = currentFace;

        // �E��]
        if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -STICK_THRESHOLD)
        {
            nextFace = (currentFace - 1 + CAMERA_FACE_COUNT) % CAMERA_FACE_COUNT;
            switchCooldownTimer = FRAME_COOLDOWN;
        }
        // ����]
        else if (Input::IsKey(DIK_LEFT) || RightStick.x >= STICK_THRESHOLD)
        {
            nextFace = (currentFace + 1) % CAMERA_FACE_COUNT;
            switchCooldownTimer = FRAME_COOLDOWN;
        }

        if (Input::IsKeyDown(DIK_K))
        {
            nextFace = 0;
        }

        if (nextFace != currentFace)
        {
            float diffRotation = XMConvertToRadians((nextFace - currentFace) * ROTATE_STEP_DEGREES);

            if (currentFace == 3 && nextFace == 0)
            {
                diffRotation = XMConvertToRadians(ROTATE_STEP_DEGREES);
            }
            else if (currentFace == 0 && nextFace == 3)
            {
                diffRotation = XMConvertToRadians(-ROTATE_STEP_DEGREES);
            }

            targetRotationY += diffRotation;
            currentFace = nextFace;
        }
    }

    transform_.rotate_.y += (targetRotationY - transform_.rotate_.y) * ROTATION_BLEND_SPEED;

    switch (CamState_)
    {
    case CAMERA_TYPE::DEFAULT_TYPE:
        transform_.position_ = { 0.0f, DEFAULT_CAM_POS_Y, DEFAULT_CAM_POS_Z };
        target_ = XMVectorSet(0.0f, DEFAULT_TARGET_Y, 0.0f, 0.0f);
        break;

    default:
        break;
    }

    DefaultComera();
}

void CameraController::Draw()
{

}

void CameraController::Release()
{
    targetRotationY = 0.0f;
    transform_.rotate_.y = 0.0f;
}

void CameraController::DefaultComera()
{
    // ���݂̈ʒu �� �J�n�n�_ + (�J�n�n�_ - �ڕW�n�_) * �i����
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

void CameraController::ResetCamera()
{
    // �ʒu�Ɖ�]�̏�����
    transform_.position_ = { 0.0f, 0.0f, 0.0f };
    transform_.rotate_.y = 0.0f;

    // �J�����̌����𐳖ʂ�
    targetRotationY = 0.0f;

    // �J������Ԃ��f�t�H���g��
    CamState_ = CAMERA_TYPE::DEFAULT_TYPE;

    currentFace = 0;
    nextFace = 0;

    isCameraRotating_ = false;

    target_ = XMVectorSet(0.0f, DEFAULT_TARGET_Y, 0.0f, 0.0f);
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
    return CAMERA_FACE::FRONT; // ����
}

XMVECTOR CameraController::GetForwardVector() const
{
    // �J�����̉�]�s����擾
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z�������i�J�����̑O�����j���擾
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

