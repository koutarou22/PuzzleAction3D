#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "NcsvReader.h"

using std::vector;

enum BLOCK_TYPE
{
	EMPTY = 0,
	ENEMY_GHOST,
	ENEMY_TURRET,
	KEY,
	GOAL,
	STAGE_BLOCK,
	RESIDUE,
	PLAYER_BLOCK,

};

enum STAGE_NUMBER
{
	STAGE1 = 1,
	STAGE2,
	STAGE3,
	STAGE4,
	STAGE_MAX
};


class Stage
	:public GameObject
{
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Stage(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	int GetStageModel() { return hBlock_; }



	//�X�e�[�W�f�[�^�𑼃N���X�ɋ��L�@GET�֐�
	std::vector<std::vector<std::vector <int>>>& GetStageGrid() { return stageAlign_; }

private:
	//�u���b�N�̃��f�����i�[����p�ϐ�
	int hBlock_;      

	//�X�e�[�W�̍���(Level)�𒲐߂���p�ϐ�
	float AdjustLevel_; 

	//�X�e�[�W�f�[�^(�O�����z��)
	std::vector<std::vector<std::vector <int>>> stageAlign_;

	int ChangeStageNumber_;//�X�e�[�W�ԍ�

};

