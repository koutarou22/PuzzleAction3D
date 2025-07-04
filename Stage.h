#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "NcsvReader.h"

using std::vector;

enum BLOCK_TYPE
{
	EMPTY = 0,
	ENEMY_GHOST,
	ENEMY_TURRET,
	KEY,
	GOAL,
	STAGE_BLOCK,
	RESIDUE,
	PLAYER_BLOCK,

};

enum STAGE_NUMBER
{
	STAGE1 = 1,
	STAGE2,
	STAGE3,
	STAGE4,
	STAGE_MAX
};


class Stage
	:public GameObject
{
public:

	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Stage(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	int GetStageModel() { return hBlock_; }



	//ステージデータを他クラスに共有　GET関数
	std::vector<std::vector<std::vector <int>>>& GetStageGrid() { return stageAlign_; }

private:
	//ブロックのモデルを格納する用変数
	int hBlock_;      

	//ステージの高さ(Level)を調節する用変数
	float AdjustLevel_; 

	//ステージデータ(三次元配列)
	std::vector<std::vector<std::vector <int>>> stageAlign_;

	int ChangeStageNumber_;//ステージ番号

};

