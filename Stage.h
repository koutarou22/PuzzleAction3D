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
    //Fbx* pFbx[5];
    int hStage_[5];
 
    int Width;
    int Height;
   
    Data table[10][10];
   /* std::vector<std::vector<Deta>> table;*/

    int SelectMode;//上げるのか下げるのか変えるのか？
    int SelectType;//デフォルトなのか？草原なのか

    //void Save();
    //void Open();
public:



    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize();

    //更新
    void Update();

    //描画
    void Draw();

    //開放
    void Release();

    int* GetModelHandle() { return hStage_; } 
    Data GetTableData(int x, int z) { return table[x][z]; }
    int GetWidth() { return Width; } 
    int GetHeight() { return Height; }

    //XMFLOAT3 GetPosition() { return transform_.position_; }
    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    //BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);}
};