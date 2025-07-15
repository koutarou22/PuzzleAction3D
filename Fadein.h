#pragma once
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"

//暗転する処理がまとまってるクラス
class Fadein : public GameObject
{
	int hBlackScreen_;   //簡単な暗転

	bool isFadingOut;    // フェードアウト中かどうか      
	bool isLoading;      // ロード中かどうか
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

};

