#include "TimerImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Player.h"

TimerImage::TimerImage(GameObject* parent)
    : GameObject(parent, "TimerImage"),
    posX1(0.8), posY(0.85), posZ(0.0),
    posX2(0.9), timerValue(99), frameCounter(0)
{
}

TimerImage::~TimerImage()
{
}

void TimerImage::Initialize()
{
    hTimerImage_ = Image::Load("Image/Text/Number.png");
    assert(hTimerImage_ >= 0);

    transform_.scale_ = { 0.5, 0.5, 0.5 };
    transform_.position_ = { posX1, posY, posZ };
}

void TimerImage::Update()
{
    frameCounter++;
    if (frameCounter >= 60) // 60�t���[����1�b�Ɖ���
    {
        frameCounter = 0;
        timerValue--;
        if (timerValue < 0)
        {
            // �V�[����؂�ւ�
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
        }
    }
}

void TimerImage::Draw()
{
    int spriteWidth = 1024 / 13;

    int spriteHeight = 158;

    // 1���ڂ�`��
    int digit1 = timerValue / 10;
    transform_.position_ = { posX1, posY, posZ };
    Image::SetRect(hTimerImage_, digit1 * spriteWidth, 0, spriteWidth, spriteHeight);
    Image::SetTransform(hTimerImage_, transform_);
    Image::Draw(hTimerImage_);

    // 2���ڂ�`��
    int digit2 = timerValue % 10;
    transform_.position_ = { posX2, posY, posZ };
    Image::SetRect(hTimerImage_, digit2 * spriteWidth, 0, spriteWidth, spriteHeight);
    Image::SetTransform(hTimerImage_, transform_);
    Image::Draw(hTimerImage_);

    //// ImGui�Ńf�o�b�O�p�|�W�V��������
    //{
    //    static float pos[3] = { posX1, posY, posZ };
    //    ImGui::Separator();

    //    if (ImGui::InputFloat3("TimerImage_Position", pos, "%.3f"))
    //    {
    //        posX1 = pos[0];
    //        posY = pos[1];
    //        posZ = pos[2];
    //        posX2 = posX1 + 0.1f; // ���Ԃ̊Ԋu�𒲐�
    //    }
    //}
}

void TimerImage::Release()
{
}
