#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"
/// <summary>
/// �X�e�[�W���쐬����N���X
/// </summary>
class Stage :public GameObject
{
    int StageModel_;
 
    Fbx* pFbx[5];

 
    int Width;
    int Height;
    struct Data
    {
        int height;
        int type;
    };
    Data table[10][10];
   /* std::vector<std::vector<Deta>> table;*/

    int SelectMode;//�グ��̂�������̂��ς���̂��H
    int SelectType;//�f�t�H���g�Ȃ̂��H�����Ȃ̂�

    //void Save();
    //void Open();
public:
    //�R���X�g���N�^
    Stage();
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize();

    //�X�V
    void Update();

    //�`��
    void Draw();

    //�J��
    void Release();

    //XMFLOAT3 GetPosition() { return transform_.position_; }
    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    //BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);}
};

