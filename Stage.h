#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "NcsvReader.h"

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
	std::vector<std::vector<std::vector <int>>>& GetStageGrid() { return stageAlign_; }//stageAlign_��Ԃ�

private:
	int hBlock_;//�u���b�N�̃��f�����i�[����p�ϐ�

	std::vector<std::vector<std::vector <int>>> stageAlign_;//�X�e�[�W�𐮗񂷂�H

};

