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

	void onUpdate(GameWindow* game, float dt);

	const EntityType& type() const { return m_type; }
	void type(const EntityType& type) { m_type = type; }

	i32 x{ 0 }, y{ 0 };

private:
	EntityType m_type{ EntityType::Empty };

	struct {
		float speed{ 200.0f };

	} player;
};

#endif // ACTOR_H
