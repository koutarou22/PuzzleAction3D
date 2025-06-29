#include "sceneManager.h"

#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include "../TitleScene.h"
#include "../PlayScene.h"
#include "../ClearScene.h"
#include "../GameOverScene.h"
#include "../LoadScene.h"


SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();

		//���[�h�����f�[�^��S�폜 
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID_LOAD: Instantiate<LoadScene>(this);break;
		case SCENE_ID_CLEAR: Instantiate<ClearScene>(this); break;
		case SCENE_ID_GAMEOVER: Instantiate<GameOverScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}

void SceneManager::SetPlayerResidue(int residue)
 {
        //�c�@�� 1 �����ɂȂ����珉���l�Ƀ��Z�b�g
        if (residue <= 0)
        {
            residue = residue;
        }

        if (residue > 5)
        {
            residue = 5;
        }

        playerResidue_ = residue;
    }
