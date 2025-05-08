#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"

#include<list>



#include<vector>

using std::list;
using std::vector;
// 3次元配列
  //     Z
  //     |
  //     |
  //     |___________Y
  //    /
  //   /
  //  X


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

    int hStage_;//ステージのブロック

    int hStageType_[4];//ブロックの種類
 
    vector<vector<int>> Stagelayer;


    int AllLayer = 10;

    int Width;
    int Height;
   
    Data table[10][10];
   /* std::vector<std::vector<Deta>> table;*/

    int SelectMode;//上げるのか下げるのか変えるのか？
    int SelectType;//デフォルトなのか？草原なのか

    void SetBlockType(int BlockNum);

    //void Save();
    //void Open();

    bool shadowCreated = false;//影のインスタンスを一回だけ呼び出せるようにしたい
    bool shadowEnemy = false;//後で消します
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

   

    //ステージのモデルを他クラスで共有する
    int GetStageModel() { return hStage_; }

    

    void PlayerRayHitStage();
    void EnemyRayHitStage();

    float GetRayHeight() const { return 0.0f; }

    /// <summary>
    /// レイの開始位置
    /// </summary>
    /// <returns></returns>
    XMFLOAT3 GetRayStart() const
    {
        XMFLOAT3 rayStart = transform_.position_;
        rayStart.y -= GetRayHeight();
        return rayStart;
    }


    /// <summary>
    /// オブジェクトの下にレイを飛ばし、地面(stageのオブジェクト)に接触したら影を出現させる
    /// </summary>
    /// <param name="name">レイキャストの宣言</param>
    /// <param name="RayStart">レイを飛ばす位置</param>
    /// <param name="dirX">レイを飛ばす方向</param>
    /// <param name="dirY">レイを飛ばす方向</param>
    /// <param name="dirZ">レイを飛ばす方向</param>
    /// <param name="CharacterPos">キャラクターの位置</param>
    void UnderShadowRayDate(RayCastData name ,XMFLOAT3 RayStart ,int dirX, int dirY ,int dirZ, XMFLOAT3 CharacterPos);

    RayCastData MoveEnemyRaydata;
    RayCastData BulletRaydata;

    list<XMFLOAT3> ShadowHitPosition;
};