#include "KeyImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Player.h"


KeyImage::KeyImage(GameObject* parent) :GameObject(parent, "KeyImage"),posX(2.0), posY(0.8), posZ(0.0)
{
}

KeyImage::~KeyImage()
{
}

void KeyImage::Initialize()
{
	hKeyImage_[0] = Image::Load("Image/Image_Key.png");
	hKeyImage_[1] = Image::Load("Image/Image_GetKey.png");
	//hKeyImage_[2] = Image::Load("Image/NoKey.png");
	assert(hKeyImage_[0] >= 0);

	transform_.scale_ = { 0.6,0.6,0.6 };
	transform_.position_ = { 0.4,posY,posZ };
}

void KeyImage::Update()
{
	

	
}

void KeyImage::Draw()
{
	
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
	{
	/*	GetChangeImageFlag = pPlayer->GetClearFlag();

		if (!GetChangeImageFlag)
		{
			Image::SetTransform(hKeyImage_[0], transform_);
			Image::Draw(hKeyImage_[0]);

		}
		else
		{
			Image::SetTransform(hKeyImage_[1], transform_);
			Image::Draw(hKeyImage_[1]);
		}*/

		

	}


	//{
	//	static float pos[3] = { posX,posY,posZ };
	//	ImGui::Separator();

	//	if (ImGui::InputFloat3("KeyImage_Position", pos, "%.3f"))
	//	{
	//		transform_.position_ = { pos[0],pos[1], pos[2] };
	//	}
	//}

}

void KeyImage::Release()
{
}

