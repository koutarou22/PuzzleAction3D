#include "TimerImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Player.h"


TimerImage::TimerImage(GameObject* parent) :GameObject(parent, "TimerImage"), posX(0.85), posY(0.85), posZ(0.0)
{
}

TimerImage::~TimerImage()
{
}

void TimerImage::Initialize()
{

	hTimerImage_ = Image::Load("Image/Text/Number.png");
	assert(hTimerImage_ >= 0);

	transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_ = { posX,posY,posZ };
}

void TimerImage::Update()
{
}

void TimerImage::Draw()
{



	Player* pPlayer = (Player*)FindObject("Player");


	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;

	//‰æ‘œƒTƒCƒY‚ðŠi”[
	int spriteWidth = 1024 / 13;
	int spriteHeight = 158;

	Image::SetRect(hTimerImage_, x, y, spriteWidth, spriteHeight);
	Image::SetTransform(hTimerImage_, transform_);
	Image::Draw(hTimerImage_);

	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("TimerImage_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}

}

void TimerImage::Release()
{
}

