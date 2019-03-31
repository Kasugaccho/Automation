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

class MakeColor {
public:
	template<typename Int_>
	constexpr Int_ operator()(const Int_& red_, const Int_& green_, const Int_& blue_) const noexcept {
		return red_ * 256 * 256 + green_ * 256 + blue_;
	}
};