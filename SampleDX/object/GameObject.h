#pragma once

#include"Object.h"
#include<DirectXMath.h>

namespace game {
	class GameObject : public Object
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameObject() = default;
		GameObject(UINT64 parent) : parent_(parent) {}
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~GameObject() = default;

		virtual void initialize()noexcept override;

		/// <summary>
		/// オブジェクトの更新
		/// </summary>
		virtual void update() noexcept override;

		virtual void setDrawCommand(const CommandList& commandList, UINT slot)noexcept override;

	public:
		/// <summary>
		/// 描画用バッファの作成
		/// </summary>
		virtual void createDrawBuffer()noexcept override;

		/// <summary>
		/// 描画用バッファの更新
		/// </summary>
		virtual void updateDrawBuffer()noexcept override;

	public:

		virtual void OnHit() noexcept {};
		
		[[nodiscard]] virtual UINT64 hitTargetTypeId() noexcept { return{}; }

	public:
		void setHandle(UINT64 handle)noexcept;

		[[nodiscard]] UINT64 handle() const noexcept;


		void set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, UINT64 shapeId)noexcept;

		/// <summary>
		/// ワールド行列の取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] DirectX::XMMATRIX world()const noexcept;

		/// <summary>
		/// カラーの取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] DirectX::XMFLOAT4 color()const noexcept;

		[[nodiscard]] float radius()noexcept { return radius_; };

	protected:
		DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();				  //ワールド行列
		DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //カラー(RGBA)
		UINT64 shapeId_{};
		UINT64 handle_{};
		UINT64 parent_{};
		float  radius_{};
	};
}