#pragma once

#include"GameObject.h"
#include"../MoveProhibited/Nomove.h"
#include<functional>

namespace game {
	class GameObjectManager final : public Nomove
	{
	public:
		static GameObjectManager& instance() noexcept {
			static GameObjectManager instance;
			return instance;
		}

	public:

		void update() noexcept;

		void postUpdate() noexcept;

		void draw(const CommandList& commandList)noexcept;

		void clear()noexcept;

		[[nodiscard]] std::optional<GameObject*> gameObject(UINT64 handle)noexcept;

		void registerHit(UINT64 handle)noexcept;

		void registerDelete(UINT64 handle)noexcept;

	public:

		template<typename T,typename... Args>
		[[nodiscard]] UINT64 createObject(Args&&... args)noexcept {
			static_assert(std::is_base_of<GameObject, T>::value, "GameObjectではない物を作ろうとしてます");
			const auto handle = ++counter_;

			auto func = [...a = std::forward<Args>(args)]() mutable {
				auto p = std::make_unique<T>(std::move(a)...);
				p->setTypeId(id::get<T>());
				return p;
			};

			registerCreation(std::move(func), handle);
			
			
			return handle;
		}

	private:

		void registerCreation(std::function<std::unique_ptr<GameObject>()> creation, const UINT64 handle)noexcept;

		void registerWaitDelete()noexcept;

	private:

		GameObjectManager() = default;
		
		~GameObjectManager() = default;

	private:
		UINT64 counter_{};
	};
}