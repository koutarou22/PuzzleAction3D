#include "Residue.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"



Residue::Residue(GameObject* parent):GameObject(parent,"Residue")
,posX(-0.9),posY(0.9),posZ(0.0),LIFE_(2)
{
}

Residue::~Residue()
{
}

void Residue::Initialize()
{
	hResidueImage_ = Image::Load("Image/num.png");
	hNum_ = Image::Load("Image/Image3.png");
	//hNum_ = Image::Load("Image/Image_GetKey.png");
	//hKeyImage_[2] = Image::Load("Image/NoKey.png");
	assert(hResidueImage_ >= 0);


	trs.position_.x = posX + 0.2;
	trs.position_.y = posY;

	transform_.scale_ = { 0.6,0.6,0.6 };
	trs.scale_ = { 0.3,0.3,0.3 };
	transform_.position_ = { posX,posY,posZ };
}

void Residue::Update()
{
}

void Residue::Draw()
{

	int spriteWidth = 1024/5;

	int spriteHeight = 821/3;


	Image::SetTransform(hResidueImage_, transform_);
	Image::Draw(hResidueImage_);

	
	Image::SetRect(hNum_, spriteWidth * LIFE_, 0, spriteWidth, spriteHeight);
	Image::SetTransform(hNum_, trs);
	Image::Draw(hNum_);
	{
		static float pos[3] = { posX,posY,posZ };
		ImGui::Separator();

		if (ImGui::InputFloat3("Resudeue_Position", pos, "%.3f"))
		{
			transform_.position_ = { pos[0],pos[1], pos[2] };
		}
	}
}

void Residue::Release()
{
}
