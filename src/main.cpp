#include <iostream>

#include "engine.h"

class Game : public GameWindow {
public:
	void onCreate() override {
		loadSpriteSheet("sheet.bmp");

		propulsionAnim = createAnimation({ 4, 6, 8, 10, 12 });
		bombAnim = createAnimation({ util::spi(0, 3), util::spi(0, 4), util::spi(0, 5), util::spi(0, 6), util::spi(0, 7), util::spi(0, 8), util::spi(0, 9), util::spi(0, 10) });
	}

	void onUpdate(float delta) override {
		u32 i = animation(propulsionAnim, 20);
		sprite(i, 32, 32);
		sprite(i+1, 40, 32);

		patch(0, 0, 4, 3, 48, 24);

		u32 b = animation(bombAnim, 15);
		sprite(b, 64, 64);
		sprite(b+1, 72, 64);
		sprite(b+2, 80, 64);
	}

	void onDestroy() override {

	}

	AnimationID propulsionAnim, bombAnim;

};

int main(int argc, char const *argv[]) {
	Game game{};
	return game.create("Space Game", 512, 320);
}
