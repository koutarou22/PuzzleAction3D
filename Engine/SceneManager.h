#pragma once
#include "global.h"
#include "GameObject.h"


// ゲームに登場するシーン
enum SCENE_ID
{
    SCENE_ID_TITLE = 0,
    SCENE_ID_PLAY,
    SCENE_ID_LOAD,
    SCENE_ID_CLEAR,
    SCENE_ID_GAMEOVER
};

//-----------------------------------------------------------
// シーン切り替えを担当するオブジェクト
// 切り替えてもデータを保持できるようにする
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:
    // コンストラクタ
    SceneManager(GameObject* parent);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // シーン切り替え（次フレームで適用）
    void ChangeScene(SCENE_ID next);

    // **残機を管理する**---------------------------------------------

    // 残機数を取得
    int GetPlayerRemain() { return playerRemain_; }  

    //　残機を他クラスで共有可能にする　
    int GetStageNumber() { return StageClearCounter_; } 

    // 残機を他クラスで操作可能にする
    void SetPlayerRemain(int Remain);                
   

  

    // ステージクリアしたらカウント増加
    void NextStageCountPlus() { StageClearCounter_ += 1; };

	//　最大ステージ数を取得
    int GetMaxStageNumber() { return MAX_STAGE_NUMBER; }

	//　ステージを最初の状態にリセット(ステージ１)
    void ResetStageNumber(); 

    // 残機を初期化
	void ResetRemain();


    //残機の最大値
    const int MAX_REMAIN = 5;

    // プレイヤーの初期残機数
    const int PLAYER_INITIALIZE_REMAIN = 2;

	const int PLAYER_INITIALIZE_STAGE = 1; // プレイヤーの初期ステージ数

    // プレイヤーの初期ステージ数
	const int PLAYER_MAX_STAGE = 4;

    //-----------------------------------------------------------------

private:
    SCENE_ID currentSceneID_;  // 現在のシーン
    SCENE_ID nextSceneID_;     // 次のシーン

    // ステージクリアの確認　プレイヤーから情報を取得用
    int StageClearCounter_;

    // プレイヤーの残機数//消えてしまうのでSceneに持たせる
    int playerRemain_ = PLAYER_INITIALIZE_REMAIN; // デフォルト残機を 2 に設定
   
    const int MAX_STAGE_NUMBER = PLAYER_MAX_STAGE; //最大ステージ数
};
