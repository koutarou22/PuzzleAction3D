#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "NcsvReader.h"

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

	float GetGroundHeight(float x, float z);


private:
	//�u���b�N�̃��f�����i�[����p�ϐ�
	int hBlock_;      

	//�v���C���[�u���b�N���i�[����p�ϐ�
	int hPlayerBlock_; 

	//�X�e�[�W�̍���(Level)�𒲐߂���p�ϐ�
	float AdjustLevel_; 

	//�X�e�[�W�f�[�^(�O�����z��)
	std::vector<std::vector<std::vector <int>>> stageAlign_;

};

