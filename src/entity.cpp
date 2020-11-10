#include "entity.h"

void Entity::onUpdate(GameWindow* game, float dt) {
	switch (m_type) {
		default: break;

		case EntityType::Player: {
			if (game->keyHeld(SDLK_w)) {
				y -= i32(player.speed * dt);
			} else if (game->keyHeld(SDLK_s)) {
				y += i32(player.speed * dt);
			}

			if (game->keyHeld(SDLK_a)) {
				x -= i32(player.speed * dt);
			} else if (game->keyHeld(SDLK_d)) {
				x += i32(player.speed * dt);
			}

			i32 tx = x - 16, ty = y - 12;
			u32 i = game->animation("propulsor", 20);
			game->sprite(i, tx - 16, ty + 8);
			game->sprite(i+1, tx - 8, ty + 8);
			game->patch(0, 0, 4, 3, tx, ty);
		} break;
	}
}
