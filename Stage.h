#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"
/// <summary>
/// ステージを作成するクラス
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

    int SelectMode;//上げるのか下げるのか変えるのか？
    int SelectType;//デフォルトなのか？草原なのか

    //void Save();
    //void Open();
public:
    //コンストラクタ
    Stage();
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

    //XMFLOAT3 GetPosition() { return transform_.position_; }
    //LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    //BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);}
};

