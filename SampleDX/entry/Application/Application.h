#pragma

#include<memory>

#include"../../MoveProhibited/Nomove.h"
#include"../../window/Window_Base.h"

#include"../../directx/Device.h"
#include"../../directx/DXGI.h"
#include"../../directx/CommandAllocator.h"
#include"../../directx/CommandList.h"
#include"../../directx/CommandQueue.h"
#include"../../directx/SwapChain.h"
#include"../../directx/DescriptorHeap.h"
#include"../../directx/RenderTarget.h"
#include"../../directx/Fence.h"
#include"../../directx/RootSignature.h"
#include"../../directx/Shader.h"
#include"../../directx/PiplineStateObject.h"

#include"../../draw_resource/TrianglePolygon.h"

#include<cassert>

struct HWND__;

using HWND = HWND__*;

struct HINSTANCE__;

using HINSTANCE = HINSTANCE__*;

class Application : public Nomove
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Application() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Application() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化できたらtrueを返す</returns>
	[[nodiscard]] bool initialize(HINSTANCE hInstance);

	/// <summary>
	/// ループ処理
	/// </summary>
	[[nodiscard]] void loop()noexcept;

private:
	std::unique_ptr<Window_Base> windowInstance_;
	DXGI dxgiInstance_{};
	Device deviceInstance_{};
	CommandQueue commandQueueInstance_{};
	SwapChain swaoChainInstance_{};
	DescriptorHeap descriptorHeapInstance_{};
	RenderTarget renderTargetInstance_{};
	CommandAllocator commandAllocatorInstance_[2]{};
	CommandList commandListInstance_{};

	Fence fenceInstance_{};
	UINT64 frameFenceValue_[2]{};
	UINT64 nextFenceValue_ = 1;

	RootSignature rootSignatureInstance_{};
	Shader shaderInstance_{};
	PiplineStateObject piplineStateObjectInstance_{};
	TrianglePolygon trianglePolygonInstance_{};
};