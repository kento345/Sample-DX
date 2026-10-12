#include "GameObjectManager.h"
#include<queue>
#include<algorithm>

namespace game {

	class GameObjectContainer final {
	public:
		void clear() noexcept
		{
			objects_.clear();
			creation_.clear();
			hit_.clear();
			waitDelete_.clear();

			creation_.shrink_to_fit();
			hit_.shrink_to_fit();
			waitDelete_.shrink_to_fit();
		}

		void registerCreation(std::function<std::unique_ptr<GameObject>()> f, const UINT64 h) noexcept
		{
			creation_.emplace_back(h, std::move(f));
		}

	public:
		std::vector<std::pair<UINT64, std::function<std::unique_ptr<GameObject>()>>> creation_{};
		std::vector<UINT64> hit_{};

		std::vector<std::unique_ptr<GameObject>> objects_{};
		std::unordered_map <UINT64, UINT> objectIndex_{};
		std::queue<UINT> freeIndex_{};

		std::vector<std::pair<std::unique_ptr<GameObject>, UINT>> waitDelete_{};
		std::vector<UINT64> deleteHandle_{};
	};
	GameObjectContainer container_{};


	void GameObjectManager::update() noexcept
	{
		if (!container_.creation_.empty()) {
			for (auto& create : container_.creation_)
			{
				auto p = create.second();
				p->setHandle(create.first);
				p->initialize();

				auto index = container_.objects_.size();
				if (!container_.freeIndex_.empty()) {
					index = container_.freeIndex_.front();
					container_.freeIndex_.pop();
				}
				else
				{
					container_.objects_.emplace_back();
				}

				container_.objects_[index] = std::move(p);
				container_.objectIndex_.emplace(create.first, static_cast<UINT>(index));
			}
			container_.creation_.clear();
		}

		for (auto& obj : container_.objects_) {
			if (!obj) {
				continue;
			}
			obj->update();
		}
	}
	void GameObjectManager::postUpdate() noexcept
	{
		if (!container_.hit_.empty()) {
			for (auto handle : container_.hit_) {
				auto obj = gameObject(handle);
				if (!obj) {
					continue;
				}
				auto myPos = obj.value()->world().r[3];

				for (auto& target : container_.objects_) {
					if (!target || target->handle() == handle ||
						target->typeId() != obj.value()->hitTargetTypeId()) {
						continue;
					}
					auto hitRadius = obj.value()->radius() + target->radius();
					auto targetPos = target->world().r[3];
					auto distace = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(myPos, targetPos)));
					if (distace < hitRadius) {
						obj.value()->OnHit();
						target->OnHit();
					}
				}
			}
			container_.hit_.clear();
		}
		registerWaitDelete();

		if (!container_.waitDelete_.empty()) {
			for (auto d = container_.waitDelete_.begin(); d != container_.waitDelete_.end();) {
				if ((d->second--) > 0) {
					++d;
					continue;
				}
				d = container_.waitDelete_.erase(d);
			}
		}
	}

	void GameObjectManager::draw(const CommandList& commandList) noexcept
	{
		constexpr UINT objectShaderSlot_ = 1;

		std::vector<GameObject*> drawObjects{};
		for (auto& obj : container_.objects_) {
			if (!obj) {
				continue;
			}

			drawObjects.emplace_back(obj.get());
		}
		std::sort(drawObjects.begin(), drawObjects.end(),
			[](const GameObject* a, const GameObject* b) {
				return a->handle() < b->handle();});

		for (auto& obj : drawObjects) {
			obj->updateDrawBuffer();
			obj->setDrawCommand(commandList, objectShaderSlot_);
		}
	}
	void GameObjectManager::clear() noexcept
	{
		container_.clear();
	}

	std::optional<GameObject*> GameObjectManager::gameObject(UINT64 handle) noexcept
	{
		auto it = container_.objectIndex_.find(handle);
		if (it == container_.objectIndex_.end()) {
			auto find = std::find_if(container_.creation_.begin(), container_.creation_.end(),
				[handle](const auto& pair) { { return pair.first == handle; }});
			if (find == container_.creation_.end()) {
				return std::nullopt;
			}
		}
		return container_.objects_[it->second].get();
	}

	void GameObjectManager::registerHit(UINT64 handle) noexcept
	{
		container_.hit_.emplace_back(handle);
	}

	void GameObjectManager::registerDelete(UINT64 handle) noexcept
	{
		if (std::find(container_.deleteHandle_.begin(), container_.deleteHandle_.end(), handle) != container_.deleteHandle_.end()) {
			return;
		}
		container_.deleteHandle_.emplace_back(handle);
	}

	void GameObjectManager::registerCreation(std::function<std::unique_ptr<GameObject>()> create, const UINT64 handle) noexcept
	{
		container_.registerCreation(std::move(create), handle);
	}

	void GameObjectManager::registerWaitDelete() noexcept
	{
		if (container_.deleteHandle_.empty()) {
			return;
		}

		for (auto handle : container_.deleteHandle_) {
			auto it = container_.objectIndex_.find(handle);
			if (it == container_.objectIndex_.end()) {
				continue;
			}

			container_.waitDelete_.emplace_back(std::move(container_.objects_[it->second]), 10);

			container_.objectIndex_.erase(it);
		}

		container_.deleteHandle_.clear();
	}

	GameObjectManager::~GameObjectManager()
	{
		clear();
	}
}



