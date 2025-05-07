#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"

#include<vector>

using std::vector;
// 3�����z��
  //     Z
  //     |
  //     |
  //     |___________Y
  //    /
  //   /
  //  X


/// <summary>
/// �X�e�[�W���쐬����N���X
/// </summary>
class Stage :public GameObject
{   
public:
    struct Data
    {
        int height;
        int type;
    };
private:

    int hStage_;//�X�e�[�W�̃u���b�N

    int hStageType_[4];//�u���b�N�̎��
 
    vector<vector<int>> Stagelayer;


    int AllLayer = 10;

    int Width;
    int Height;
   
    Data table[10][10];
   /* std::vector<std::vector<Deta>> table;*/

    int SelectMode;//�グ��̂�������̂��ς���̂��H
    int SelectType;//�f�t�H���g�Ȃ̂��H�����Ȃ̂�

    void SetBlockType(int BlockNum);

    //void Save();
    //void Open();

    bool shadowCreated = false;//�e�̃C���X�^���X����񂾂��Ăяo����悤�ɂ�����

public:

    Stage(GameObject* parent);

    ~Stage();

    void Initialize();

    void Update();

    void Draw();

    void Release();

    Data GetTableData(int x, int z) { return table[x][z]; }
    int GetWidth() { return Width; } 
    int GetHeight() { return Height; }

    //XMFLOAT3 GetPosition() { return transform_.position_; }
    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    //BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);}

    float GetGroundHeight(float x, float z);

    float GetBlockHeight(int x, int z) const { return table[x][z].height; }

   

    //�X�e�[�W�̃��f���𑼃N���X�ŋ��L����
    int GetStageModel() { return hStage_; }

    

    void PlayerRayHitStage();

    float GetRayHeight() const { return 0.0f; }

    /// <summary>
    /// ���C�̊J�n�ʒu
    /// </summary>
    /// <returns></returns>
    XMFLOAT3 GetRayStart() const
    {
        XMFLOAT3 rayStart = transform_.position_;
        rayStart.y -= GetRayHeight();
        return rayStart;
    }
};