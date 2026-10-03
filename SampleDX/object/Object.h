#pragma once

#include"../directx/ConstantBuffer.h"
#include"../directx/CommandList.h"
#include"../util.h"

namespace game {

	class Object
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Object() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Object() = default;

		/// <summary>
		/// オブジェクト初期化
		/// </summary>
		virtual void initialize()noexcept {};

		/// <summary>
		/// オブジェクトの更新
		/// </summary>
		virtual void update() noexcept {};

		/// <summary>
		/// 描画コマンド設定
		/// </summary>
		/// <param name="commandList"></param>
		/// <param name="slot"></param>
		virtual void setDrawCommand(const CommandList& commandList, UINT slot)noexcept;

	public:

		/// <summary>
		/// 描画用バッファの作成
		/// </summary>
		virtual void createDrawBuffer()noexcept = 0;

		/// <summary>
		/// 描画バッファの更新
		/// </summary>
		virtual void updateDrawBuffer()noexcept = 0;

	public:

		/// <summary>
		/// オブジェクトタイプIDの取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] UINT64 typeId() const noexcept {
			return typeId_;
		}

		/// <summary>
		/// オブジェクトタイプIDの設定
		/// </summary>
		/// <param name="id"></param>
		void setTypeId(UINT64 id)noexcept {
			typeId_ = id;
		}

	protected:

		/// <summary>
		/// コンスタントバッファの更新
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="data"></param>
		template<class T>
		void updateConstantBuffer(const T& data)noexcept {
			std::byte* dst{};
			constantBuffer_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&dst));
			memcpy_s(dst, sizeof(T), &data, sizeof(T));
			constantBuffer_.constantBuffer()->Unmap(0, nullptr);
		};

	protected:
		ConstantBuffer constantBuffer_{};
		UINT64 typeId_{};
	};
}



