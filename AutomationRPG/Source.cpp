#include "Engine.hpp"
#include "AsLib2.hpp"
#include "DTL.hpp"

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>

enum :std::size_t {
	//
	tile_empty,
	tile_sea,
	tile_plains,
	tile_desert,
	tile_mountain,
	tile_plateau,
	tile_lake,
	tile_forest,

	tile_sea_home,
	tile_plains_home,
	tile_desert_home,
	tile_plateau_home,
	tile_lake_home,
	tile_forest_home,

	tile_sea_dungeon,
	tile_plains_dungeon,
	tile_desert_dungeon,
	tile_plateau_dungeon,
	tile_lake_dungeon,
	tile_forest_dungeon,

	tile_num
};

class View_Class {
public:
	template<typename Matrix_>
	void operator()(Matrix_& matrix_, [[maybe_unused]]const std::int_fast32_t x_, [[maybe_unused]]const std::int_fast32_t y_, const std::int_fast32_t array_x_, const std::int_fast32_t array_y_, const std::int_fast32_t draw_x_, const std::int_fast32_t draw_y_, const std::int_fast32_t w_, const std::int_fast32_t h_) const noexcept {
		switch (matrix_[array_y_][array_x_]) {
		case tile_lake:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(42, 92, 161)).draw(); break;
		case tile_plains:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(101, 163, 56)).draw(); break;
		case tile_desert:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(223, 203, 140)).draw(); break;
		case tile_sea:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(41, 40, 159)).draw(); break;
		case tile_plateau:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(101, 203, 56)).draw(); break;
		case tile_mountain:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(164, 143, 50)).draw(); break;
		case tile_forest:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(9, 100, 5)).draw(); break;
		case tile_sea_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 0, 255)).draw(); break;
		case tile_sea_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 0, 170)).draw(); break;
		case tile_plains_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(255, 0, 0)).draw(); break;
		case tile_plains_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(170, 0, 0)).draw(); break;
		case tile_desert_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(255, 255, 0)).draw(); break;
		case tile_desert_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(170, 170, 0)).draw(); break;
		case tile_plateau_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(255, 0, 255)).draw(); break;
		case tile_plateau_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(170, 0, 170)).draw(); break;
		case tile_lake_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 255, 255)).draw(); break;
		case tile_lake_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 170, 170)).draw(); break;
		case tile_forest_home:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 255, 0)).draw(); break;
		case tile_forest_dungeon:as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(0, 170, 0)).draw(); break;
		}

		//if (matrix_[array_y_][array_x_] < tile_sea_home) {
		//	as::Rect(draw_x_, draw_y_, w_, h_, as::makeColor(255, 255, 255, 100)).draw();
		//}
	}
};

template<typename Matrix_Int_, typename Matrix_>
std::pair<std::int_fast32_t, std::int_fast32_t> setHuman(const Matrix_& matrix_, const Matrix_Int_ clear_value_) noexcept {
	std::vector<std::pair<std::int_fast32_t, std::int_fast32_t>> value_pairs{};

	for (std::int_fast32_t row{}; row < matrix_.size(); ++row)
		for (std::int_fast32_t col{}; col < matrix_[row].size(); ++col)
			if (matrix_[row][col] == clear_value_) value_pairs.emplace_back(std::make_pair(col, row));
	if (value_pairs.size() == 0) return std::make_pair(1, 1);
	value_pairs.shrink_to_fit();

	std::int_fast32_t index{ static_cast<std::int_fast32_t>(dtl::random::mersenne_twister_32bit(static_cast<std::int_fast32_t>(value_pairs.size()))) };
	return value_pairs[index];
}

template<typename Matrix_, typename Matrix_Int_>
void cutM(Matrix_& matrix_, const Matrix_Int_& value_) noexcept {
	int tmp{ 1 };
	for (std::size_t row{}; row < matrix_.size(); ++row)
		for (std::size_t col{}, col2{}; col < matrix_[row].size(); col = col2 + 1) {
			if (matrix_[row][col] != value_) continue;
			tmp = 1;
			for (col2 = col; col2 < matrix_[row].size(); ++col2) {
				if (matrix_[row][col2] != value_) break;
				++tmp;
			}
		}


}

//フィールドマップの大きさ
constexpr std::size_t field_w{ 256 };
constexpr std::size_t field_h{ 256 };
constexpr std::uint_fast8_t mountown{ 255 };

class GameManager {
private:

	int loop_num{ 120 }, re_num{ loop_num };
	using shape_t = std::uint_fast8_t;
	//フィールドマップデータ
	std::array<std::array<shape_t, field_w>, field_h> matrix{ {} };

	//std::array<std::array<shape_t, field_w>, field_h> lv_matrix{ {} };

	int hachi{ 4 };

	//


	dtl::entity::MobManager mob;
	dtl::camera::MatrixView dv;

public:

	void createField() noexcept {
		//地形生成
		dtl::shape::CellularAutomatonMixIsland<shape_t>(200, tile_lake, tile_plains, tile_plains, tile_desert, tile_mountain, tile_forest).draw(matrix);
		//
		dtl::retouch::RemovePoint<shape_t>().draw(matrix);
		std::vector<std::vector<shape_t>> lv_matrix(field_h, std::vector<shape_t>(field_w, 0));
		for (size_t y{}; y < lv_matrix.size(); ++y)
			for (size_t x{}; x < lv_matrix[y].size(); ++x)
				lv_matrix[y][x] = matrix[y][x];

		dtl::artist::Bucket<shape_t>().paint(matrix, 0, 0, tile_sea);

		dtl::artist::Bucket<shape_t>().paint(lv_matrix, 0, 0, mountown);
		for (size_t y{ 1 }; y < lv_matrix.size() - 1; ++y)
			for (size_t x{ 1 }; x < lv_matrix[y].size() - 1; ++x) {
				if (lv_matrix[y][x] == mountown || lv_matrix[y][x] == tile_mountain) continue;
				if (lv_matrix[y][x - 1] == mountown || lv_matrix[y][x + 1] == mountown || lv_matrix[y - 1][x] == mountown || lv_matrix[y + 1][x] == mountown) {
					dtl::artist::Bucket<shape_t>().paint(lv_matrix, x, y, mountown);
				}
			}
		for (size_t y{ 1 }; y < lv_matrix.size() - 1; ++y)
			for (size_t x{ 1 }; x < lv_matrix[y].size() - 1; ++x) {
				if (lv_matrix[y][x] == mountown || lv_matrix[y][x] == tile_mountain) continue;
				matrix[y][x] = tile_plateau;
			}
		dtl::utility::stl::setItem(matrix, 4, tile_sea_home, tile_sea);
		dtl::utility::stl::setItem(matrix, 4, tile_sea_dungeon, tile_sea);

		dtl::utility::stl::setItem(matrix, 16, tile_plains_home, tile_plains);
		dtl::utility::stl::setItem(matrix, 16, tile_plains_dungeon, tile_plains);

		dtl::utility::stl::setItem(matrix, 3, tile_desert_home, tile_desert);
		dtl::utility::stl::setItem(matrix, 3, tile_desert_dungeon, tile_desert);

		dtl::utility::stl::setItem(matrix, 3, tile_plateau_home, tile_plateau);
		dtl::utility::stl::setItem(matrix, 2, tile_plateau_dungeon, tile_plateau);

		dtl::utility::stl::setItem(matrix, 4, tile_lake_home, tile_lake);
		dtl::utility::stl::setItem(matrix, 4, tile_lake_dungeon, tile_lake);

		dtl::utility::stl::setItem(matrix, 4, tile_forest_home, tile_forest);
		dtl::utility::stl::setItem(matrix, 4, tile_forest_dungeon, tile_forest);
	}
	void output() noexcept {

		int a{}, b{}, c{};
		for (size_t y{}; y < matrix.size(); ++y)
			for (size_t x{}; x < matrix[y].size(); ++x) {
				a = dtl::random::mersenne_twister_32bit(20) - 10;
				b = dtl::random::mersenne_twister_32bit(20) - 10;
				c = dtl::random::mersenne_twister_32bit(20) - 10;
				switch (matrix[y][x]) {
				case tile_lake:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(42 + a, 92 + b, 161 + c)).draw(); break;
				case tile_plains:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(101 + a, 163 + b, 56 + c)).draw(); break;
				case tile_desert:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(223 + a, 203 + b, 140 + c)).draw(); break;
				case tile_sea:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(41 + a, 40 + b, 159 + c)).draw(); break;
				case tile_plateau:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(101 + a, 203 + b, 56 + c)).draw(); break;
				case tile_mountain:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(164 + a, 143 + b, 50 + c)).draw(); break;
				case tile_forest:as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(9, 100 + b, 5)).draw(); break;
				case tile_sea_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 0, 255)).draw(); break;
				case tile_sea_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 0, 170)).draw(); break;
				case tile_plains_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(255, 0, 0)).draw(); break;
				case tile_plains_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(170, 0, 0)).draw(); break;
				case tile_desert_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(255, 255, 0)).draw(); break;
				case tile_desert_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(170, 170, 0)).draw(); break;
				case tile_plateau_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(255, 0, 255)).draw(); break;
				case tile_plateau_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(170, 0, 170)).draw(); break;
				case tile_lake_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 255, 255)).draw(); break;
				case tile_lake_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 170, 170)).draw(); break;
				case tile_forest_home:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 255, 0)).draw(); break;
				case tile_forest_dungeon:as::Rect((int)(x - 2) * hachi, (int)(y - 2) * hachi, hachi * 3, hachi * 3, as::makeColor(0, 170, 0)).draw(); break;
				}

				if (matrix[y][x] < tile_sea_home) {
					as::Rect((int)x * hachi, (int)y * hachi, hachi, hachi, as::makeColor(255, 255, 255, 100)).draw();
				}

			}

	}
	void init() noexcept {createField();
	
		//mob.setPosition(field_w / 2, field_h / 2);
		mob.setPosition(setHuman(matrix, 2));
		mob.setWalkSpeed(0.2);
		
		dv.setInit((std::int_fast32_t)field_w * hachi, (std::int_fast32_t)field_h * hachi, 15, 15, mob.getPositionX(), mob.getPositionY());
	
	}
	void loop() noexcept {
		while (System::Update()) {
			//if (re_num >= loop_num) {
			//	createField();
			//	output();
			//	re_num = 0;
			//}
			//else ++re_num;
			mob.setCanWalkDirection(matrix, field_w, field_h, dtl::entity::CollisionDetectionTrue());
			mob.manualWalk(
				CheckHitKey(KEY_INPUT_W) == 1,
				CheckHitKey(KEY_INPUT_S) == 1,
				CheckHitKey(KEY_INPUT_A) == 1,
				CheckHitKey(KEY_INPUT_D) == 1);
			mob.move();

			mob.setInner(field_w, field_h);

			dv.setTarget(mob.getPositionX(), mob.getPositionY());

			dv.draw(matrix, field_w, field_h, View_Class());

			DrawCircle((std::int_fast32_t)field_w * hachi / 2, (std::int_fast32_t)field_h * hachi / 2, 15 / 2, 0);
		}
	}
	void end() noexcept {}

};

void Main() {
	GameManager gm;
	gm.init();
	gm.loop();
	gm.end();
}