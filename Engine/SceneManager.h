#pragma once
#include "global.h"
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY,
	SCENE_ID_LOAD,
	SCENE_ID_CLEAR,
	SCENE_ID_GAMEOVER
};

//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト,切り替えるとデータが削除されてしまうので、ここに一部データを保管できるようにする
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	void ChangeScene(SCENE_ID next);

	//他クラスでも値を確認できる用にGet関数をつくる
	int GetClearConfirm() { return StageClearCounter_; }

	//ステージをクリアしたらカウントを増やす
	void NextStageCountPlus() { StageClearCounter_ += 1; };

private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

	//ステージをクリアしたか確認する処理
	int StageClearCounter_;

	//他クラスでも値
	//使わない可能性はたかい
	void SetClearCount(int Clearcount) { StageClearCounter_ = Clearcount; }


};