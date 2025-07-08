#include "Residue.h"
#include "Engine/Image.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Engine/SceneManager.h"



Residue::Residue(GameObject* parent):GameObject(parent,"Residue")
,posX(-0.9),posY(0.9),posZ(0.0)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		LIFE_ = pSceneManager->GetPlayerResidue(); // SceneManager から残機を持ってくる
	}
}

Residue::~Residue()
{
}

void Residue::Initialize()
{
	hResidueImage_ = Image::Load("Image/num1.png");
	hNum_ = Image::Load("Image/Image4.png");

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

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	int spriteWidth = 1024 / 5;
	int spriteHeight = 821 / 3;

	int PlayerResidue = pSceneManager->GetPlayerResidue();

	//残機が 1 未満の場合は「1」を表示**
	int displayResidue;

	if (PlayerResidue < 1)
	{
		displayResidue = 1;
	}
	else
	{
		displayResidue = PlayerResidue;
	}


	//スプライトの位置計算
	int spriteX = spriteWidth * (displayResidue - 1);
	int spriteY = 0;

	Image::SetTransform(hResidueImage_, transform_);
	Image::Draw(hResidueImage_);

	

	Image::SetRect(hNum_, spriteX, spriteY, spriteWidth, spriteHeight);
	Image::SetTransform(hNum_, trs);
	Image::Draw(hNum_);


	Image::SetAlpha(hResidueImage_, ResiduedueAlpha_*255);
	Image::SetAlpha(hNum_, ResiduedueAlpha_*255);

}

void Residue::Release()
{
}

void Residue::SetPosition(float x, float y, float z)
{
	transform_.position_ = { x, y, z };
	trs.position_ = { x + 0.2f, y, z };
}

void Residue::SetAlpha(float alpha)
{
   ResiduedueAlpha_ = alpha;
}
