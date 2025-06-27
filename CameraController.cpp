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
	const int FRAME_COOLDOWN = 60; // �؂�ւ����֎~����t���[���� 
}

enum CAMERA_TYPE
{
	DEFAULT_TYPE,
	//FPS_TYPE,
	OVERLOOK_TYPE,
	MAX_TYPE,
};

//Camera���l�̖ʂɕ�����
enum CAMERA_FACE
{
	FRONT = 0,
	RIGHT,
	BACK,
	LEFT,
};


CameraController::CameraController(GameObject* parent) :GameObject(parent, "CameraController")
{
	nextFace = 0;
}

CameraController::~CameraController()
{
	nextFace = 0;
	transform_.position_ = { 0,0,0 };
}

void CameraController::Initialize()
{
	targetRotationY = transform_.rotate_.y;
}

void CameraController::Update()
{
    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 RightStick = Input::GetPadStickR(0);

    int currentFace = GetCurrentFace();
    const int frameCooldown = 40;
    static int switchCooldownTimer = 0;
    static float targetRotationY = 0.0f;

    if (switchCooldownTimer > 0)
    {
        switchCooldownTimer--;
        isCameraRotating_ = true;
        if (pPlayer) pPlayer->SetMoveCamera(true);
    }
    else
    {
        isCameraRotating_ = false;
        if (pPlayer) pPlayer->SetMoveCamera(false);
    }

    if (switchCooldownTimer == 0 && pPlayer != nullptr)
    {
        int nextFace = currentFace;

        //�E��]
        if (Input::IsKey(DIK_RIGHT) || RightStick.x <= -0.3f)
        {
            nextFace = (currentFace - 1 + 4) % 4;
            switchCooldownTimer = frameCooldown;
        }
        //����]
        else if (Input::IsKey(DIK_LEFT) || RightStick.x >= 0.3f)
        {
            nextFace = (currentFace + 1) % 4;
            switchCooldownTimer = frameCooldown;
        }

        if (Input::IsKeyDown(DIK_K))
        {
            nextFace = 0;
        }

        if (nextFace != currentFace)
        {
            float diffRotation = XMConvertToRadians((nextFace - currentFace) * 90.0f);

           
            if (currentFace == 3 && nextFace == 0)
            {
                diffRotation = XMConvertToRadians(90.0f);
            }
            else if (currentFace == 0 && nextFace == 3)
            {
                diffRotation = XMConvertToRadians(-90.0f);
            }

            targetRotationY += diffRotation;
            currentFace = nextFace;
        }
    }

    transform_.rotate_.y += (targetRotationY - transform_.rotate_.y) * RotateSpeed;

    switch (CamState_)
    {
    case CAMERA_TYPE::DEFAULT_TYPE:
        transform_.position_ = { 0.0f, 10.0f, -18.0f };
        target_ = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
        break;

    default:
        break;
    }

    DefaultComera();

}

void CameraController::Draw()
{
	ImGui::Begin("Debug Camera");
	ImGui::Text("Current Face: %d", currentFace);
	ImGui::Text("Target Rotate: %5.2f", XMConvertToDegrees(targetRotationY));
	ImGui::Text("Current Rotate: %5.2f", XMConvertToDegrees(transform_.rotate_.y));
	ImGui::Text("Rotate Progress: %5.2f", RotateProgress);
	ImGui::End();

}

void CameraController::Release()
{
	targetRotationY = 0.0f; 
	transform_.rotate_.y = 0.0f;
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

void CameraController::CameraControl()
{
}

