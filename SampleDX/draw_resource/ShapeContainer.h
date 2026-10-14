#pragma once

#include"PolygonBase.h"
#include"../MoveProhibited/Nomove.h"
#include<unordered_map>
#include<memory>
#include<cassert>

class ShapeContainer : public Nomove
{
public:
	static ShapeContainer& instance() {
		static ShapeContainer instance;
		return instance;
	}

public:
	template<class T>
	UINT64 create()noexcept {
		const auto id = id::get<T>();
		if (shapes_.find(id) != shapes_.end()) {
			// 既に存在している場合は何もしない
			return id;
		}

		// 形状の生成と登録
		auto p = std::make_unique<T>();
		if (!p->create()) {
			assert(false && "形状の生成に失敗しました");
			return 0;
		}
		shapes_.emplace(id, std::move(p));
		return id;
	}

	void draw(const CommandList& commandList, UINT64 id)noexcept;
	
private:
	ShapeContainer() = default;
	~ShapeContainer() = default;

private:
	std::unordered_map<UINT64, std::unique_ptr<PolygonBase>> shapes_;
};

