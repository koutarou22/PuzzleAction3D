#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"
/// <summary>
/// ステージを作成するクラス
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

    int hStage_;
 
    std::vector<XMFLOAT3> StagePosList_;

    int Width;
    int Height;
   
    Data table[10][10];
   /* std::vector<std::vector<Deta>> table;*/

    int SelectMode;//上げるのか下げるのか変えるのか？
    int SelectType;//デフォルトなのか？草原なのか

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
};