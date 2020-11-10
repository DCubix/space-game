#include <iostream>

#include "engine.h"
#include "entity.h"

class Game : public GameWindow {
public:
	void onCreate() override {
		loadSpriteSheet("sheet.bmp");

		createAnimation("propulsor", { 4, 6, 8, 10, 12 });

	}

	void onUpdate(float delta) override {
		
	}

	void onDestroy() override {

	}

};

int main(int argc, char const *argv[]) {
	Game game{};
	return game.create("Space Game", 512, 320);
}
