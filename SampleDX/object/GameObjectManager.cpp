#include "GameObjectManager.h"
#include<queue>
#include<algorithm>

namespace game {

	class GameObjectContainer final {
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
}

