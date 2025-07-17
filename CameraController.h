#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum CAMERA_SE
{
	CAMERA_SE_ROTATE = 0, //カメラの回転音
	CAMERA_SE_MAX,
};
/// <summary>
/// カメラを操作するクラス
/// </summary>
class CameraController :public GameObject
{
	Transform transform_;
	XMVECTOR target_;
	XMVECTOR position_;

	float CamPosX, CamPosY, CamPosZ;

	int CamState_;

	int nextFace = 0; //次の面を格納する変数
	int currentFace = 0; //カメラの向き
	float RotateProgress = 1.0f; //補間状態
	const float RotateSpeed = 0.05f; //補間速度

	float targetRotationY = 0.0f; // 対象の回転角度


	bool isCameraRotating_ = false;


	int hCameraSE_[CAMERA_SE_MAX]; // カメラのSEハンドル
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

