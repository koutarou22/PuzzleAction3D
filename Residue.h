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

    Transform trs;
public:
    Residue(GameObject* parent);

    ~Residue();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

