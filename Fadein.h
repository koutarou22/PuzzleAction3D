#pragma once
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"
#include <string>

using std::string;


enum CHANGE_SCENE_SE
{
	CHANGE_SCENE_SE_START = 0, //開始音
	CHANGE_SCENE_SE_MAX,
};
//暗転する処理がまとまってるクラス
class Fadein : public GameObject
{
	int hBlackScreen_;   //簡単な暗転

	bool isFadingOut;    // フェードアウト中かどうか      
	bool isLoading;      // ロード中かどうか

	//サウンド
	int hStartSE_[CHANGE_SCENE_SE_MAX]; //押された時のSEハンドル
	string SceneSEPath = "Sound//SE//SceneSE//";
	bool IsPlaySE_;// SEが鳴っているかどうか
public:

	Fadein(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;

	/// <summary>
	/// 画面を暗転させる処理
	/// </summary>
	/// <param name="scene">変更したいシーン名</param>
	void FadeIn(SCENE_ID scene);

	//フェードアウトの状態を他クラスで操作
	void SetFadeOut(bool fadeout) { isFadingOut = fadeout; }
	bool GetFadeOut() const { return isFadingOut; }

	//簡単な押されたSEを鳴らす情報
	void PlayStartSound();

};

