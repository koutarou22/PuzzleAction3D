#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

//現在の位置　＝　開始地点　+  (開始地点-目標地点) * 進捗状況
using namespace Camera;

enum CAMERA_TYPE
{
    DEFAULT_TYPE,
    //FPS_TYPE,
    OVERLOOK_TYPE,
    MAX_TYPE,
};

//Cameraを四つの面として考える
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

    // 現在のカメラ面（FRONT, RIGHT, BACK, LEFT）を取得
    int currentFace = GetCurrentFace();

    const int frameCooldown = 20; // 切り替えロックのフレーム数 
    static int switchCooldownTimer = 0; // フレームタイマー（クールダウン用） 

    // タイマーの減少処理 
    if (switchCooldownTimer > 0) 
    { 
        switchCooldownTimer--; 
    }

    // クールダウンタイマーがゼロなら切り替え可能 
    if (switchCooldownTimer == 0)
    {

        float t = 0.0f;
        if (pPlayer != nullptr)
        {

            float progres = transform_.rotate_.y + ((float)currentFace - currentFace * 90.0f) * t;
            // カメラの回転処理：四方向にスナップ
            if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -0.3f)
            {
              
                
                pPlayer->SetMoveCamera(true);


                currentFace = (currentFace + 1) % 4; // 次の面に切り替え（右回転）
                transform_.rotate_.y = XMConvertToRadians(currentFace * 90.0f); // 90度刻みに固定
                switchCooldownTimer = frameCooldown; // タイマーをリセット


            }
            else
            {
                pPlayer->SetMoveCamera(false);
            }

            if (Input::IsKey(DIK_LEFT) || RightStick.x >= 0.3f)
            {
              //  transform_.rotate_.y -= CAMERA_MOVE_SPEED;
                pPlayer->SetMoveCamera(true);
                currentFace = (currentFace - 1 + 4) % 4; // 前の面に切り替え（左回転）
                transform_.rotate_.y = XMConvertToRadians(currentFace * 90.0f); // 90度刻みに固定
                switchCooldownTimer = frameCooldown; // タイマーをリセット


            }
            else
            {
                pPlayer->SetMoveCamera(false);
            }
        }

    

    }


    // カメラの位置設定
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

    // カメラ情報を更新
    DefaultComera();

    // カメラタイプの切り替え（Zキー）
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
    return XMMatrixRotationY(transform_.rotate_.y); // Y軸回転
}


int CameraController::GetCurrentFace() const
{
    // カメラのY軸回転角度を取得
    float cameraYRotation = XMConvertToDegrees(transform_.rotate_.y);

    // 角度を0~360に収める
    //if (cameraYRotation < 0)
    //{
    //    cameraYRotation += 360.0f;
    //}

    // 面を判定
    if (cameraYRotation >= 45.0f && cameraYRotation < 135.0f)
    {
        return CAMERA_FACE::RIGHT; // 右面
    }
    else if (cameraYRotation >= 135.0f && cameraYRotation < 225.0f)
    {
        return CAMERA_FACE::BACK; // 逆面
    }
    else if (cameraYRotation >= 225.0f && cameraYRotation < 315.0f)
    {
        return CAMERA_FACE::LEFT; // 左面
    }
    return CAMERA_FACE::FRONT; // 正面
}

XMVECTOR CameraController::GetForwardVector() const
{
    // カメラの回転行列を取得
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z軸方向（カメラの前方向）を取得
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

