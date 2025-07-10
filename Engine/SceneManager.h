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

    // **残機を管理する**
    int GetPlayerResidue() { return playerResidue_; }  // 残機数を取得
    void SetPlayerResidue(int residue);
   

    // 他クラスでも値を確認できる用に Get関数を作る
    int GetStageNumber() { return StageClearCounter_; }

    // ステージクリアしたらカウント増加
    void NextStageCountPlus() { StageClearCounter_ += 1; };

	//　最大ステージ数
    int GetMaxStageNumber() { return MAX_STAGE_NUMBER; }

	//　ステージを最初の状態にリセット(ステージ１)
    void ResetStageNumber(); 
    // 残機を初期化
	void ResetResidue();

    const int MAX_RESIDUE = 5;//残機の最大値

    // プレイヤーの初期残機数
    const int PLAYER_INITIALIZE_RESIDUE = 2;

private:
    SCENE_ID currentSceneID_;  // 現在のシーン
    SCENE_ID nextSceneID_;     // 次のシーン

    // ステージクリアの確認　プレイヤーから情報を取得用
    int StageClearCounter_ = 1;

    // プレイヤーの残機数//消えてしまうのでSceneに持たせる
    int playerResidue_ = PLAYER_INITIALIZE_RESIDUE; // デフォルト残機を 2 に設定
   
    const int MAX_STAGE_NUMBER = 4; //最大ステージ数
};
