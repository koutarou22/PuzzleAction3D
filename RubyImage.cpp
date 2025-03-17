#include "RubyImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Player.h"


RubyImage::RubyImage(GameObject* parent) :GameObject(parent, "RubyImage"), posX(1.0), posY(0.7), posZ(0.0)
{
}

RubyImage::~RubyImage()
{
}

void RubyImage::Initialize()
{
	hRubyImage_[0] = Image::Load("Image/Image_Ruby.png");
	hRubyImage_[1] = Image::Load("Image/Image_GetRuby.png");
	assert(hRubyImage_[0] >= 0);

	transform_.scale_ = { 0.2,0.2,0.2 };
	transform_.position_ = { 0.65,0.85,posZ };
}

void RubyImage::Update()
{

}

void RubyImage::Draw()
{

	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
	{
		GetChangImageFlag = pPlayer->GetScoreFlag();

		if (!GetChangImageFlag)
		{
			Image::SetTransform(hRubyImage_[0], transform_);
			Image::Draw(hRubyImage_[0]);
		}
		else
		{
			Image::SetTransform(hRubyImage_[1], transform_);
			Image::Draw(hRubyImage_[1]);
		}
	}

	//{
	//	static float pos[3] = { posX,posY,posZ };
	//	ImGui::Separator();

	//	if (ImGui::InputFloat3("RubyImage_Position", pos, "%.3f"))
	//	{
	//		transform_.position_ = { pos[0],pos[1], pos[2] };
	//	}
	//}

}

void RubyImage::Release()
{
}

