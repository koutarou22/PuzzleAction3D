#pragma once
#include "Engine/GameObject.h"

//�U������l�̊Ǘ�
namespace
{
	//�A�j���[�V�����p�萔
	const float MOVE_AMPLITUDE = 0.1f;//�U�ꕝ
	const float MOVE_FREQUENCY_SPEED = 1.0f;// �h��̑����𒲐�
	const float DELTATIME = 1.0f / 60.0f;

	//�����p�x�̎w��
	const float OBJECT_ROTATE_X = 30;
	const float OBJECT_ROTATE_Y = 0.3f;

	//�I�u�W�F�N�g�̍ŏ��̈ʒu
	const float FAST_POSITION_X = 9.0f;
	const float FAST_POSITION_Y = 6.5f;
	const float FAST_POSITION_Z = 9.0f;

    //�I�u�W�F�N�g�̍ŏ��̃T�C�Y
    const float FAST_SCALE_X = 0.5f;
    const float FAST_SCALE_Y = 0.5f;
    const float FAST_SCALE_Z = 0.5f;

}
/// <summary>
/// �c����A�b�v�����邽�߂̏���
/// </summary>
class ResidueItem: public GameObject
{
    int hResidue_;

    //�����ʒu
    float posX, posY, posZ;

    //�ȒP�ȏc�h��̃A�j���[�V�����p�ϐ�

    float totalTime_;  //���v�̎��Ԃ��i�[����p
    float deltaTime;   //1�t���[���̎���(�b�Ŋ��Z�\��)

    float amplitude_;  // �U���̐U�ꕝ
    float frequency_;  // �h��̑��x
    float offsetY_;    //�ŏI�I�Ȍv�Z���i�[����p

public:

    ResidueItem(GameObject* parent);

    ~ResidueItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void ResidueVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

