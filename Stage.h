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
 
   // std::vector<XMFLOAT3> StagePosList_;

    vector<vector<int>> Stagelayer;
    /*CsvReader StageCsv;
    StageCsv.Load("");*/


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


    void PlayerRayHitStage();
};