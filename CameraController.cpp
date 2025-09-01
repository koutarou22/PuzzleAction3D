#include "CameraController.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/Audio.h"

using namespace Camera;

namespace
{
    // カメラ動作関連の定数
	constexpr int CAMERA_FACE_COUNT = 4;// カメラの面の数
	constexpr int FRAME_COOLDOWN = 60;// カメラの次の回転までのクールダウン

	constexpr float STICK_THRESHOLD = 0.3f;// スティックの入力範囲
	constexpr float ROTATE_STEP_DEGREES = 90.0f; // カメラの回転角度のステップ
	constexpr float ROTATION_BLEND_SPEED = 0.05f; // カメラの回転のブレンド速度

    // 初期位置関連
    constexpr float DEFAULT_CAM_POS_Y = 10.0f;
    constexpr float DEFAULT_CAM_POS_Z = -18.0f;
    constexpr float DEFAULT_TARGET_Y = 2.0f;
}

enum CAMERA_TYPE
{
    DEFAULT_TYPE,
    MAX_TYPE,
};

// Cameraを四つの面に分ける
enum CAMERA_FACE
{
    FRONT = 0,
    RIGHT,
    BACK,
    LEFT,
};

// カメラの回転角度の値を定義
namespace CameraRotationThreshold
{
	const float RIGHT_MIN = 45.0f;// 右面の最小角度
    const float RIGHT_MAX = 135.0f;//右面の最大角度
    const float BACK_MIN = 135.0f;// 逆面の最小角度
    const float BACK_MAX = 225.0f;// 逆面の最大角度
    const float LEFT_MIN = 225.0f;// 左面の最小角度
    const float LEFT_MAX = 315.0f;// 左面の最大角度
	const  float FULL_ROTATION_DEGREES = 360.0f;// 360度の全回転
}


CameraController::CameraController(GameObject* parent) : GameObject(parent, "CameraController")
{

    transform_.position_ = { 0.0f, 0.0f, 0.0f };
    transform_.rotate_.y = 0.0f;
    targetRotationY = 0.0f;
    currentFace = 0;
    nextFace = 0;
    CamState_ = CAMERA_TYPE::DEFAULT_TYPE;
    isCameraRotating_ = false;

    DefaultComera();
}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
    ResetCamera();
	hCameraSE_[CAMERA_SE::CAMERA_SE_ROTATE] = Audio::Load("Sound//SE//CameraSE//CameraScroll.wav");
	assert(hCameraSE_[CAMERA_SE::CAMERA_SE_ROTATE] >= 0);

}

void CameraController::Update()
{
    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 RightStick = Input::GetPadStickR(0);

    int currentFace = GetCurrentFace();
    static int switchCooldownTimer = 0;

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

        // 右回転
        if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -STICK_THRESHOLD)
        {
            Audio::Play(hCameraSE_[CAMERA_SE::CAMERA_SE_ROTATE]);

            nextFace = (currentFace - 1 + CAMERA_FACE_COUNT) % CAMERA_FACE_COUNT;
            switchCooldownTimer = FRAME_COOLDOWN;
		
        }
        // 左回転
        else if (Input::IsKey(DIK_LEFT) || RightStick.x >= STICK_THRESHOLD)
        {
            Audio::Play(hCameraSE_[CAMERA_SE::CAMERA_SE_ROTATE]);

            nextFace = (currentFace + 1) % CAMERA_FACE_COUNT;
            switchCooldownTimer = FRAME_COOLDOWN;

        }


        //特定のキーを押したら初期位置に戻す　
        if (Input::IsKeyDown(DIK_K))
        {
            nextFace = 0;
        }

        //カメラの回転を更新する処理
        if (nextFace != currentFace)
        {
            float diffRotation = XMConvertToRadians((nextFace - currentFace) * ROTATE_STEP_DEGREES);

			//カメラの回転が360度を超えた場合の補正
            if (currentFace == 3 && nextFace == 0)
            {
                diffRotation = XMConvertToRadians(ROTATE_STEP_DEGREES);
            }
            else if (currentFace == 0 && nextFace == 3)
            {
                diffRotation = XMConvertToRadians(-ROTATE_STEP_DEGREES);
            }

			//カメラの目標回転角度を更新
            targetRotationY += diffRotation;

			// 現在の面を更新
            currentFace = nextFace;
        }
    }

	// カメラの回転を滑らかに補間する処理
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
    // 現在の位置 ＝ 開始地点 + (開始地点 - 目標地点) * 進捗状況
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
    // 位置と回転の初期化
    transform_.position_ = { 0.0f, 0.0f, 0.0f };
    transform_.rotate_.y = 0.0f;

    // カメラの向きを正面に
    targetRotationY = 0.0f;

    // カメラ状態をデフォルトに
    CamState_ = CAMERA_TYPE::DEFAULT_TYPE;

    currentFace = 0;
    nextFace = 0;

    isCameraRotating_ = false;

    target_ = XMVectorSet(0.0f, DEFAULT_TARGET_Y, 0.0f, 0.0f);
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
    cameraYRotation = fmod(cameraYRotation + CameraRotationThreshold::FULL_ROTATION_DEGREES, CameraRotationThreshold::FULL_ROTATION_DEGREES);

	// 右面、逆面、左面、正面のいずれかを判定
    if (cameraYRotation >= CameraRotationThreshold::RIGHT_MIN && cameraYRotation < CameraRotationThreshold::RIGHT_MAX)
    {
		return CAMERA_FACE::RIGHT;// 右面
    }
    else if (cameraYRotation >= CameraRotationThreshold::BACK_MIN &&cameraYRotation < CameraRotationThreshold::BACK_MAX)
    {
		return CAMERA_FACE::BACK;// 逆面
    }
    else if (cameraYRotation >= CameraRotationThreshold::LEFT_MIN && cameraYRotation < CameraRotationThreshold::LEFT_MAX)
    {
		return CAMERA_FACE::LEFT;// 左面
    }
	return CAMERA_FACE::FRONT;// 正面

}

XMVECTOR CameraController::GetForwardVector() const
{
    // カメラの回転行列を取得
    XMMATRIX rotationMatrix = GetRotationMatrix();

    // Z軸方向（カメラの前方向）を取得
    return XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
}

