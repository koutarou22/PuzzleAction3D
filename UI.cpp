#include "UI.h"
#include "Engine/Image.h"
#include "KeyImage.h"
#include "TextImage.h"

UI::UI(GameObject* parent) :GameObject(parent, "UI")
{
}

UI::~UI()
{
}

void UI::Initialize()
{
	Instantiate<KeyImage>(this);
	Instantiate<TextImage>(this);
}

void UI::Update()
{

}

void UI::Draw()
{
	
}

void UI::Release()
{
}

