#ifndef ACTOR_H
#define ACTOR_H

#define ENGINE_ONLY_PRIMITIVES
#include "engine.h"

enum class EntityType {
	Empty = 0,

	Player,
	EnemySimple,
	EnemyWavy,
	EnemyQuick,
	EnemyKamikaze,
	EnemyBoss,
	EnemyBullet,

	PlayerBullet,
	PlayerNuke,
	PlayerShockwave,

	Meteor,

	PowerUpHealth,
	PowerUpNuke,
	PowerUpShockwave,
	PowerUpShield
};

class Entity {
public:
	Entity() = default;
	~Entity() = default;

	void onUpdate(float dt);

private:
	i32 m_x, m_y;

	struct {
		u32 health{ 100 };
		float speed{ 30.0f };
		enum {
			SpecialNone = 0,
			SpecialNuke,
			SpecialShockwave
		} special{ SpecialNone };
		u32 specialCount{ 0 };
		bool hasShield{ false };
		float shieldTime{ 10.0f };
	} m_player;

	struct {
		float speed{ 60.0f };
		int xDirection{ 1 };
	} m_bullet;

	EntityType m_type{ EntityType::Empty };
};

#endif // ACTOR_H