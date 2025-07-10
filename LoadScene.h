#pragma once
#include "Engine/GameObject.h"


enum LOAD_SCENE_STAGE_NUMBER
{
	LOAD_SCENE_STAGE1 = 1,
	LOAD_SCENE_STAGE2,
	LOAD_SCENE_STAGE3,
	LOAD_SCENE_STAGE4,
	LOAD_SCENE_MAX,
};


class LoadScene :public GameObject
{
	int hImage_;
	int hBlackScreen_;//�ȒP�ȈÓ]

	int StageNumber_;//�����Ă����X�e�[�W�ԍ��擾�p�ϐ�

	int StageImage_[LOAD_SCENE_MAX];//���ƂŐ��l�͂������悤�ɂ��鎖

	int StageNumberCurrent_;//StaggeImage_���i�[

	int hStartSound_; //�J�n�������ꂽ��SE��炷

	int TimeFlame_ = 60;//���ԍ���Scene���ړ�������

	bool isNextScene = false;//���̃V�[���Ɉړ����邩�ǂ����̃t���O
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;


};

