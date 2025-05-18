#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// �v���C���[�̎c��N���X
/// </summary>
class Residue :
    public GameObject
{
    int hResidueImage_;
    int hNum_; //�c��y���l�z
    float posX1;//���l�̈ʒu
    float posX, posY, posZ;//�����ʒu

    int LIFE_;

    Transform trs;
public:
    Residue(GameObject* parent);

    ~Residue();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
    
    //�c�@����Ԃ�
    int GetLife() { return LIFE_; }

    //���N���X�ł����L������悤
    void SetLife(int LifeNum) { LifeNum = LIFE_; }
};

