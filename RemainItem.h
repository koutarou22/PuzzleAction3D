#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// �c����A�b�v�����邽�߂̏���
/// </summary>
class RemainItem: public GameObject
{
    int hRemain_;

    //�����ʒu
    float posX, posY, posZ;

    //�ȒP�ȏc�h��̃A�j���[�V�����p�ϐ�

    float totalTime_;  //���v�̎��Ԃ��i�[����p
    float deltaTime;   //1�t���[���̎���(�b�Ŋ��Z�\��)

    float amplitude_;  // �U���̐U�ꕝ
    float frequency_;  // �h��̑��x
    float offsetY_;    //�ŏI�I�Ȍv�Z���i�[����p

public:

    RemainItem(GameObject* parent);

    ~RemainItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void RemainVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

