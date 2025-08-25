#pragma once
#include "global.h"
#include "GameObject.h"


// �Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
    SCENE_ID_TITLE = 0,
    SCENE_ID_PLAY,
    SCENE_ID_LOAD,
    SCENE_ID_CLEAR,
    SCENE_ID_GAMEOVER
};

//-----------------------------------------------------------
// �V�[���؂�ւ���S������I�u�W�F�N�g
// �؂�ւ��Ă��f�[�^��ێ��ł���悤�ɂ���
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:
    // �R���X�g���N�^
    SceneManager(GameObject* parent);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �V�[���؂�ւ��i���t���[���œK�p�j
    void ChangeScene(SCENE_ID next);

    // **�c�@���Ǘ�����**---------------------------------------------

    // �c�@�����擾
    int GetPlayerRemain() { return playerRemain_; }  

    //�@�c�@�𑼃N���X�ŋ��L�\�ɂ���@
    int GetStageNumber() { return StageClearCounter_; } 

    // �c�@�𑼃N���X�ő���\�ɂ���
    void SetPlayerRemain(int Remain);                
   

  

    // �X�e�[�W�N���A������J�E���g����
    void NextStageCountPlus() { StageClearCounter_ += 1; };

	//�@�ő�X�e�[�W�����擾
    int GetMaxStageNumber() { return MAX_STAGE_NUMBER; }

	//�@�X�e�[�W���ŏ��̏�ԂɃ��Z�b�g(�X�e�[�W�P)
    void ResetStageNumber(); 

    // �c�@��������
	void ResetRemain();


    //�c�@�̍ő�l
    const int MAX_REMAIN = 5;

    // �v���C���[�̏����c�@��
    const int PLAYER_INITIALIZE_REMAIN = 2;

	const int PLAYER_INITIALIZE_STAGE = 1; // �v���C���[�̏����X�e�[�W��

    // �v���C���[�̏����X�e�[�W��
	const int PLAYER_MAX_STAGE = 4;

    //-----------------------------------------------------------------

private:
    SCENE_ID currentSceneID_;  // ���݂̃V�[��
    SCENE_ID nextSceneID_;     // ���̃V�[��

    // �X�e�[�W�N���A�̊m�F�@�v���C���[��������擾�p
    int StageClearCounter_;

    // �v���C���[�̎c�@��//�����Ă��܂��̂�Scene�Ɏ�������
    int playerRemain_ = PLAYER_INITIALIZE_REMAIN; // �f�t�H���g�c�@�� 2 �ɐݒ�
   
    const int MAX_STAGE_NUMBER = PLAYER_MAX_STAGE; //�ő�X�e�[�W��
};
