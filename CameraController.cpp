#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

using namespace Camera;

enum CAMERA_TYPE
{
    DEFAULT_TYPE,
    TPS_TYPE,
    OVERLOOK_TYPE,
    MAX_TYPE,
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


    switch (CamState_)
    {
    case CAMERA_TYPE::DEFAULT_TYPE:

        transform_.position_ = { 4.5f, 10.0f, -13.0f };
        target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);

        if (Input::IsKey(DIK_RIGHT))
        {
            transform_.rotate_.y += CAMERA_MOVE_SPEED;
        }
        if (Input::IsKey(DIK_LEFT))
        {
            transform_.rotate_.y -= CAMERA_MOVE_SPEED;
        }
        break;

    case CAMERA_TYPE::TPS_TYPE:

        transform_.position_ = { 0.5f, 10.0f, -13.0f };
        target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);

        break;

    case CAMERA_TYPE::OVERLOOK_TYPE:

        transform_.position_ = { 4.5f, 20.0f, 0.0f };
        target_ = XMVectorSet(4.5f, 4.0f, 5.0f, 0.0f);

        if (Input::IsKey(DIK_RIGHT))
        {
            transform_.rotate_.y += CAMERA_MOVE_SPEED;
        }
        if (Input::IsKey(DIK_LEFT))
        {
            transform_.rotate_.y -= CAMERA_MOVE_SPEED;
        }
        break;

    default:
        break;
    }

    DefaultComera();

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
