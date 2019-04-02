#pragma once

namespace as {

	using Color = unsigned int;
	using SubColor = unsigned int;
	using Texture = int;
	using SubTexture = int;

	struct ColorData {
		SubColor r{};
		SubColor g{};
		SubColor b{};
		SubColor a{ 255 };
		constexpr ColorData() noexcept = default;
		constexpr ColorData(const SubColor& r_, const SubColor& g_, const SubColor& b_) noexcept :r(r_), g(g_), b(b_) {}
		constexpr ColorData(const SubColor& r_, const SubColor& g_, const SubColor& b_, const SubColor& a_) noexcept :r(r_), g(g_), b(b_), a(a_) {}
	};

	//RGBから色情報を送る
	class OriginColor {
	public:
		constexpr Color operator()(const ColorData& color_data_) const noexcept {
			return static_cast<Color>(color_data_.r * 256 * 256 + color_data_.g * 256 + color_data_.b);
		}
		template<typename Int_, typename Int2_, typename Int3_>
		constexpr Color operator()(const Int_& red_, const Int2_& green_, const Int3_& blue_) const noexcept {
			return static_cast<Color>(red_) * 256 * 256 + static_cast<Color>(green_) * 256 + static_cast<Color>(blue_);
		}
	};
	static thread_local OriginColor originColor;

	//RGBから色情報を送る
	class MakeColor {
	public:
		template<typename Int_, typename Int2_, typename Int3_>
		constexpr ColorData operator()(const Int_& red_, const Int2_& green_, const Int3_& blue_) const noexcept {
			return ColorData(static_cast<SubColor>(red_), static_cast<SubColor>(green_), static_cast<SubColor>(blue_));
		}
		template<typename Int_, typename Int2_, typename Int3_, typename Int4_>
		constexpr ColorData operator()(const Int_& red_, const Int2_& green_, const Int3_& blue_, const Int4_& alpha_) const noexcept {
			return ColorData(static_cast<SubColor>(red_), static_cast<SubColor>(green_), static_cast<SubColor>(blue_), static_cast<SubColor>(alpha_));
		}
	};
	static thread_local MakeColor makeColor;

	class Rect {
	public:

		SubTexture x{};
		SubTexture y{};
		SubTexture w{};
		SubTexture h{};
		ColorData color{};

		Rect(const SubTexture& x_, const SubTexture& y_, const SubTexture& w_, const SubTexture& h_)
			:x(x_), y(y_), w(w_), h(h_) {}
		Rect(const SubTexture& x_, const SubTexture& y_, const SubTexture& w_, const SubTexture& h_, const ColorData& color_)
			:x(x_), y(y_), w(w_), h(h_), color(color_) {}

		void draw() noexcept {
			if (color.a != 255) SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a);
			DrawBox(x, y, x + w, y + h, as::originColor(color), TRUE);
			if (color.a != 255) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	};


}