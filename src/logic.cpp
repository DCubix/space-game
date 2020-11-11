#include "logic.h"

Entity GameLogic::create() {
	const u64 id = m_currentID++;
	m_behaviors[id] = BehaviorList();
	return id;
}

void GameLogic::destroy(Entity entity) {
	for (auto& p : m_behaviors[entity]) {
		p->onDestroy(*this, *m_game);
	}
	m_behaviors.erase(entity);
}

void GameLogic::update(float dt) {
	for (auto& [entity, bs] : m_behaviors) {
		std::for_each(bs.begin(), bs.end(), [dt, this](BehaviorPtr& b){
			b->onUpdate(*this, *m_game, dt);
		});
	}
}