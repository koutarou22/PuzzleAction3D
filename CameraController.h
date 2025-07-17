#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum CAMERA_SE
{
	CAMERA_SE_ROTATE = 0, //�J�����̉�]��
	CAMERA_SE_MAX,
};
/// <summary>
/// �J�����𑀍삷��N���X
/// </summary>
class CameraController :public GameObject
{
	Transform transform_;
	XMVECTOR target_;
	XMVECTOR position_;

	float CamPosX, CamPosY, CamPosZ;

	int CamState_;

	int nextFace = 0; //���̖ʂ��i�[����ϐ�
	int currentFace = 0; //�J�����̌���
	float RotateProgress = 1.0f; //��ԏ��
	const float RotateSpeed = 0.05f; //��ԑ��x

	float targetRotationY = 0.0f; // �Ώۂ̉�]�p�x


	bool isCameraRotating_ = false;


	int hCameraSE_[CAMERA_SE_MAX]; // �J������SE�n���h��
	string CameraSEPath = "Sound//SE//CameraSE//";

public:

	CameraController(GameObject* parent);

	~CameraController();

	void Initialize();

	void Update();

	void Draw();

	void Release();

	void DefaultComera();

	void ResetCamera();

	XMMATRIX GetRotationMatrix() const;

	int GetCurrentFace() const;

	XMFLOAT3 Getroatate() { return transform_.rotate_; }

	XMVECTOR GetForwardVector() const;

	bool IsRotating() const { return isCameraRotating_; }

};

