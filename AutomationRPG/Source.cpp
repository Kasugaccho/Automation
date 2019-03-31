#include "Engine.hpp"
#include "DTL.hpp"
#include <cstddef>
#include <cstdint>
#include <array>

constexpr std::size_t size_x{ 256 };
constexpr std::size_t size_y{ 256 };

class GameManager {
private:
	
	int loop_num{ 30 }, re_num{ loop_num };
	using shape_t = std::uint_fast8_t;
	std::array<std::array<shape_t, size_x>, size_y> matrix{ {} };
	int hachi{ 4 };
	enum :std::size_t {
		tile_empty,
		tile_sea,
		tile_plains,
		tile_desert,
		tile_mountain,
		tile_lake,
		tile_forest,
		tile_num
	};

public:

	void createWorld() noexcept {
		dtl::shape::CellularAutomatonMixIsland<shape_t>(200, tile_lake, tile_plains, tile_plains, tile_desert, tile_mountain, tile_forest).draw(matrix);
	}
	void output() noexcept {

		int a{}, b{}, c{};
		for (size_t y{}; y < matrix.size(); ++y)
			for (size_t x{}; x < matrix[y].size(); ++x) {
				a = dtl::random::mersenne_twister_32bit(20) - 10;
				b = dtl::random::mersenne_twister_32bit(20) - 10;
				c = dtl::random::mersenne_twister_32bit(20) - 10;
				switch (matrix[y][x])
				{
				case tile_sea:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(42 + a, 92 + b, 161 + c), TRUE);break;
				case tile_plains:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(101 + a, 163 + b, 56 + c), TRUE);break;
				case tile_desert:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(223 + a, 203 + b, 140 + c), TRUE);break;
				//case 4:DrawBox((int)(x - 2) * hachi, (int)(y - 2) * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(255, 0, 0), TRUE);break;
				case tile_lake:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(41 + a, 40 + b, 159 + c), TRUE);break;
				//case 6:DrawBox((int)(x - 2) * hachi, (int)(y - 2) * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(200, 0, 255), TRUE);break;
				//case 7:DrawBox((int)(x - 2) * hachi, (int)(y - 2) * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(0, 255, 255), TRUE);break;
				case tile_mountain:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(164 + a, 143 + b, 50 + c), TRUE);break;
				case tile_forest:DrawBox((int)x * hachi, (int)y * hachi, (int)(x + 1) * hachi, (int)(y + 1) * hachi, MakeColor()(9, 100 + b, 5), TRUE);break;
				}

			}

	}

	void init() noexcept {
		//createWorld();
	}
	void loop() noexcept {
		while (System::Update()) {
			if (re_num >= loop_num) {
				createWorld();
				output();
				re_num = 0;
			}
			else ++re_num;
			
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