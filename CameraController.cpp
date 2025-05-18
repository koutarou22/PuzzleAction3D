#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"


#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


//現在の位置　＝　開始地点　+  (開始地点-目標地点) * 進捗状況
using namespace Camera;

namespace
{
    int TIMER = 20;
    const int FRAME_COOLDOWN = 60; // 切り替えロックのフレーム数 
}

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
    currentFace = GetCurrentFace();

    static int switchCooldownTimer = 0; // フレームタイマー（クールダウン用） 


    // クールダウンタイマーがゼロなら切り替え可能 
    if (switchCooldownTimer == 0)
    {
        if (pPlayer != nullptr)
        {

            //右回転
            if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -0.3f)
            {


                pPlayer->SetMoveCamera(true);
                int nextFace = (currentFace - 1 + 4) % 4;

                // 目標回転角との差分を求める（現在の角度からスムーズに回転）
                float diffRotation = XMConvertToRadians((nextFace - currentFace) * 90.0f);

                // 例外処理: FRONT(0度) -> LEFT(270度) の場合、360度ジャンプではなく -90度へ
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

        //左回転
        if (Input::IsKey(DIK_LEFT) || RightStick.x >= 0.3f)
        {
            pPlayer->SetMoveCamera(true);
            int nextFace = (currentFace + 1) % 4;

            // 目標回転角との差分を求める
            float diffRotation = XMConvertToRadians((nextFace - currentFace) * 90.0f);

            // 特別処理: LEFT(270度) -> FRONT(0度) の場合、360度ジャンプではなく +90度へ
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

        //線形補間でスムーズに回転（ただし開始は即時！)
        RotateProgress += RotateSpeed;
        if (RotateProgress > 0.01f) 
        {
            RotateProgress = 0.01f;
        }
        //補間処理
        transform_.rotate_.y = (1.0f - RotateProgress) * transform_.rotate_.y + RotateProgress * targetRotationY;
    }



    // タイマーの減少処理 
    if (switchCooldownTimer > 0)
    {
        switchCooldownTimer--;
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
    return XMMatrixRotationY(transform_.rotate_.y); // Y軸回転
}


int CameraController::GetCurrentFace() const
{
    // カメラのY軸回転角度を取得
    float cameraYRotation = XMConvertToDegrees(transform_.rotate_.y);

    // 角度を 0～360 に収める（負の値も正しく処理）
    cameraYRotation = fmod(cameraYRotation + 360.0f, 360.0f);

    // 面を判定（境界値を正しく処理）
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
    return CAMERA_FACE::FRONT; // 正面（0～45度 & 315～360度）
}


XMVECTOR CameraController::GetForwardVector() const
{
    // カメラの回転行列を取得
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z軸方向（カメラの前方向）を取得
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

