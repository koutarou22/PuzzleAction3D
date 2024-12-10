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

    //1.位置を更新する(情報を渡す)
    XMVECTOR Position = XMLoadFloat3(&transform_.position_);

    //2.位置とターゲットの位置を計算 距離を離しいのでオブジェクトの座標を引く
    XMVECTOR DistancePos = Position - target_;

    //3.Y軸の回転行列を作る
    XMMATRIX RotationMatrix = XMMatrixRotationY(transform_.rotate_.y);

    //4.カメラの位置を回転させる
    DistancePos = XMVector3TransformCoord(DistancePos, RotationMatrix);

    //5.回転後のカメラの位置を計算　これをしないと視点が近づいたり離れたりブレブレになる
    XMVECTOR CamPos = DistancePos + target_;

    //6. XMVECTORからXMFLOAT3に変換する
    XMFLOAT3 SetPos;
    XMStoreFloat3(&SetPos, CamPos);
    XMFLOAT3 SetTag;
    XMStoreFloat3(&SetTag, target_);

    //7.最後に設定する
    Camera::SetPosition(SetPos);
    Camera::SetTarget(SetTag);
}

void CameraController::Draw()
{
}

void CameraController::Release()
{
}
