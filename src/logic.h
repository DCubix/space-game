#ifndef LOGIC_H
#define LOGIC_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <algorithm>
#include <functional>

#include "engine.h"
using Entity = uint64_t;

using TypeIndex = std::type_index;
template<typename T>
static TypeIndex getTypeIndex() {
	return std::type_index(typeid(T));
}

class GameLogic;
class Behavior {
	friend class GameLogic;
public:
	Behavior() = default;
	~Behavior() = default;

	virtual void onCreate(GameLogic& logic, GameWindow& game) = 0;
	virtual void onUpdate(GameLogic& logic, GameWindow& game, float delta) = 0;
	virtual void onDestroy(GameLogic& logic, GameWindow& game) = 0;

	const Entity& owner() const { return m_owner; }

protected:
	Entity m_owner;
};

using BehaviorPtr = std::unique_ptr<Behavior>;
using BehaviorList = std::vector<BehaviorPtr>;

class GameLogic {
public:
	GameLogic() = default;
	~GameLogic() = default;

	GameLogic(GameWindow* game) : m_game(game) {}

	Entity create();
	void update(float dt);
	void destroy(Entity entity);

	template <class B>
	void behaveAs(Entity entity)  {
		static_assert(
			std::is_base_of<Behavior, B>::value,
			"Behaviors must be derived from 'Behavior'."
		);
		std::unique_ptr<Behavior> b{};
		b.reset(new B());
		b->m_owner = entity;
		b->onCreate(*this, *m_game);
		m_behaviors[entity].push_back(std::move(b));
		m_behaviorTypes[entity].push_back(getTypeIndex<B>());
	}

	template <class B>
	bool has(Entity entity) {
		static_assert(
			std::is_base_of<Behavior, B>::value,
			"Behaviors must be derived from 'Behavior'."
		);
		auto& bs = m_behaviorTypes[entity];
		return std::find(bs.begin(), bs.end(), getTypeIndex<B>()) != bs.end();
	}

	template <typename B, typename V, typename... Types>
	bool has(Entity entity) {
		return has<B>(entity) && has<V, Types...>();
	}

	template <class B>
	B* get(Entity entity) {
		static_assert(
			std::is_base_of<Behavior, B>::value,
			"Behaviors must be derived from 'Behavior'."
		);
		auto& bs = m_behaviorTypes[entity];
		auto pos = std::find(bs.begin(), bs.end(), getTypeIndex<B>());
		if (pos != bs.end()) {
			return (B*) m_behaviors[entity][std::distance(bs.begin(), pos)].get();
		}
		return nullptr;
	}

	// template<class... Bs>
	// void each(void(*f)(Entity, Bs...)) {
	// 	eachInternal<Bs...>(f);
	// }

	template <class Fn>
	void each(Fn&& func) {
		eachInternalLambda(&Fn::operator(), func);
	}

private:
	u64 m_currentID{ 1 };

	std::unordered_map<Entity, std::vector<TypeIndex>> m_behaviorTypes{};
	std::unordered_map<Entity, BehaviorList> m_behaviors{};

	GameWindow* m_game;

	template <class... Bs, class Fn>
	void eachInternal(Fn&& func) {
		for (auto& [e, b] : m_behaviors) {
			if (has<Bs...>(e)) {
				func(e, *get<Bs...>(e));
			}
		}
	}

	template <class G, class... Bs, class Fn>
	void eachInternalLambda(void (G::*)(Entity, Bs&...), Fn&& func) {
		eachInternal<Bs...>(std::forward<Fn>(func));
	}

	template <class G, class... Bs, class Fn>
	void eachInternalLambda(void (G::*)(Entity, Bs&...) const, Fn&& func) {
		eachInternal<Bs...>(std::forward<Fn>(func));
	}
};

#endif // LOGIC_H
