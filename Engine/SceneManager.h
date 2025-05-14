#pragma once
#include "global.h"
#include "GameObject.h"

//�Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY,
	SCENE_ID_LOAD,
	SCENE_ID_CLEAR,
	SCENE_ID_GAMEOVER
};

//-----------------------------------------------------------
//�V�[���؂�ւ���S������I�u�W�F�N�g,�؂�ւ���ƃf�[�^���폜����Ă��܂��̂ŁA�����Ɉꕔ�f�[�^��ۊǂł���悤�ɂ���
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//�R���X�g���N�^
	//�����Fparent	�e�I�u�W�F�N�g�i��{�I�ɃQ�[���}�l�[�W���[�j
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
	//�����Fnext	���̃V�[����ID
	void ChangeScene(SCENE_ID next);

	//���N���X�ł��l���m�F�ł���p��Get�֐�������
	int GetClearConfirm() { return StageClearCounter_; }

	//�X�e�[�W���N���A������J�E���g�𑝂₷
	void NextStageCountPlus() { StageClearCounter_ += 1; };

private:
	SCENE_ID currentSceneID_;	//���݂̃V�[��
	SCENE_ID nextSceneID_;		//���̃V�[��

	//�X�e�[�W���N���A�������m�F���鏈��
	int StageClearCounter_;

	//���N���X�ł��l
	//�g��Ȃ��\���͂�����
	void SetClearCount(int Clearcount) { StageClearCounter_ = Clearcount; }


};