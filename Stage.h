#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/Transform.h"

#include<list>
#include<vector>

using std::list;
using std::vector;

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

    const int Width;
    const int Height;
   
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
   
    float GetGroundHeight(float x, float z);

    float GetBlockHeight(int x, int z) const { return table[x][z].height; }

   

    //ステージのモデルを他クラスで共有する
    int GetStageModel() { return hStage_; }

    

    void PlayerRayHitStage();
    void MoveEnemyRayHitStage();

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



    RayCastData MoveEnemyRaydata;
    RayCastData BulletRaydata;

};