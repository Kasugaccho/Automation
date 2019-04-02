#pragma once
#include "DxLib.h"

namespace System {// && DxLib::ClearDrawScreen() != -1
	bool Update() { return (DxLib::ScreenFlip() != -1 && DxLib::ProcessMessage() != -1); }
}

void Main();
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib::ChangeWindowMode(TRUE);
	SetGraphMode(1024,1024, 32);
	if (DxLib::DxLib_Init() == -1) return -1;
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	Main();
	return DxLib::DxLib_End();
}

