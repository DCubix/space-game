#include <iostream>

#include "engine.h"
#include "logic.h"

class BulletBehavior : public Behavior {
public:

	void onCreate(GameLogic& logic, GameWindow& game) override {
		spawn = game.createAnimation({ util::spi(4, 1), util::spi(5, 1), util::spi(6, 1), util::spi(7, 1), util::spi(8, 1), util::spi(8, 1) });
		x = 0;
		y = 0;
	}

	void onUpdate(GameLogic& logic, GameWindow& game, float dt) override {
		game.sprite(util::spi(11, 1), x - 5, y - 5);
		game.sprite(util::spi(12, 1), (x + 8) - 5, y - 5);

		if (initial) {
			sx = x;
			sy = y;
			initial = false;
		}

		u32 s = game.animation(spawn, 20, AnimationMode::OneShot);
		if (!game.animationEnded(spawn)) {
			game.sprite(s, sx - 4, sy - 4);
		}

		x += i32(300 * dt);
		if (x-8 >= game.width()) {
			dead = true;
		}
	}

	void onDestroy(GameLogic& logic, GameWindow& game) override {

	}

	u32 spawn;

	i32 x, y, sx, sy;
	bool dead{ false }, initial{ true };
};

class PlayerBehavior : public Behavior {
public:

	void onCreate(GameLogic& logic, GameWindow& game) override {
		prop = game.createAnimation({ 4, 6, 8, 10, 12 });

		x = 50;
		y = game.height() / 2;
	}

	void onUpdate(GameLogic& logic, GameWindow& game, float dt) override {
		if (game.keyHeld(SDLK_w)) {
			y -= i32(200 * dt);
		} else if (game.keyHeld(SDLK_s)) {
			y += i32(200 * dt);
		}

		if (game.keyHeld(SDLK_a)) {
			x -= i32(200 * dt);
		} else if (game.keyHeld(SDLK_d)) {
			x += i32(200 * dt);
		}

		if (game.keyPressed(SDLK_SPACE)) {
			Entity bullet = logic.create();
			logic.behaveAs<BulletBehavior>(bullet);
			auto b = logic.get<BulletBehavior>(bullet);
			b->x = x + 16;
			b->y = y + 1;
		}

		i32 tx = x - 16, ty = y - 12;
		u32 i = game.animation(prop, 20);
		game.sprite(i, tx - 16, ty + 8);
		game.sprite(i+1, tx - 8, ty + 8);
		game.patch(0, 0, 4, 3, tx, ty);
	}

	void onDestroy(GameLogic& logic, GameWindow& game) override {

	}

	u32 prop;
	i32 x, y;

};

class Game : public GameWindow {
public:
	void onCreate() override {
		loadSpriteSheet("sheet.bmp");
		logic.reset(new GameLogic(this));

		Entity player = logic->create();
		logic->behaveAs<PlayerBehavior>(player);
	}

	void onUpdate(float delta) override {
		logic->each([=](Entity e, BulletBehavior& b) {
			if (b.dead) {
				logic->destroy(e);
			}
		});

		logic->update(delta);
	}

	void onDestroy() override {
		logic.reset(nullptr);
	}

	std::unique_ptr<GameLogic> logic;
};

int main(int argc, char const *argv[]) {
	Game game{};
	return game.create("Space Game", 512, 320);
}
