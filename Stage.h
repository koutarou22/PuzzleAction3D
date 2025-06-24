#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "NcsvReader.h"

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
	std::vector<std::vector<std::vector <int>>>& GetStageGrid() { return stageAlign_; }//stageAlign_を返す

private:
	int hBlock_;//ブロックのモデルを格納する用変数

	std::vector<std::vector<std::vector <int>>> stageAlign_;//ステージを整列する？

};

