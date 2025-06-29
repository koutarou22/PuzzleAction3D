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

namespace
{
  const int PLAYER_INITIALIZE_RESIDUE = 2;
}
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

    // **�c�@���Ǘ�����**
    int GetPlayerResidue() { return playerResidue_; }  // �c�@�����擾
    void SetPlayerResidue(int residue);
   

    // ���N���X�ł��l���m�F�ł���p�� Get�֐������
    int GetClearConfirm() { return StageClearCounter_; }

    // �X�e�[�W�N���A������J�E���g����
    void NextStageCountPlus() { StageClearCounter_ += 1; };
 

    const int MAX_RESIDUE = 5;//�c�@�̍ő�l

private:
    SCENE_ID currentSceneID_;  // ���݂̃V�[��
    SCENE_ID nextSceneID_;     // ���̃V�[��

    // �X�e�[�W�N���A�̊m�F�@���݂͖��g�p�@
    int StageClearCounter_;

    // �v���C���[�̎c�@��//�����Ă��܂��̂�Scene�Ɏ�������
    int playerResidue_ = PLAYER_INITIALIZE_RESIDUE; // �f�t�H���g�c�@�� 2 �ɐݒ�

    // ���N���X�ł��l��ݒ�
    void SetClearCount(int Clearcount) { StageClearCounter_ = Clearcount; }
};
