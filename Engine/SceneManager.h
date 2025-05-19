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
    void SetPlayerResidue(int residue)
    {
        // **残機が 1 未満になったら初期値にリセット**
        if (residue < 1)
        {
            residue = 2; // 初期値 (例: 2 に戻す)
        }

        // **残機が 5 を超えないよう制限**
        if (residue > 5)
        {
            residue = 5;
        }

        playerResidue_ = residue;
    }


    // 他クラスでも値を確認できる用に Get関数を作る
    int GetClearConfirm() { return StageClearCounter_; }

    // ステージクリアしたらカウント増加
    void NextStageCountPlus() { StageClearCounter_ += 1; };
 


private:
    SCENE_ID currentSceneID_;  // 現在のシーン
    SCENE_ID nextSceneID_;     // 次のシーン

    // ステージクリアの確認
    int StageClearCounter_;

    // プレイヤーの残機数//消えてしまうのでSceneに持たせる
    int playerResidue_ = 2; // デフォルト残機を 2 に設定

    // 他クラスでも値を設定
    void SetClearCount(int Clearcount) { StageClearCounter_ = Clearcount; }
};
